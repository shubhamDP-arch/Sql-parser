#ifndef LEXER_H
#define LEXER_H

#include "token.h"
#include <vector>
#include <string>

class Lexer {
private:
    std::string input;
    size_t position;
    
    Token readNumber();
    Token readIdentifierOrKeyword();
    Token readString();
    Token readOperator();
    TokenType getKeywordType(const std::string& word);
    
public:
    explicit Lexer(const std::string& sql);
    std::vector<Token> tokenize();
};

#endif 