// ===----------------------------------------------------------------------===//
//                         DuckDB

// random_nums_config.hpp


// ===----------------------------------------------------------------------===//

#pragma once

#include "duckdb.hpp"
#include "yyjson.hpp"

namespace duckdb {

enum class RandomPercentagesEnum  : idx_t {
    // ----------------------------------
	// Generate Attach Percentages
	// ----------------------------------
    ATTACH = 0,
    ATTACH_USE = 1,
    ATTACH_READ_ONLY = 2,
    // ----------------------------------
	// Generate Detach Percentages
	// ----------------------------------
    DETACH = 3,
    DETACH_RANDOM_NAME = 4,
    // ----------------------------------
	// Generate Set Percentages
	// ----------------------------------
    SET = 5,
    SET_ATTACHED_DB = 6,
    // ----------------------------------
	// Generate Select Percentages
	// ----------------------------------
    DELETE = 7,

    // ----------------------------------
	// Generate Select Percentages
	// ----------------------------------
	SELECT = 8,
    SELECT_NODE = 9,
    SELECT_NODE_IS_DISTINCT = 10,
    SELECT_NODE_FROM_TABLE = 11,
    SELECT_NODE_WHERE = 12,
    SELECT_NODE_HAVING = 13,
    SELECT_NODE_GROUPS = 14,
    SELECT_NODE_GROUP_BY = 15,
    SELECT_NODE_QUALIFY = 16,
    SELECT_NODE_AGGREGATE = 17,
    SELECT_NODE_SAMPLE = 18,
    SELECT_NODE_SAMPLE_IS_PERC = 19,
    SELECT_NODE_SAMPLE_SIZE = 20,
    RESULT_MODIFIERS = 21,
    LIMIT_PERCENT_MODIFIER = 22,
    LIMIT_PERCENT_MODIFIER_LIMIT = 23,
    LIMIT_PERCENT_MODIFIER_OFFSET = 24,
    LIMIT_MODIFIER_LIMIT = 25,
    LIMIT_MODIFIER_OFFSET = 26,
    
    // ----------------------------------
	// Generate Create Percentages
	// ----------------------------------
    CREATE = 27,
    CREATE_GENERATE_SELECT = 28,
    CREATE_NUM_COLS = 29,
    // -----------------------------------
	// Generate Table Ref Percentages
	// -----------------------------------
    TABLE_REF_BASE_TABLE_REF_PERC = 30,
    TABLE_REF_EXPRESSION_LIST_REF = 31,
    TABLE_REF_JOIN_REF = 32,

    // -----------------------------------
	// Generate Join Ref Percentages
	// -----------------------------------
    JOIN_REF_CROSS = 33,
    JOIN_REF_ASOF = 34,
    JOIN_REF_NATURAL = 35,
    JOIN_REF_POSITIONAL = 36,
    JOIN_REF_GENERAL_EXPRESSION = 37,
    
    // -----------------------------------
	// Generate Expression Percentages
	// -----------------------------------
    EXPRESSION_COLUMN_REF = 38,
    EXPRESSION_CONSTANT = 39,
    EXPRESSION_SUBQUERY = 40,
    
    // -----------------------------------
	// Generate Constant Value Percentages
	// -----------------------------------
    CONSTANT_VALUE_BIGINT = 41,
    CONSTANT_VALUE_TO_STRING = 42,

    // -----------------------------------
	// Generate Function Percentages
	// -----------------------------------    
    FUNCTION_AGGREGATE_WINDOW_FUNCTION = 43,
    FUNCTION_AGGREGATE_ORDER_BY = 44,
    FUNCTION_AGGREGATE_DISTINCT = 45,
    FUNCTION_AGGREGATE_RANDOM_EXPRESSION = 46,

    // -----------------------------------
	// Generate Window Function Percentages
	// -----------------------------------    
    WINDOW_FUNCTION_PARTITIONS = 47,
    WINDOW_FUNCTION_ORDERS = 48,
    WINDOW_FUNCTION_IGNORE_NULLS = 49,
    WINDOW_FUNCTION_RESULT_OFFSET = 50,
    WINDOW_FUNCTION_RESULT_DEFAULT = 51,
    
    // -----------------------------------
	// Generate Star Percentages
	// -----------------------------------
    STAR_RELATION_NAME = 52,
    STAR_COLUMN_NAME = 53,
    STAR_COLUMN_NAME_EXCLUDE_LIST = 54,
    STAR_COLUMNS_TRUE = 55,
    STAR_COLUMNS_TRUE_LAMBDA = 56,
    
    RELATIONAL_NAME_CHOOSE_CURRENT = 57,
    COLUMN_NAMES_CHOOSE_CURRENT = 58,
        
    COUNT

};

unordered_map<RandomPercentagesEnum, idx_t> GetDefaultConfig();
unordered_map<RandomPercentagesEnum, idx_t> GetConfigFromFile(const char *json_string);
string RandomPercentagesEnumToString(RandomPercentagesEnum type);

} // namespace duckdb