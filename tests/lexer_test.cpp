#include <gtest/gtest.h>
#include "parser/lexer.h"

class LexerTest : public ::testing::Test {
protected:
    void expectToken(const Token& token, TokenType expected_type, 
                     const std::string& expected_value) {
        EXPECT_EQ(token.type, expected_type);
        EXPECT_EQ(token.value, expected_value);
    }
};

TEST_F(LexerTest, TokenizeKeywords) {
    Lexer lexer("SELECT FROM WHERE");
    auto tokens = lexer.tokenize();
    
    ASSERT_EQ(tokens.size(), 4); // 3 keywords + EOF
    expectToken(tokens[0], TokenType::SELECT, "SELECT");
    expectToken(tokens[1], TokenType::FROM, "FROM");
    expectToken(tokens[2], TokenType::WHERE, "WHERE");
    expectToken(tokens[3], TokenType::END_OF_FILE, "");
}

TEST_F(LexerTest, TokenizeKeywordsCaseInsensitive) {
    Lexer lexer("select SeLeCt SELECT");
    auto tokens = lexer.tokenize();
    
    ASSERT_EQ(tokens.size(), 4);
    EXPECT_EQ(tokens[0].type, TokenType::SELECT);
    EXPECT_EQ(tokens[1].type, TokenType::SELECT);
    EXPECT_EQ(tokens[2].type, TokenType::SELECT);
}

TEST_F(LexerTest, TokenizeIdentifiers) {
    Lexer lexer("table_name column123 _underscore");
    auto tokens = lexer.tokenize();
    
    ASSERT_EQ(tokens.size(), 4);
    expectToken(tokens[0], TokenType::IDENTIFIER, "table_name");
    expectToken(tokens[1], TokenType::IDENTIFIER, "column123");
    expectToken(tokens[2], TokenType::IDENTIFIER, "_underscore");
}

TEST_F(LexerTest, TokenizeNumbers) {
    Lexer lexer("123 45.67 0.5 100");
    auto tokens = lexer.tokenize();
    
    ASSERT_EQ(tokens.size(), 5);
    expectToken(tokens[0], TokenType::NUMBER, "123");
    expectToken(tokens[1], TokenType::NUMBER, "45.67");
    expectToken(tokens[2], TokenType::NUMBER, "0.5");
    expectToken(tokens[3], TokenType::NUMBER, "100");
}

TEST_F(LexerTest, TokenizeStrings) {
    Lexer lexer("'hello' 'world with spaces' 'it\\'s escaped'");
    auto tokens = lexer.tokenize();
    
    ASSERT_EQ(tokens.size(), 4);
    expectToken(tokens[0], TokenType::STRING, "hello");
    expectToken(tokens[1], TokenType::STRING, "world with spaces");
    expectToken(tokens[2], TokenType::STRING, "it's escaped");
}

TEST_F(LexerTest, TokenizeOperators) {
    Lexer lexer("= != < > <= >= + - * /");
    auto tokens = lexer.tokenize();
    
    ASSERT_EQ(tokens.size(), 11);
    expectToken(tokens[0], TokenType::EQUALS, "=");
    expectToken(tokens[1], TokenType::NOT_EQUALS, "!=");
    expectToken(tokens[2], TokenType::LESS_THAN, "<");
    expectToken(tokens[3], TokenType::GREATER_THAN, ">");
    expectToken(tokens[4], TokenType::LESS_EQUAL, "<=");
    expectToken(tokens[5], TokenType::GREATER_EQUAL, ">=");
    expectToken(tokens[6], TokenType::PLUS, "+");
    expectToken(tokens[7], TokenType::MINUS, "-");
    expectToken(tokens[8], TokenType::STAR, "*");
    expectToken(tokens[9], TokenType::SLASH, "/");
}

TEST_F(LexerTest, TokenizePunctuation) {
    Lexer lexer("( ) , ;");
    auto tokens = lexer.tokenize();
    
    ASSERT_EQ(tokens.size(), 5);
    expectToken(tokens[0], TokenType::LEFT_PAREN, "(");
    expectToken(tokens[1], TokenType::RIGHT_PAREN, ")");
    expectToken(tokens[2], TokenType::COMMA, ",");
    expectToken(tokens[3], TokenType::SEMICOLON, ";");
}

TEST_F(LexerTest, TokenizeCompleteSelectStatement) {
    Lexer lexer("SELECT name, age FROM users WHERE age > 18");
    auto tokens = lexer.tokenize();
    
    ASSERT_EQ(tokens.size(), 11);
    expectToken(tokens[0], TokenType::SELECT, "SELECT");
    expectToken(tokens[1], TokenType::IDENTIFIER, "name");
    expectToken(tokens[2], TokenType::COMMA, ",");
    expectToken(tokens[3], TokenType::IDENTIFIER, "age");
    expectToken(tokens[4], TokenType::FROM, "FROM");
    expectToken(tokens[5], TokenType::IDENTIFIER, "users");
    expectToken(tokens[6], TokenType::WHERE, "WHERE");
    expectToken(tokens[7], TokenType::IDENTIFIER, "age");
    expectToken(tokens[8], TokenType::GREATER_THAN, ">");
    expectToken(tokens[9], TokenType::NUMBER, "18");
}

TEST_F(LexerTest, SkipsWhitespace) {
    Lexer lexer("  SELECT   name  \n\t FROM   users  ");
    auto tokens = lexer.tokenize();
    
    ASSERT_EQ(tokens.size(), 5);
    expectToken(tokens[0], TokenType::SELECT, "SELECT");
    expectToken(tokens[1], TokenType::IDENTIFIER, "name");
    expectToken(tokens[2], TokenType::FROM, "FROM");
    expectToken(tokens[3], TokenType::IDENTIFIER, "users");
}

TEST_F(LexerTest, EmptyInput) {
    Lexer lexer("");
    auto tokens = lexer.tokenize();
    
    ASSERT_EQ(tokens.size(), 1);
    expectToken(tokens[0], TokenType::END_OF_FILE, "");
}

TEST_F(LexerTest, TokenPositions) {
    Lexer lexer("SELECT name FROM users");
    auto tokens = lexer.tokenize();
    
    EXPECT_EQ(tokens[0].position, 0);   // SELECT at position 0
    EXPECT_EQ(tokens[1].position, 7);   // name at position 7
    EXPECT_EQ(tokens[2].position, 12);  // FROM at position 12
    EXPECT_EQ(tokens[3].position, 17);  // users at position 17
}