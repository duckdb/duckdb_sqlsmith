#include "include/random_nums_config.hpp"

#include <algorithm>
#include <iterator>
#include <ctype.h>

namespace duckdb {

using namespace duckdb_yyjson;

unordered_map<RandomPercentagesEnum, idx_t> GetDefaultConfig() {
    unordered_map<RandomPercentagesEnum, idx_t> default_config = {
        { RandomPercentagesEnum::ATTACH, 40 },
        { RandomPercentagesEnum::ATTACH_USE, 50 },
        { RandomPercentagesEnum::DELETE, 40 },
        { RandomPercentagesEnum::DETACH, 60 },
        { RandomPercentagesEnum::SELECT, 60 },
        { RandomPercentagesEnum::SET, 30 }
    };
    return default_config;
}

unordered_map<string, RandomPercentagesEnum> StringToRandomPercentagesEnum = {
    { "attach_percentage", RandomPercentagesEnum::ATTACH },
    { "attach_use_percentage", RandomPercentagesEnum::ATTACH_USE },
    { "delete_percentage", RandomPercentagesEnum::DELETE },
    { "detach_percentage", RandomPercentagesEnum::DETACH },
    { "select_percentage", RandomPercentagesEnum::SELECT },
    { "set_percentage", RandomPercentagesEnum::SET }
};


unordered_map<RandomPercentagesEnum, idx_t> GetConfigFromFile(const char *json_string) {
    unordered_map<RandomPercentagesEnum, idx_t> config_from_file;
    auto doc = yyjson_read_file(json_string, YYJSON_READ_NOFLAG, NULL, NULL);
    if (doc) {
        yyjson_val *obj = yyjson_doc_get_root(doc);
        yyjson_obj_iter iter;
        yyjson_obj_iter_init(obj, &iter);
        yyjson_val *key, *val;
        while ((key = yyjson_obj_iter_next(&iter))) {
            const char* k = yyjson_get_str(key);
            val = yyjson_obj_iter_get_val(key);
            auto it = StringToRandomPercentagesEnum.find(k);
            if (it != StringToRandomPercentagesEnum.end()) {
                RandomPercentagesEnum perc_type = it->second;
                auto perc_value = yyjson_get_str(val);
                config_from_file[perc_type] = std::stoi(perc_value);
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