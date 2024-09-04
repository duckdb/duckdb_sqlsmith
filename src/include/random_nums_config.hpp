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
private:

    idx_t select_percentage = 60;
	idx_t attach_percentage = 40;
	idx_t attach_use_percentage = 50;
	idx_t detach_percentage = 60;
	idx_t set_percentage = 30;
	idx_t delete_percentage = 40;

public:
    RandomNumsConfig() {

    };
	~RandomNumsConfig();

	// read file and update default values
	void getConfigFromFile(duckdb_string_t file_path) {

	}

	// set default values
	void GetDefaultConfig() {
		
	}

};

} // namespace duckdb