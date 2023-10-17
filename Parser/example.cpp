#include <iostream>
#include <string>
#include <vector>

// Token types
enum class TokenType {
    INTEGER,
    PLUS,
    MINUS,
    MULTIPLY,
    DIVIDE,
    LPAREN,
    RPAREN,
    END
};

// Token structure
struct Token {
    TokenType type;
    std::string value;
};

class Parser {
public:
    Parser(const std::string& input) : input(input), pos(0), currentToken({TokenType::END, ""}) {}

    // Lexical analyzer (Tokenizer)
    void getNextToken() {
        if (pos >= input.length()) {
            currentToken = {TokenType::END, ""};
            return;
        }

        char currentChar = input[pos];

        if (isdigit(currentChar)) {
            std::string value;
            while (pos < input.length() && isdigit(input[pos])) {
                value += input[pos];
                pos++;
            }
            currentToken = {TokenType::INTEGER, value};
            return;
        }

        switch (currentChar) {
            case '+':
                currentToken = {TokenType::PLUS, "+"};
                break;
            case '-':
                currentToken = {TokenType::MINUS, "-"};
                break;
            case '*':
                currentToken = {TokenType::MULTIPLY, "*"};
                break;
            case '/':
                currentToken = {TokenType::DIVIDE, "/"};
                break;
            case '(':
                currentToken = {TokenType::LPAREN, "("};
                break;
            case ')':
                currentToken = {TokenType::RPAREN, ")"};
                break;
            default:
                // Invalid character
                throw std::runtime_error("Invalid character: " + std::string(1, currentChar));
        }

        pos++;
    }

    // Expression grammar rule: term ((PLUS | MINUS) term)*
    double expression() {
        double result = term();

        while (currentToken.type == TokenType::PLUS || currentToken.type == TokenType::MINUS) {
            Token op = currentToken;
            getNextToken();
            double nextTerm = term();
            if (op.type == TokenType::PLUS) {
                result += nextTerm;
            } else {
                result -= nextTerm;
            }
        }

        return result;
    }

    // Term grammar rule: factor ((MULIPLY | DIVIDE) factor)*
    double term() {
        double result = factor();

        while (currentToken.type == TokenType::MULTIPLY || currentToken.type == TokenType::DIVIDE) {
            Token op = currentToken;
            getNextToken();
            double nextFactor = factor();
            if (op.type == TokenType::MULTIPLY) {
                result *= nextFactor;
            } else {
                result /= nextFactor;
            }
        }

        return result;
    }

    // Factor grammar rule: INTEGER | LPAREN expression RPAREN
    double factor() {
        if (currentToken.type == TokenType::INTEGER) {
            double value = std::stod(currentToken.value);
            getNextToken();
            return value;
        } else if (currentToken.type == TokenType::LPAREN) {
            getNextToken();
            double result = expression();
            if (currentToken.type != TokenType::RPAREN) {
                throw std::runtime_error("Expected ')'");
            }
            getNextToken();
            return result;
        } else {
            throw std::runtime_error("Invalid token");
        }
    }

    double parse() {
        getNextToken();
        return expression();
    }

private:
    std::string
