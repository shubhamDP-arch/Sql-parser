#include "parser/lexer.h"
#include "parser/parser.h"
#include <iostream>
#include <string>

int main() {
    std::string sql = "SELECT name from USERS";
    
    Lexer lexer(sql);
    auto tokens = lexer.tokenize();
    
    Parser parser(tokens);
    auto ast = parser.parse();
    
    std::cout << "AST: " << ast->toString() << "\n";
    
    return 0;
}