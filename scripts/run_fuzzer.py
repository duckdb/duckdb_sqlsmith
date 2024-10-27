import json
import requests
import sys
import os
import subprocess
import reduce_sql
import fuzzer_helper
import random

seed = -1

fuzzer = None
db = None
shell = None
perform_checks = True
no_git_checks = False
max_queries = 1000
verification = False

for param in sys.argv:
    if param == '--sqlsmith':
        fuzzer = 'sqlsmith'
    elif param == '--duckfuzz':
        fuzzer = 'duckfuzz'
    elif param == '--duckfuzz_functions':
        fuzzer = 'duckfuzz_functions'
    elif param == '--alltypes':
        db = 'alltypes'
    elif param == '--tpch':
        db = 'tpch'
    elif param == '--emptyalltypes':
        db = 'emptyalltypes'
    elif param == '--no_checks':
        perform_checks = False
    elif param.startswith('--enable_verification'):
        verification = param.replace('--enable_verification=', '').lower() == 'true'
    elif param.startswith('--randoms_config_filepath'):
        randoms_config_filepath = param.replace('--randoms_config_filepath=', '')
    elif param.startswith('--shell='):
        shell = param.replace('--shell=', '')
    elif param.startswith('--seed='):
        seed = int(param.replace('--seed=', ''))
    elif param.startswith('--max_queries='):
        max_queries = int(param.replace('--max_queries=', ''))
    elif param.startswith('--no-git-checks'):
        no_git_checks = param.replace('--no-git-checks=', '').lower() == 'true'

if fuzzer is None:
    print("Unrecognized fuzzer to run, expected e.g. --sqlsmith or --duckfuzz")
    exit(1)

if db is None:
    print("Unrecognized database to run on, expected either --tpch, --alltypes or --emptyalltypes")
    exit(1)

if shell is None:
    print("Unrecognized path to shell, expected e.g. --shell=build/debug/duckdb")
    exit(1)

if seed < 0:
    seed = random.randint(0, 2**30)

git_hash = os.getenv('DUCKDB_HASH')


def create_db_script(db):
    if db == 'alltypes':
        return 'create table all_types as select * exclude(small_enum, medium_enum, large_enum) from test_all_types();'
    elif db == 'tpch':
        return 'call dbgen(sf=0.1);'
    elif db == 'emptyalltypes':
        return 'create table all_types as select * exclude(small_enum, medium_enum, large_enum) from test_all_types() limit 0;'
    else:
        raise Exception("Unknown database creation script")


def run_fuzzer_script(fuzzer):
    if fuzzer == 'sqlsmith':
        return "call sqlsmith(max_queries=${MAX_QUERIES}, seed=${SEED}, verbose_output=1, log='${LAST_LOG_FILE}', complete_log='${COMPLETE_LOG_FILE}');"
    elif fuzzer == 'duckfuzz':
        return "call fuzzyduck(max_queries=${MAX_QUERIES}, seed=${SEED}, verbose_output=1, log='${LAST_LOG_FILE}', complete_log='${COMPLETE_LOG_FILE}', \
        enable_verification='${ENABLE_VERIFICATION}', randoms_config_filepath='${RANDOMS_CONFIG_FILEPATH}');"
    elif fuzzer == 'duckfuzz_functions':
        return "call fuzz_all_functions(seed=${SEED}, verbose_output=1, log='${LAST_LOG_FILE}', complete_log='${COMPLETE_LOG_FILE}');"
    else:
        raise Exception("Unknown fuzzer type")


def get_fuzzer_name(fuzzer):
    if fuzzer == 'sqlsmith':
        return 'SQLSmith'
    elif fuzzer == 'duckfuzz':
        return 'DuckFuzz'
    elif fuzzer == 'duckfuzz_functions':
        return 'DuckFuzz (Functions)'
    else:
        return 'Unknown'


def run_shell_command(cmd):
    command = [shell, '--batch', '-init', '/dev/null']
    res = subprocess.run(command, input=bytearray(cmd, 'utf8'), stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    stdout = res.stdout.decode('utf8', 'ignore').strip()
    stderr = res.stderr.decode('utf8', 'ignore').strip()
    return (stdout, stderr, res.returncode)


# first get a list of all github issues, and check if we can still reproduce them

if no_git_checks:
    current_errors = []
else:
    current_errors = fuzzer_helper.extract_github_issues(shell, perform_checks)

# Don't go on and fuzz if perform checks = true
if perform_checks:
    exit(0)

last_query_log_file = 'sqlsmith.log'
complete_log_file = 'sqlsmith.complete.log'

print(
    f'''==========================================
        RUNNING {fuzzer} on {db}
=========================================='''
)


load_script = create_db_script(db)
fuzzer_name = get_fuzzer_name(fuzzer)
fuzzer = (
    run_fuzzer_script(fuzzer)
    .replace('${MAX_QUERIES}', str(max_queries))
    .replace('${LAST_LOG_FILE}', last_query_log_file)
    .replace('${COMPLETE_LOG_FILE}', complete_log_file)
    .replace('${SEED}', str(seed))
    .replace('${ENABLE_VERIFICATION}', str(verification))
    .replace('${RANDOMS_CONFIG_FILEPATH}', randoms_config_filepath)
)

print(load_script)
print(fuzzer)

cmd = load_script + "\n" + fuzzer

print("==========================================")

(stdout, stderr, returncode) = run_shell_command(cmd)

print(
    f'''==========================================
        FINISHED RUNNING
=========================================='''
)
print("==============  STDOUT  ================")
print(stdout)
print("==============  STDERR  =================")
print(stderr)
print("==========================================")

print(returncode)
if returncode == 0:
    print("==============  SUCCESS  ================")
    exit(0)

print("==============  FAILURE  ================")
print("Attempting to reproduce and file issue...")

# run the last query, and see if the issue persists
with open(last_query_log_file, 'r') as f:
    last_query = f.read()

with open(complete_log_file, 'r') as f:
    all_queries = f.read()

(stdout, stderr, returncode) = run_shell_command(load_script + '\n' + all_queries)

if returncode == 0:
    print("Failed to reproduce the issue...")
    exit(0)

print("==============  STDOUT  ================")
print(stdout)
print("==============  STDERR  =================")
print(stderr)
print("==========================================")
if not fuzzer_helper.is_internal_error(stderr):
    print("Failed to reproduce the internal error")
    exit(0)

error_msg = reduce_sql.sanitize_error(stderr)

print("=========================================")
print("         Reproduced successfully         ")
print("=========================================")

# check if this is a duplicate issue
if error_msg in current_errors:
    print("Skip filing duplicate issue")
    print(
        "Issue already exists: https://github.com/duckdb/duckdb-fuzzer/issues/"
        + str(current_errors[error_msg]['number'])
    )
    exit(0)

print("=========================================")
print("        Attempting to reduce query       ")
print("=========================================")
# try to reduce the query as much as possible
# reduce_multi_statement checks just the last statement first as a heuristic to see if
# only the last statement causes the error.
required_queries = reduce_sql.reduce_multi_statement(all_queries, shell, load_script)
cmd = load_script + '\n' + required_queries

# get a new error message.
(stdout, stderr, returncode) = run_shell_command(cmd)
error_msg = reduce_sql.sanitize_error(stderr)

print(f"================MARKER====================")
print(f"After reducing: the below sql causes an internal error \n `{cmd}`")
print(f"{error_msg}")
print(f"================MARKER====================")

if not no_git_checks:
    fuzzer_helper.file_issue(cmd, error_msg, fuzzer_name, seed, git_hash)
