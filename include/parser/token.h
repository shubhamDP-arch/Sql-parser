#ifndef TOKEN_H
#define TOKEN_H
#include <string>
#include <cstddef>

enum class TokenType {
    // Keywords
    SELECT, FROM, WHERE, INSERT, INTO, VALUES,
    CREATE, TABLE, DELETE, UPDATE, SET,
    AND, OR, NOT,
    
    // Literals
    NUMBER,        // 123, 45.67
    STRING,        // 'hello'
    IDENTIFIER,    // table_name, column_name
    
    // Operators
    EQUALS,        // =
    NOT_EQUALS,    // !=
    LESS_THAN,     // 
    GREATER_THAN,  // >
    LESS_EQUAL,    // <=
    GREATER_EQUAL, // >=
    PLUS,          // +
    MINUS,         // -
    STAR,          // *
    SLASH,         // /
    
    // Punctuation
    COMMA,         // ,
    SEMICOLON,     // ;
    LEFT_PAREN,    // (
    RIGHT_PAREN,   // )
    
    // Special
    END_OF_FILE,
    INVALID
};

struct Token {
    TokenType type;
    std::string value;
    size_t position;

    Token(TokenType t, std::string v, size_t pos)
        : type(t), value(std::move(v)), position(pos) {}
};

#endif  