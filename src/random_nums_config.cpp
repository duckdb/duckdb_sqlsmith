#include "random_nums_config.hpp"

namespace duckdb {
	
RandomNumsConfig::RandomNumsConfig() {
    
}

RandomNumsConfig::~RandomNumsConfig() {
}


void RandomNumsConfig::GetConfigFromFile(duckdb_string_t file_path) {

}

// set default values
void RandomNumsConfig::GetDefaultConfig() {
    idx_t select_percentage = 60;
    idx_t attach_percentage = 40;
    idx_t attach_use_percentage = 50;
    idx_t detach_percentage = 60;
    idx_t set_percentage = 30;
    idx_t delete_percentage = 40;
}
}