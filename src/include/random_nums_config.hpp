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
};

unordered_map<RandomPercentagesEnum, idx_t> GetDefaultConfig();
unordered_map<RandomPercentagesEnum, idx_t> GetConfigFromFile(const char *json_string);
string RandomPercentagesEnumToString(RandomPercentagesEnum type);

} // namespace duckdb