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
    END_OF_FILE
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
        std::string operators = "+-*/,";
        std::string delimiters = ";";
        
        for(int i = 0; i < src.length(); i++) {
            std::string token = "";
            
            if (src[i] == ' ' || src[i] == '\n' || src[i] == '\t')
                continue;
            
            if (operators.find(src[i]) != std::string::npos) {

                token += src[i];
                
                tokenCollection.push_back(Token{getCharType(src[i]), token});

                std::cout << "Operator: " << token << '\n';
                
                continue;
            }

            if (delimiters.find(src[i]) != std::string::npos) {
                token += src[i];

                tokenCollection.push_back(Token{getCharType(src[i]), token});
                std::cout << "Delimiter: " << token << '\n';
                continue;
            }

            if (isDigit(src[i])) {
                while(i < src.length() && (isdigit(src[i]) || src[i] == '.')) {
                    token += src[i];
                    i++;
                }

                std::cout << "Number: " << token << '\n';
                i--;
                continue;
            }

            if (isIdentifierOk(src[i])) {
                while(i < src.length() && isIdentifierOk(src[i])) {
                    token += src[i];
                    i++;
                }

                tokenCollection.push_back(Token{getKeyWord(token), token});

                std::cout << "Identifier: " << token << '\n';
                i--;
                continue;
            }
        }
        return tokenCollection;
    }


    TokenType getCharType(char c) {
        switch(c){
            case '+': return TokenType::PLUS;
            case '-': return TokenType::MINUS;
            case '/': return TokenType::DIVIDE;
            case '*': return TokenType::MULTIPLY;
            case ',': return TokenType::COMMA;
            case ';': return TokenType::DELIMITER;
            default:
                // WARNING!
                break;
        }
    }

    TokenType getKeyWord(std::string str) {
        if(str == VAR_KEYWORD) return TokenType::KEYWORD;

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

    bool isIdentifierOk(char c) noexcept {
        if('a' <= c && c <= 'z') return true;
        if('A' <= c && c <= 'Z') return true;
        if(isDigit(c)) return true;
        if(c == '_') return true;
        return false;
    }

    
};



int main(int argc, char** argv) {

    if(argc != 2) {
        std::cerr << "ERROR: ";
        return 1;
    }

    Lexer lexer(argv[1]);

    std::vector<Token> tokens = lexer.analyze();
    

    return 0;
}

