#pragma once
#include <string>
#include <list>
#include "../Lexer/lexer.h"

enum class TokenType
{
    NUMBER,
    IDENTIFIER,
    COMMA,
    KEYWORD,
    DELIMITER,
    LEFTBRACKET,
    RIGHTBRACKET,
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

class Token {
public:
    TokenType type;
    std::string value;
};

class TreeNode {
    Token token;
    TreeNode** children;
};

class AST {
    std::list<Token> tokens;
    TreeNode *program;
public:
    AST(std::list<Token> tokenCollection) {
        tokens = tokenCollection;
        program = nullptr;
    }

    Token getToken() {
        Token t = tokens.front();
        tokens.pop_front();
        return t;
    }

    TreeNode parseId() {
        
    }

    TreeNode mkBinTree(Token tk, TreeNode left, TreeNode right){
        return TreeNode(token = , children = left, right);
    }

    TreeNode mkUnaryTree(Token tk, TreeNode node){
        
    }

    TreeNode parseExpr() {
        TreeNode left = parseTerm();
        Token tk;
        while (tk = getToken(), tk.type == TokenType::PLUS || tk.type == TokenType::MINUS)
            left = mkBinTree(tk, left, parseTerm());
        return left;
    }

    TreeNode parseTerm() {
        TreeNode left = parseFactor();
        Token tk;
        while (tk = getToken(), tk.type == TokenType::MULTIPLY || tk.type == TokenType::DIVIDE)
            left = mkBinTree(tk, left, parseFactor());
        return left;
    }

    TreeNode parseFactor() {
        TreeNode res;
        Token tk;
        if (tk = getToken(), tk.type == TokenType::LEFTBRACKET){
            res = parseExpr();
            getToken(); // skip ')'
        }
        else
            res = mkUnaryTree(parseId());
        return res;
    }

};
