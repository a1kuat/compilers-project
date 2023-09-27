#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "keywords.h"

enum class TokenType {
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
    RANGE
};

class Token {
public:
    TokenType type;
    std::string value;
};


class Lexer {
    int currentPosition;
    std::string src;
    std::vector<Token> tokenCollection;
public:
    
    Lexer(std::string fileName) {
        std::ifstream file(fileName);
        if(!file.is_open()) {
            std::cerr << "Failed to open the file." << std::endl;
            return;
        }

        std::string line;
        while(std::getline(file, line)) {
            src += line + '\n';
        }
        file.close();
    }

    std::vector<Token> analyze() {
        std::string operators = "+-*/,:=(){}[]";
        std::string delimiters = ";";
        
        for(int i = 0; i < src.length(); i++) {
            std::string token = "";
            
            if (src[i] == ' ' || src[i] == '\n' || src[i] == '\t')
                continue;
            
            if (operators.find(src[i]) != std::string::npos) {
                TokenType t;
                if(src[i] == ':') {
                    if(i + 1 < src.size() && src[i+1] == '='){
                        token += src[i];
                        token += src[i+1];
                        i++;
                    }
                    t = TokenType::DEFINITION;
                    
                } else if(src[i] == '/') {
                    if(i + 1 < src.size() && src[i+1] == '/'){
                        int j = i;
                        while (j < src.size() && src[j] != '\n'){
                            token += src[j];
                            j ++;
                        }
                        i = j - 1;
                        t = TokenType::COMMENT;
                    }
                } else {
                    token += src[i];
                    t = getTokenType(src[i]);
                }
            
                tokenCollection.push_back(Token{t, token});
                continue;
            }

            if (delimiters.find(src[i]) != std::string::npos) {
                token += src[i];

                tokenCollection.push_back(Token{getTokenType(src[i]), token});
                continue;
            }

            if (isDigit(src[i])) {
                TokenType ttype = TokenType::NUMBER;
                while(i < src.length() && (isDigit(src[i]) || src[i] == '.')) {
                    token += src[i];
                    i++;
                    
                    if(src[i] == '.' && src[i+1] == '.'){
                        ttype = TokenType::RANGE;
                    }

                }
                    

                i--;
                tokenCollection.push_back(Token{ttype, token});
                continue;
            }

            if (isIdentifierOk(src[i])) {
                while(i < src.length() && (isIdentifierOk(src[i]) || isDigit(src[i]))) {
                    token += src[i];
                    i++;
                }

                tokenCollection.push_back(Token{getKeyWord(token), token});

                i--;
                continue;
            }
        }
        return tokenCollection;
    }


    TokenType getTokenType(char c) {
        switch(c){
            case '+': return TokenType::PLUS;
            case '-': return TokenType::MINUS;
            case '/': return TokenType::DIVIDE;
            case '*': return TokenType::MULTIPLY;
            case ',': return TokenType::COMMA;
            case ';': return TokenType::DELIMITER;
            case '(': return TokenType::OPENBRACKET;
            case ')': return TokenType::CLOSEBRACKET;
            case '{': return TokenType::OPENFIGUREBRACKET;
            case '}': return TokenType::CLOSEFIGUREBRACKET;
            case '[': return TokenType::OPENSQUAREBRACKET;
            case ']': return TokenType::CLOSESQUAREBRACKET;
            default:
                return TokenType::IDENTIFIER;
        }
    }

    TokenType getKeyWord(std::string str) {
        std::string sarr[14] = {VAR_KEYWORD, IF_KEYWORD, THEN_KEYWORD, ELSE_KEYWORD, FOR_KEYWORD, END_KEYWORD,
                            WHILE_KEYWORD, LOOP_KEYWORD, PRINT_KEYWORD, FUNC_KEYWORD, EMPTY_KEYWORD, TRUE_KEYWORD, FALSE_KEYWORD, IS_KEYWORD};
        for (int i = 0; i < 14; i ++) {
            if(sarr[i] == str) return TokenType::KEYWORD;
        }
        return TokenType::IDENTIFIER;
    }
     
    void error(std::string keyword) {
        std::cout << "ERROR: " << keyword << '\n';
    }

    bool isSpace(char c){
        switch(c){
            case ' ':
            case '\t':
            case '\n':
                return true;
            default:
                return false;
        }
    }

    bool isDigit(char c){
        return ('0' <= c && c <= '9');
    }

    bool isIdentifierOk(char c) {
        if('a' <= c && c <= 'z') return true;
        if('A' <= c && c <= 'Z') return true;
        if(c == '_') return true;
        return false;
    }


    std::string getTokenTypeName(TokenType tokenType) {
        std::string typeName;

        switch(tokenType) {
            case TokenType::NUMBER:
                typeName = "NUMBER";
                break;
            case TokenType::IDENTIFIER:
                typeName = "IDENTIFIER";
                break;
            case TokenType::COMMA:
                typeName = "COMMA";
                break;
            case TokenType::KEYWORD:
                typeName = "KEYWORD";
                break;
            case TokenType::DELIMITER:
                typeName = "DELIMITER";
                break;
            case TokenType::LEFTBRACKET:
                typeName = "LEFTBRACKET";
                break;
            case TokenType::RIGHTBRACKET:
                typeName = "RIGHTBRACKET";
                break;
            case TokenType::PLUS:
                typeName = "PLUS";
                break;
            case TokenType::MINUS:
                typeName = "MINUS";
                break;
            case TokenType::MULTIPLY:
                typeName = "MULTIPLY";
                break;
            case TokenType::DIVIDE:
                typeName = "DIVIDE";
                break;
            case TokenType::END_OF_FILE:
                typeName = "END_OF_FILE";
                break;
            case TokenType::COMMENT:
                typeName = "COMMENT";
                break;
            case TokenType::DEFINITION:
                typeName = "DEFINITION";
                break;
            case TokenType::OPENBRACKET:
                typeName = "OPENBRACKET";
                break;
            case TokenType::CLOSEBRACKET:
                typeName = "CLOSEBRACKET";
                break;
            case TokenType::OPENSQUAREBRACKET:
                typeName = "OPENSQUAREBRACKET";
                break;
            case TokenType::CLOSESQUAREBRACKET:
                typeName = "CLOSESQUAREBRACKET";
                break;
            case TokenType::OPENFIGUREBRACKET:
                typeName = "OPENFIGUREBRACKET";
                break;
            case TokenType::CLOSEFIGUREBRACKET:
                typeName = "CLOSEFIGUREBRACKET";
                break;
            case TokenType::RANGE:
                typeName = "RANGE";
                break;
        }

        return typeName;
    }


    void printTokens(std::vector<Token> tokens){
        for (auto token : tokens) {
            std::cout << getTokenTypeName(token.type) << " |" << token.value << "|\n";
        }
    }

};

int main(int argc, char** argv) {

    if(argc != 2) {
        std::cerr << "ERROR: ";
        return 1;
    }

    Lexer lexer(argv[1]);

    std::vector<Token> tokens = lexer.analyze();

    lexer.printTokens(tokens);
    

    return 0;
}

