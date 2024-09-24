#include "include/random_nums_config.hpp"

#include <algorithm>
#include <iterator>
#include <ctype.h>

namespace duckdb {

using namespace duckdb_yyjson;

unordered_map<RandomNumsConfig, idx_t> GetDefaultConfig() {
    unordered_map<RandomNumsConfig, idx_t> default_config = {
        { RandomNumsConfig::ATTACH, 40 },
        { RandomNumsConfig::ATTACH_USE, 50 },
        { RandomNumsConfig::DELETE, 40 },
        { RandomNumsConfig::DETACH, 60 },
        { RandomNumsConfig::SELECT, 60 },
        { RandomNumsConfig::SET, 30 }
    };
    return default_config;
}

unordered_map<string, RandomNumsConfig> StringToRandomNumsConfig = {
    { "attach_percentage", RandomNumsConfig::ATTACH },
    { "attach_use_percentage", RandomNumsConfig::ATTACH_USE },
    { "delete_percentage", RandomNumsConfig::DELETE },
    { "detach_percentage", RandomNumsConfig::DETACH },
    { "select_percentage", RandomNumsConfig::SELECT },
    { "set_percentage", RandomNumsConfig::SET }
};


unordered_map<RandomNumsConfig, idx_t> GetConfigFromFile(const char *json_string) {
    unordered_map<RandomNumsConfig, idx_t> config_from_file;
    auto doc = yyjson_read_file(json_string, YYJSON_READ_NOFLAG, NULL, NULL);
    if (doc) {
        yyjson_val *obj = yyjson_doc_get_root(doc);
        yyjson_obj_iter iter;
        yyjson_obj_iter_init(obj, &iter);
        yyjson_val *key, *val;
        while ((key = yyjson_obj_iter_next(&iter))) {
            const char* k = yyjson_get_str(key);
            val = yyjson_obj_iter_get_val(key);
            auto it = StringToRandomNumsConfig.find(k);
            if (it != StringToRandomNumsConfig.end()) {
                RandomNumsConfig perc_type = it->second;
                idx_t perc_value = yyjson_get_int(val);
                config_from_file[perc_type] = perc_value;
            }
        }
    } else {
        throw IOException("Couldn't read JSON with percentages config.");
        yyjson_doc_free(doc);
        return GetDefaultConfig();
    }
        // Free the doc
    yyjson_doc_free(doc);
    return config_from_file;
}
} // namespace duckdb