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
    { "select_node_perc", RandomPercentagesEnum::SELECT_NODE },
    { "select_node_is_distinct_perc", RandomPercentagesEnum::SELECT_NODE_IS_DISTINCT },
    { "select_node_from_table_perc", RandomPercentagesEnum::SELECT_NODE_FROM_TABLE },
    { "select_node_where_perc", RandomPercentagesEnum::SELECT_NODE_WHERE },
    { "select_node_having_perc", RandomPercentagesEnum::SELECT_NODE_HAVING },
    { "select_node_groups_perc", RandomPercentagesEnum::SELECT_NODE_GROUPS },
    { "select_node_group_by_perc", RandomPercentagesEnum::SELECT_NODE_GROUP_BY },
    { "select_node_qualify_perc", RandomPercentagesEnum::SELECT_NODE_QUALIFY },
    { "select_node_aggregate_perc", RandomPercentagesEnum::SELECT_NODE_AGGREGATE },
    { "select_node_sample_perc", RandomPercentagesEnum::SELECT_NODE_SAMPLE },
    { "select_node_sample_is_perc", RandomPercentagesEnum::SELECT_NODE_SAMPLE_IS_PERC },
    { "select_node_sample_size", RandomPercentagesEnum::SELECT_NODE_SAMPLE_SIZE },
    { "result_modifiers", RandomPercentagesEnum::RESULT_MODIFIERS },
    { "limit_percent_modifier", RandomPercentagesEnum::LIMIT_PERCENT_MODIFIER },
    { "limit_percent_modifier_limit", RandomPercentagesEnum::LIMIT_PERCENT_MODIFIER_LIMIT },
    { "limit_percent_modifier_offset", RandomPercentagesEnum::LIMIT_PERCENT_MODIFIER_OFFSET },
    { "limit_modifier_limit", RandomPercentagesEnum::LIMIT_MODIFIER_LIMIT },
    { "limit_modifier_offset", RandomPercentagesEnum::LIMIT_MODIFIER_OFFSET }
};

enum Statements {
    select = 0,
    attach,
    delete_st,
    set,

};


unordered_map<RandomPercentagesEnum, idx_t> GetConfigFromFile(const char *json_string) {
    unordered_map<RandomPercentagesEnum, idx_t> config_from_file;
    auto doc = yyjson_read_file(json_string, YYJSON_READ_NOFLAG, NULL, NULL);
    if (doc) {
        yyjson_val *obj = yyjson_doc_get_root(doc);
        yyjson_obj_iter iter;
        yyjson_obj_iter_init(obj, &iter);
        size_t idx, max;
        yyjson_val *key, *val;
        yyjson_obj_foreach(obj, idx, max, key, val) {
            const char* root_key = yyjson_get_str(key);
            auto it = StringToRandomPercentagesEnum.find(root_key); // "select" or "attach"
            if (it != StringToRandomPercentagesEnum.end()) {
                RandomPercentagesEnum perc_type = it->second; // SELECT
                auto perc_value = yyjson_get_str(val);
                config_from_file[perc_type] = std::stoi(perc_value); // { SELECT: 90 }
            } 
            if (yyjson_is_obj(val)) {
                size_t node_idx, node_max;
                yyjson_val *node_key, *node_val;
                yyjson_obj_foreach(val, node_idx, node_max, node_key, node_val) {
                    const char* node_root_key = yyjson_get_str(node_key); // sub roots are "select_node" and "select_node_sample"
                    auto node_it = StringToRandomPercentagesEnum.find(node_root_key);
                    if (node_it != StringToRandomPercentagesEnum.end()) {
                        RandomPercentagesEnum node_perc_type = node_it->second;
                        auto node_perc_value = yyjson_get_str(node_val);
                        config_from_file[node_perc_type] = std::stoi(node_perc_value);
                    }
                    if (yyjson_is_obj(node_val)) {
                        size_t sub_idx, sub_max;
                        yyjson_val *sub_key, *sub_val;
                        yyjson_obj_foreach(node_val, sub_idx, sub_max, sub_key, sub_val) {
                            const char* sub_root_key = yyjson_get_str(sub_key);
                            auto sub_it = StringToRandomPercentagesEnum.find(sub_root_key);
                            if (sub_it != StringToRandomPercentagesEnum.end()) {
                                RandomPercentagesEnum sub_perc_type = sub_it->second;
                                auto sub_perc_value = yyjson_get_str(sub_val);
                                config_from_file[sub_perc_type] = std::stoi(sub_perc_value);
                            }
                            if (yyjson_is_obj(sub_val)) {
                                size_t subnode_idx, subnode_max;
                                yyjson_val *subnode_key, *subnode_val;
                                yyjson_obj_foreach(sub_val, subnode_idx, subnode_max, subnode_key, subnode_val) {
                                    const char* subnode_root_key = yyjson_get_str(subnode_key);
                                    auto subnode_it = StringToRandomPercentagesEnum.find(subnode_root_key);
                                    if (subnode_it != StringToRandomPercentagesEnum.end()) {
                                        RandomPercentagesEnum subnode_perc_type = subnode_it->second;
                                        auto subnode_perc_value = yyjson_get_str(subnode_val);
                                        config_from_file[subnode_perc_type] = std::stoi(subnode_perc_value);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    } else {
        // Couldn't read JSON with percentages config
        yyjson_doc_free(doc);
        return GetDefaultConfig();
    }
        // Free the doc
    yyjson_doc_free(doc);
    return config_from_file;
}
} // namespace duckdb