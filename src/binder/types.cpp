#include "binder/types.h"
#include <string>

std::string dataTypeToString(DataType type) {
    switch (type) {
        case DataType::INTEGER: return "INTEGER";
        case DataType::FLOAT: return "FLOAT";
        case DataType::VARCHAR: return "VARCHAR";
        case DataType::BOOLEAN: return "BOOLEAN";
        case DataType::DATE: return "DATE";
        case DataType::UNKNOWN: return "UNKNOWN";
    }
    return "UNKNOWN";
}

bool isNumericType(DataType type) {
    return type == DataType::INTEGER || type == DataType::FLOAT;
}

bool areTypesCompatible(DataType left, DataType right) {
    if (left == right) {
        return true;
    }
    
    if (isNumericType(left) && isNumericType(right)) {
        return true;
    }
    
    return false;
}


DataType promoteNumericTypes(DataType left, DataType right) {
    if (left == DataType::FLOAT || right == DataType::FLOAT) {
        return DataType::FLOAT;
    }

    if (left == DataType::INTEGER && right == DataType::INTEGER) {
        return DataType::INTEGER;
    }
    
    return DataType::UNKNOWN;
}