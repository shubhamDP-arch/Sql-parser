#ifndef AST_H
#define AST_H

#include <string>
#include <vector>
#include <memory>

class AST_NODE{
  public:
    virtual ~AST_NODE() = default;
    [[nodiscard]] virtual std::string toString() const = 0;
};

class Expression: public AST_NODE {};

class ColumnExpression : public Expression{
  public:
    std::string column_name;
    explicit ColumnExpression(std::string name);
    std::string toString() const override;
};

class LiteralExpression : public Expression {
public:
    enum class Type { NUMBER, STRING };
    std::string value;
    Type type;
    
    LiteralExpression(std::string val, Type t);
    std::string toString() const override;
};

class BinaryExpression : public Expression {
public:
    enum class Operator {
        EQUALS, NOT_EQUALS, LESS_THAN, GREATER_THAN,
        LESS_EQUAL, GREATER_EQUAL, AND, OR,
        PLUS, MINUS, MULTIPLY, DIVIDE
    };
    
    std::unique_ptr<Expression> left;
    std::unique_ptr<Expression> right;
    Operator op;
    
    BinaryExpression(std::unique_ptr<Expression> l,
                     std::unique_ptr<Expression> r,
                     Operator o);
    std::string toString() const override;
    
private:
    std::string operatorToString(Operator op) const;
};

class Statement : public AST_NODE {};

class SelectStatement : public Statement {
public:
    std::vector<std::unique_ptr<Expression>> columns;
    std::string table_name;
    std::unique_ptr<Expression> where_clause;
    
    std::string toString() const override;
};

class InsertStatement : public Statement {
public:
    std::string table_name;
    std::vector<std::string> columns;
    std::vector<std::unique_ptr<Expression>> values;
    
    std::string toString() const override;
};

#endif
