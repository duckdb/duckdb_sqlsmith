//===----------------------------------------------------------------------===//
//                         DuckDB
//
// random_nums_config.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "duckdb.hpp"

namespace duckdb {

class RandomNumsConfig {
public:

	idx_t select_percentage;
	idx_t attach_percentage;
	idx_t attach_use_percentage;
	idx_t detach_percentage;
	idx_t set_percentage;
	idx_t delete_percentage;

    RandomNumsConfig();
	~RandomNumsConfig();

	// is it better to return an object initialised with fetched/default params?
	// read file and update default values
	void GetConfigFromFile(duckdb_string_t file_path);
	// set default values
	void GetDefaultConfig();

};

} // namespace duckdb