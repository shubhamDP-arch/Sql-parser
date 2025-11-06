#include "parser/parser.h"
#include "parser/ast.h"
#include "parser/token.h"
#include <memory>
#include <stdexcept>
#include <vector>
#include <utility>

Parser::Parser(std::vector<Token> toks)
    : tokens(std::move(toks)), current(0) {}

std::unique_ptr<Statement> Parser::parse() {
    if (match(TokenType::SELECT)) {
        return parseSelect();
    } if (match(TokenType::INSERT)) {
        return parseInsert();
    }
    throw std::runtime_error("Expected SELECT or INSERT statement");
}

std::unique_ptr<SelectStatement> Parser::parseSelect(){
    auto stmt = std::make_unique<SelectStatement>();
    stmt->columns = parseColumnList();
    if (!match(TokenType::FROM)) {
        throw std::runtime_error("Expected FROM keyword");
    }
    
    if (peek().type != TokenType::IDENTIFIER) {
        throw std::runtime_error("Expected table name");
    }
    stmt->table_name = advance().value;
    
    if (match(TokenType::WHERE)) {
        stmt->where_clause = parseExpression();
    }
    
    return stmt;

}

std::vector<std::unique_ptr<Expression>> Parser::parseColumnList() {
    std::vector<std::unique_ptr<Expression>> columns;
    do {
        if (peek().type == TokenType::STAR) {
            advance();
            columns.push_back(std::make_unique<ColumnExpression>("*"));
        } else if (peek().type == TokenType::IDENTIFIER) {
            columns.push_back(std::make_unique<ColumnExpression>(advance().value));
        } else {
            throw std::runtime_error("Expected column name or *");
        }
    } while (match(TokenType::COMMA));
    return columns;
}

std::unique_ptr<Expression> Parser::parseExpression(){
    return parseOr();
}

std::unique_ptr<Expression> Parser::parseOr() {
    auto left = parseAnd();
    while (match(TokenType::OR)) {
        auto right = parseAnd();
        left = std::make_unique<BinaryExpression>(
            std::move(left), std::move(right), BinaryExpression::Operator::OR
        );
    }
    return left;
}

std::unique_ptr<Expression> Parser::parseAnd() {
    auto left = parseComparison();
    while (match(TokenType::AND)) {
        auto right = parseComparison();
        left = std::make_unique<BinaryExpression>(
            std::move(left), std::move(right), BinaryExpression::Operator::AND
        );
    }
    return left;
}


std::unique_ptr<Expression> Parser::parseComparison() {
    auto left = parsePrimary();
    
    BinaryExpression::Operator op;
    if (match(TokenType::EQUALS)) op = BinaryExpression::Operator::EQUALS;
    else if (match(TokenType::NOT_EQUALS)) op = BinaryExpression::Operator::NOT_EQUALS;
    else if (match(TokenType::LESS_THAN)) op = BinaryExpression::Operator::LESS_THAN;
    else if (match(TokenType::GREATER_THAN)) op = BinaryExpression::Operator::GREATER_THAN;
    else if (match(TokenType::LESS_EQUAL)) op = BinaryExpression::Operator::LESS_EQUAL;
    else if (match(TokenType::GREATER_EQUAL)) op = BinaryExpression::Operator::GREATER_EQUAL;
    else return left;
    
    auto right = parsePrimary();
    return std::make_unique<BinaryExpression>(std::move(left), std::move(right), op);
}

std::unique_ptr<Expression> Parser::parsePrimary() {
    if (peek().type == TokenType::NUMBER) {
        return std::make_unique<LiteralExpression>(
            advance().value, LiteralExpression::Type::NUMBER
        );
    }
    if (peek().type == TokenType::STRING) {
        return std::make_unique<LiteralExpression>(
            advance().value, LiteralExpression::Type::STRING
        );
    }
    if (peek().type == TokenType::IDENTIFIER) {
        return std::make_unique<ColumnExpression>(advance().value);
    }
    if (match(TokenType::LEFT_PAREN)) {
        auto expr = parseExpression();
        if (!match(TokenType::RIGHT_PAREN)) {
            throw std::runtime_error("Expected closing parenthesis");
        }
        return expr;
    }
    throw std::runtime_error("Expected expression");
}

std::unique_ptr<InsertStatement> Parser::parseInsert() {
    auto stmt = std::make_unique<InsertStatement>();
    
    if (!match(TokenType::INTO)) {
        throw std::runtime_error("Expected INTO after INSERT");
    }
    if (peek().type != TokenType::IDENTIFIER) {
        throw std::runtime_error("Expected table name");
    }
    stmt->table_name = advance().value;
    
    if (match(TokenType::LEFT_PAREN)) {
        do {
            if (peek().type != TokenType::IDENTIFIER) {
                throw std::runtime_error("Expected column name");
            }
            stmt->columns.push_back(advance().value);
        } while (match(TokenType::COMMA));
        
        if (!match(TokenType::RIGHT_PAREN)) {
            throw std::runtime_error("Expected closing parenthesis");
        }
    }
    
    if (!match(TokenType::VALUES)) {
        throw std::runtime_error("Expected VALUES");
    }
    if (!match(TokenType::LEFT_PAREN)) {
        throw std::runtime_error("Expected opening parenthesis");
    }
    
    do {
        stmt->values.push_back(parsePrimary());
    } while (match(TokenType::COMMA));
    
    if (!match(TokenType::RIGHT_PAREN)) {
        throw std::runtime_error("Expected closing parenthesis");
    }
    
    return stmt;
}



Token Parser::peek() const {
    return tokens[current];
}

Token Parser::advance() {
    if (current < tokens.size()) {
        return tokens[current++];
    }
    return tokens.back();
}


bool Parser::match(TokenType type) {
    if (peek().type == type) {
        advance();
        return true;
    }
    return false;
}
bool Parser::check(TokenType type) const {
    return peek().type == type;
}