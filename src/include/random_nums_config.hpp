// ===----------------------------------------------------------------------===//
//                         DuckDB

// random_nums_config.hpp


// ===----------------------------------------------------------------------===//

#pragma once

#include "duckdb.hpp"
#include "yyjson.hpp"

namespace duckdb {

enum class RandomPercentagesEnum : idx_t {
	SELECT = 0,
    ATTACH = 1,
    ATTACH_USE = 2,
    DETACH = 3,
    SET = 4,
    DELETE = 5,

    // ----------------------------------
	// Generate Select Percentages Types
	// ----------------------------------
    SELECT_NODE = 6,
    SELECT_NODE_IS_DISTINCT = 7,
    SELECT_NODE_FROM_TABLE = 8,
    SELECT_NODE_WHERE = 9,
    SELECT_NODE_HAVING = 10,
    SELECT_NODE_GROUPS = 11,
    SELECT_NODE_GROUP_BY = 12,
    SELECT_NODE_QUALIFY = 13,
    SELECT_NODE_AGGREGATE = 14,
    SELECT_NODE_SAMPLE = 15,
    SELECT_NODE_SAMPLE_IS_PERC = 16,
    SELECT_NODE_SAMPLE_SIZE = 17,
    RESULT_MODIFIERS = 18,
    LIMIT_PERCENT_MODIFIER = 19,
    LIMIT_PERCENT_MODIFIER_LIMIT = 20,
    LIMIT_PERCENT_MODIFIER_OFFSET = 21,
    LIMIT_MODIFIER_LIMIT = 22,
    LIMIT_MODIFIER_OFFSET = 23,

    COUNT

};

unordered_map<RandomPercentagesEnum, idx_t> GetDefaultConfig();
unordered_map<RandomPercentagesEnum, idx_t> GetConfigFromFile(const char *json_string);
string RandomPercentagesEnumToString(RandomPercentagesEnum type);

} // namespace duckdb