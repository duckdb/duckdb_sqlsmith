#include "include/random_nums_config.hpp"

#include "duckdb/common/string_util.hpp"
#include "duckdb/common/local_file_system.hpp"

#include <iostream>
#include <fstream>

namespace duckdb {

using namespace duckdb_yyjson;

RandomNumsConfig::RandomNumsConfig() {
    GetDefaultConfig();
}

RandomNumsConfig::RandomNumsConfig(string &json_string) {
    GetDefaultConfig();
    GetConfigFromFile(json_string);
}

RandomNumsConfig::~RandomNumsConfig() {
}

void RandomNumsConfig::GetDefaultConfig() {
    select_percentage = 60;
    attach_percentage = 40;
    attach_use_percentage = 50;
    detach_percentage = 60;
    set_percentage = 30;
    delete_percentage = 40;
}

void RandomNumsConfig::GetConfigFromFile(string &json_string) {
    // open and read file into a string &
    // string result;
    // std::ifstream open_file(file_path);
    // if (!open_file.is_open()) {
    //     std::cerr << "Could not open the file!" << std::endl;
    // }
    // std::string line;
    // while (std::getline(open_file, line)) {
    //     if (open_file.fail()) {
    //         std::cerr << "Error reading file!" << std::endl;
    //     }
    //     result.append(line);
    // }

    unordered_map<string, string> json;
    try {
        json = StringUtil::ParseJSONMap(json_string);
    } catch (std::exception &ex) {
        throw IOException("Couldn't parse JSON string containing percentages config.");
    }

    select_percentage = stoi(json.find("select_percentage")->second);
    attach_percentage = stoi(json.find("attach_percentage")->second);
    attach_use_percentage = stoi(json.find("attach_use_percentage")->second);
    detach_percentage = stoi(json.find("detach_percentage")->second);
    set_percentage = stoi(json.find("set_percentage")->second);
    delete_percentage = stoi(json.find("delete_percentage")->second);

}
} // namespace duckdb