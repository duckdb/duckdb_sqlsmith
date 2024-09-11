#include "include/random_nums_config.hpp"
#include "duckdb/common/string_util.hpp"
#include "/Users/zuleykhapavlichenkova/Desktop/duckdb_sqlsmith/duckdb/third_party/yyjson/include/yyjson.hpp"
#include "duckdb/common/local_file_system.hpp"

#include <iostream>
#include <fstream>

namespace duckdb {

using namespace duckdb_yyjson;

RandomNumsConfig::RandomNumsConfig() {
    GetDefaultConfig();
}

RandomNumsConfig::RandomNumsConfig(string &file_path) {
    GetDefaultConfig();
    GetConfigFromFile(file_path);
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

void RandomNumsConfig::GetConfigFromFile(string &file_path) {
    // select_percentage = csv_parser(file_path, "select_percentage");
    // attach_percentage = csv_parser(file_path, "attach_percentage");
    // attach_use_percentage =csv_parser(file_path, "attach_use_percentage");
    // detach_percentage =csv_parser(file_path, "detach_percentage");
    // set_percentage = csv_parser(file_path, "set_percentage");
    // delete_percentage = csv_parser(file_path, "delete_percentage");

    // open and read file into a string &
    string result;
    std::ifstream open_file(file_path);
    if (!open_file.is_open()) {
        std::cerr << "Could not open the file!" << std::endl;
    }
    std::string line;
    while (std::getline(open_file, line)) {
        if (open_file.fail()) {
            std::cerr << "Error reading file!" << std::endl;
        }
        result.append(line);
    }
    unordered_map<string, string> json;

    try {
        json = StringUtil::ParseJSONMap(result);
    } catch (std::exception &ex) {
        throw IOException("Couldn't parse JSON file with percentages config.");
    }

    open_file.close();

    std::cout << json[0] << std::endl;
    // LocalFileSystem fs;
    // string open_file;
    // try {
    //     auto handle = fs.OpenFile(file_path, FileFlags::FILE_FLAGS_READ);
    //     fs.Read(handle, buffer, n_bites, location);
         
        

    // } catch (std::exception &ex) {
    //     throw IOException("Failed to open config file with provided path '%s.", file_path);
    // }

    //read
    


    // check the JSON format
    // if (open_file[0] != '{') {
    //     // throw an error that this is not a JSON
    //     throw IOException("Not JSON file provided as the config.");
    // }
    // if (!strstr(open_file, "percentage_types")) {
    //     throw IOException("percentage_types are not provided in the config file.");
    // }
    
    // yyjson_doc *doc = yyjson_read(open_file, strlen(open_file), 0);
    // if (!doc) {
    //     throw IOException("Failed to read config file with provided path '%s.", file_path);
    // }

    // yyjson_val *root = yyjson_doc_get_root(doc);
    // if (!yyjson_is_obj(root)) {
    //     throw IOException("Not JSON object provided as the config.");
    //     yyjson_doc_free(doc);
    // }

    // parse json to map
    // unordered_map<string, string> json;

    // try {
    //     json = StringUtil::ParseJSONMap(open_file);
    // } catch (std::exception &ex) {
    //     throw IOException("Couldn't parse JSON file with percentages config.");
    // }

}

    // test_yyjson/config.json



}