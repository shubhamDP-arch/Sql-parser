#ifndef PARSER_H
#define PARSER_H

#include "token.h"
#include "ast.h"
#include <vector>
#include <memory>

class Parser {
private:
    std::vector<Token> tokens;
    size_t current;

    std::unique_ptr<SelectStatement> parseSelect();
    std::unique_ptr<InsertStatement> parseInsert();

    std::vector<std::unique_ptr<Expression>> parseColumnList();
    std::unique_ptr<Expression> parseExpression();
    std::unique_ptr<Expression> parseOr();
    std::unique_ptr<Expression> parseAnd();
    std::unique_ptr<Expression> parseComparison();
    std::unique_ptr<Expression> parsePrimary();
    
    Token peek() const;
    Token advance();
    bool match(TokenType type);
    bool check(TokenType type) const;
    
public:
    explicit Parser(std::vector<Token> toks);
    std::unique_ptr<Statement> parse();
};

#endif // PARSER_H