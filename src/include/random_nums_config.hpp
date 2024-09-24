// ===----------------------------------------------------------------------===//
//                         DuckDB

// random_nums_config.hpp


// ===----------------------------------------------------------------------===//

#pragma once

#include "duckdb.hpp"
#include "percentages_enum.hpp"
#include "yyjson.hpp"

namespace duckdb {

enum class RandomNumsConfig : idx_t {
	SELECT = 0,
    ATTACH = 1,
    ATTACH_USE = 2,
    DETACH = 3,
    SET = 4,
    DELETE = 5,
};

unordered_map<RandomNumsConfig, idx_t> GetDefaultConfig();
unordered_map<RandomNumsConfig, idx_t> GetConfigFromFile(const char *json_string);
string PercentagesEnumTypeToString(RandomNumsConfig type);

} // namespace duckdb