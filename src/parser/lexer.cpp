#include "parser/lexer.h"
#include "parser/token.h"
#include <algorithm>
#include <cctype>
#include <cstddef>
#include <string>
#include <unordered_map>

Lexer::Lexer(const std::string& sql) : input(sql), position(0) {}

std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;
    
    while (position < input.length()) {
        char current = input[position];
        
        if (std::isspace(current) != 0) {
            position++;
            continue;
        }
        
        if (std::isdigit(current) != 0) {
            tokens.push_back(readNumber());
            continue;
        }
        
        if ((std::isalpha(current) != 0) || current == '_') {
            tokens.push_back(readIdentifierOrKeyword());
            continue;
        }
        
        if (current == '\'') {
            tokens.push_back(readString());
            continue;
        }
        
        tokens.push_back(readOperator());
    }
    
    tokens.emplace_back(TokenType::END_OF_FILE, "", position);
    return tokens;
}

Token Lexer::readNumber() {
    size_t start = position;
    while (position < input.length() && 
           ((std::isdigit(input[position]) != 0) || input[position] == '.')) {
        position++;
    }
    return Token(TokenType::NUMBER, input.substr(start, position - start), start);
}

Token Lexer::readIdentifierOrKeyword() {
    size_t start = position;
    while (position < input.length() && 
           ((std::isalnum(input[position]) != 0) || input[position] == '_')) {
        position++;
    }
    std::string value = input.substr(start, position - start);
    TokenType type = getKeywordType(value);
    return Token(type, value, start);
}

Token Lexer::readString() {
    size_t start = position;
    position++;
    std::string value;
    while (position < input.length() && input[position] != '\'') {
        if (input[position] == '\\' && position + 1 < input.length()) {
            position++;
        }
        value += input[position++];
    }
    if (position < input.length()) { 
        position++;
    }
    return Token(TokenType::STRING, value, start);
}

Token Lexer::readOperator() {
    size_t start = position;
    char current = input[position++];
    
    switch (current) {
        case '=': return Token(TokenType::EQUALS, "=", start);
        case ',': return Token(TokenType::COMMA, ",", start);
        case ';': return Token(TokenType::SEMICOLON, ";", start);
        case '(': return Token(TokenType::LEFT_PAREN, "(", start);
        case ')': return Token(TokenType::RIGHT_PAREN, ")", start);
        case '*': return Token(TokenType::STAR, "*", start);
        case '+': return Token(TokenType::PLUS, "+", start);
        case '-': return Token(TokenType::MINUS, "-", start);
        case '/': return Token(TokenType::SLASH, "/", start);
        case '<':
            if (position < input.length() && input[position] == '=') {
                position++;
                return Token(TokenType::LESS_EQUAL, "<=", start);
            }
            return Token(TokenType::LESS_THAN, "<", start);
        case '>':
            if (position < input.length() && input[position] == '=') {
                position++;
                return Token(TokenType::GREATER_EQUAL, ">=", start);
            }
            return Token(TokenType::GREATER_THAN, ">", start);
        case '!':
            if (position < input.length() && input[position] == '=') {
                position++;
                return Token(TokenType::NOT_EQUALS, "!=", start);
            }
        default:
            return Token(TokenType::INVALID, std::string(1, current), start);
    }
}

TokenType Lexer::getKeywordType(const std::string& word) {
    std::string upper = word;
    std::transform(upper.begin(), upper.end(), upper.begin(), ::toupper);
    
    static const std::unordered_map<std::string, TokenType> keywords = {
        {"SELECT", TokenType::SELECT}, {"FROM", TokenType::FROM},
        {"WHERE", TokenType::WHERE}, {"INSERT", TokenType::INSERT},
        {"INTO", TokenType::INTO}, {"VALUES", TokenType::VALUES},
        {"CREATE", TokenType::CREATE}, {"TABLE", TokenType::TABLE},
        {"DELETE", TokenType::DELETE}, {"UPDATE", TokenType::UPDATE},
        {"SET", TokenType::SET}, {"AND", TokenType::AND},
        {"OR", TokenType::OR}, {"NOT", TokenType::NOT}
    };
    
    auto it = keywords.find(upper);
    return (it != keywords.end()) ? it->second : TokenType::IDENTIFIER;
}