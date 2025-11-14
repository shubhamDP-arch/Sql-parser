#ifndef TYPES_H
#define TYPES_H

#include <string>

enum class DataType {
    INTEGER,
    FLOAT,
    VARCHAR,
    BOOLEAN,
    DATE,
    UNKNOWN
};

std::string dataTypeToString(DataType type);

bool isNumericType(DataType type);

bool areTypesCompatible(DataType left, DataType right);

DataType promoteNumericTypes(DataType left, DataType right);

#endif