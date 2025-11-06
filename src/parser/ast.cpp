#include "parser/ast.h"
#include <cstddef>
#include <memory>
#include <string>
#include <utility>

// ColumnExpression
ColumnExpression::ColumnExpression(std::string name)
    : column_name(std::move(name)) {}

std::string ColumnExpression::toString() const {
    return "Column(" + column_name + ")";
}

// LiteralExpression
LiteralExpression::LiteralExpression(std::string val, Type t)
    : value(std::move(val)), type(t) {}

std::string LiteralExpression::toString() const {
    return type == Type::STRING ? "'" + value + "'" : value;
}

// BinaryExpression
BinaryExpression::BinaryExpression(std::unique_ptr<Expression> l,
                                   std::unique_ptr<Expression> r,
                                   Operator o)
    : left(std::move(l)), right(std::move(r)), op(o) {}

std::string BinaryExpression::toString() const {
    return "(" + left->toString() + " " + 
           operatorToString(op) + " " + 
           right->toString() + ")";
}

std::string BinaryExpression::operatorToString(Operator op) const {
    switch (op) {
        case Operator::EQUALS: return "=";
        case Operator::NOT_EQUALS: return "!=";
        case Operator::LESS_THAN: return "<";
        case Operator::GREATER_THAN: return ">";
        case Operator::LESS_EQUAL: return "<=";
        case Operator::GREATER_EQUAL: return ">=";
        case Operator::AND: return "AND";
        case Operator::OR: return "OR";
        case Operator::PLUS: return "+";
        case Operator::MINUS: return "-";
        case Operator::MULTIPLY: return "*";
        case Operator::DIVIDE: return "/";
    }
    return "?";
}

// SelectStatement
std::string SelectStatement::toString() const {
    std::string result = "SELECT ";
    for (size_t i = 0; i < columns.size(); ++i) {
        if (i > 0) result += ", ";
        result += columns[i]->toString();
    }
    result += " FROM " + table_name;
    if (where_clause) {
        result += " WHERE " + where_clause->toString();
    }
    return result;
}

// InsertStatement
std::string InsertStatement::toString() const {
    std::string result = "INSERT INTO " + table_name;
    if (!columns.empty()) {
        result += " (";
        for (size_t i = 0; i < columns.size(); ++i) {
            if (i > 0) result += ", ";
            result += columns[i];
        }
        result += ")";
    }
    result += " VALUES (";
    for (size_t i = 0; i < values.size(); ++i) {
        if (i > 0) result += ", ";
        result += values[i]->toString();
    }
    result += ")";
    return result;
}