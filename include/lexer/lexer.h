#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <regex>
#include "keyword.h"

enum class TokenType
{
    NUMBER,
    IDENTIFIER,
    COMMA,
    KEYWORD,
    DELIMITER,
    PLUS,
    MINUS,
    MULTIPLY,
    DIVIDE,
    END_OF_FILE,
    COMMENT,
    DEFINITION,
    OPENBRACKET,
    CLOSEBRACKET,
    OPENSQUAREBRACKET,
    CLOSESQUAREBRACKET,
    OPENFIGUREBRACKET,
    CLOSEFIGUREBRACKET,
    RANGE,
    DOT,
    UNKNOWNTYPE,
    COLON,
    EQUAL,
    EQUALITY,
    UNEQUALITY,
    LESSOREQUAL,
    MOREOREQUAL,
    LESS,
    MORE,
    EXCLAMATION,
    FOLLOWING
};

class Token
{
public:
    TokenType type;
    std::string value;

    Token();
    Token(TokenType t, std::string v);
    // ~Token();
};

class Lexer
{
    std::string src;
    std::list<Token> tokenCollection;

public:
    Lexer();

    Lexer(std::string fileName);

    std::list<Token> analyze();

    bool isRange(const std::string &str);

    bool hasMoreThanOneDot(const std::string &str);

    TokenType getTokenType(char c);
    
    TokenType getKeyWord(std::string str);

    void error(std::string keyword);

    bool isSpace(char c);

    bool isDigit(char c);
    
    bool isIdentifierOk(char c);

    std::string getTokenTypeName(TokenType tokenType);
    
    void printTokens(std::list<Token> tokens);
};
