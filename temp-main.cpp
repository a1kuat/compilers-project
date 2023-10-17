#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <regex>

const std::string VAR_KEYWORD = "var";
const std::string IF_KEYWORD = "if";
const std::string THEN_KEYWORD = "then";
const std::string ELSE_KEYWORD = "else";
const std::string END_KEYWORD = "end";
const std::string FOR_KEYWORD = "for";
const std::string WHILE_KEYWORD = "while";
const std::string LOOP_KEYWORD = "loop";
const std::string PRINT_KEYWORD = "print";
const std::string FUNC_KEYWORD = "func";
const std::string EMPTY_KEYWORD = "empty";
const std::string TRUE_KEYWORD = "true";
const std::string FALSE_KEYWORD = "false";
const std::string IS_KEYWORD = "is";
const std::string RANGE_KEYWOED = "range";

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

    Token() {}
    Token(TokenType t, std::string v) : type(t), value(v) {}
    ~Token() {}
};

class Lexer
{
    std::string src;
    std::list<Token> tokenCollection;

public:
    Lexer() {}

    Lexer(std::string fileName)
    {
        std::ifstream file(fileName);
        if (!file.is_open())
        {
            std::cerr << "Failed to open the file." << std::endl;
            return;
        }

        std::string line;
        while (std::getline(file, line))
        {
            src += line + '\n';
        }
        file.close();
    }

    std::list<Token> analyze()
    {
        std::string operators = "+-*/,:=(){}[];.><!";

        for (int i = 0; i < src.length(); i++)
        {
            std::string token = "";

            if (src[i] == ' ' || src[i] == '\n' || src[i] == '\t')
                continue;

            else if (operators.find(src[i]) != std::string::npos)
            {
                TokenType t;
                if (src[i] == ':')
                {
                    if (i + 1 < src.size() && src[i + 1] == '=')
                    {
                        token += src[i];
                        token += src[i + 1];
                        i++;
                        t = TokenType::DEFINITION;
                    }
                    else
                    {
                        token += src[i];
                        t = TokenType::COLON;
                    }
                }
                else if (src[i] == '/')
                {
                    if (i + 1 < src.size() && src[i + 1] == '/')
                    {
                        int j = i;
                        while (j < src.size() && src[j] != '\n')
                        {
                            token += src[j];
                            j++;
                        }
                        i = j - 1;
                        t = TokenType::COMMENT;
                    }
                    else
                    {
                        token = "/";
                        t = TokenType::DIVIDE;
                    }
                }
                else if (src[i] == '=')
                {
                    if (i + 1 < src.size() && src[i + 1] == '=')
                    {
                        token += src[i];
                        token += src[i + 1];
                        i++;
                        t = TokenType::EQUALITY;
                    }
                    else if (i + 1 < src.size() && src[i + 1] == '>')
                    {
                        token += src[i];
                        token += src[i + 1];
                        i++;
                        t = TokenType::FOLLOWING;
                    }
                    else
                    {
                        token += src[i];
                        t = TokenType::EQUAL;
                    }
                }
                else if (src[i] == '!')
                {
                    if (i + 1 < src.size() && src[i + 1] == '=')
                    {
                        token += src[i];
                        token += src[i + 1];
                        i++;
                        t = TokenType::UNEQUALITY;
                    }
                    else
                    {
                        token += src[i];
                        t = TokenType::EXCLAMATION;
                    }
                }
                else if (src[i] == '>')
                {
                    if (i + 1 < src.size() && src[i + 1] == '=')
                    {
                        token += src[i];
                        token += src[i + 1];
                        i++;
                        t = TokenType::MOREOREQUAL;
                    }
                    else
                    {
                        token += src[i];
                        t = TokenType::MORE;
                    }
                }
                else if (src[i] == '<')
                {
                    if (i + 1 < src.size() && src[i + 1] == '=')
                    {
                        token += src[i];
                        token += src[i + 1];
                        i++;
                        t = TokenType::LESSOREQUAL;
                    }
                    else
                    {
                        token += src[i];
                        t = TokenType::LESS;
                    }
                }

                else
                {
                    token += src[i];
                    t = getTokenType(src[i]);
                }

                tokenCollection.push_back(Token{t, token});
                continue;
            }

            else if (isDigit(src[i]))
            {
                TokenType ttype = TokenType::NUMBER;
                while (i < src.length() && (isDigit(src[i]) || src[i] == '.'))
                {
                    token += src[i];
                    i++;
                }

                if (isRange(token))
                {
                    ttype = TokenType::RANGE;
                }

                if (hasMoreThanOneDot(token))
                {
                    ttype = TokenType::UNKNOWNTYPE;
                }

                i--;
                tokenCollection.push_back(Token{ttype, token});
                continue;
            }

            else if (isIdentifierOk(src[i]))
            {
                while (i < src.length() && (isIdentifierOk(src[i]) || isDigit(src[i])))
                {
                    token += src[i];
                    i++;
                }

                tokenCollection.push_back(Token{getKeyWord(token), token});

                i--;
                continue;
            }

            else
            {
                token += src[i];
                tokenCollection.push_back(Token{TokenType::UNKNOWNTYPE, token});
            }
        }
        return tokenCollection;
    }

    bool isRange(const std::string &str)
    {
        std::regex rangeRegex("^\\d+\\.\\.\\d+$");
        return std::regex_match(str, rangeRegex);
    }

    bool hasMoreThanOneDot(const std::string &str)
    {
        int count = 0;
        for (char ch : str)
        {
            if (ch == '.')
            {
                count++;
                if (count > 1)
                {
                    return true;
                }
            }
        }
        return false;
    }

    TokenType getTokenType(char c)
    {
        switch (c)
        {
        case '+':
            return TokenType::PLUS;
        case '-':
            return TokenType::MINUS;
        case '/':
            return TokenType::DIVIDE;
        case '*':
            return TokenType::MULTIPLY;
        case ',':
            return TokenType::COMMA;
        case ';':
            return TokenType::DELIMITER;
        case '(':
            return TokenType::OPENBRACKET;
        case ')':
            return TokenType::CLOSEBRACKET;
        case '{':
            return TokenType::OPENFIGUREBRACKET;
        case '}':
            return TokenType::CLOSEFIGUREBRACKET;
        case '[':
            return TokenType::OPENSQUAREBRACKET;
        case ']':
            return TokenType::CLOSESQUAREBRACKET;
        case ':':
            return TokenType::COLON;
        case '=':
            return TokenType::EQUAL;
        default:
            return TokenType::IDENTIFIER;
        }
    }

    TokenType getKeyWord(std::string str)
    {
        std::string sarr[14] = {VAR_KEYWORD, IF_KEYWORD, THEN_KEYWORD, ELSE_KEYWORD, FOR_KEYWORD, END_KEYWORD,
                                WHILE_KEYWORD, LOOP_KEYWORD, PRINT_KEYWORD, FUNC_KEYWORD, EMPTY_KEYWORD, TRUE_KEYWORD, FALSE_KEYWORD, IS_KEYWORD};
        for (int i = 0; i < 14; i++)
        {
            if (sarr[i] == str)
                return TokenType::KEYWORD;
        }
        return TokenType::IDENTIFIER;
    }

    void error(std::string keyword)
    {
        std::cout << "ERROR: " << keyword << '\n';
    }

    bool isSpace(char c)
    {
        switch (c)
        {
        case ' ':
        case '\t':
        case '\n':
            return true;
        default:
            return false;
        }
    }

    bool isDigit(char c)
    {
        return ('0' <= c && c <= '9');
    }

    bool isIdentifierOk(char c)
    {
        if ('a' <= c && c <= 'z')
            return true;
        if ('A' <= c && c <= 'Z')
            return true;
        if (c == '_')
            return true;
        return false;
    }

    std::string getTokenTypeName(TokenType tokenType)
    {
        std::string typeName;

        switch (tokenType)
        {
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
        case TokenType::UNKNOWNTYPE:
            typeName = "UNKNOWNTYPE";
            break;
        case TokenType::DOT:
            typeName = "DOT";
            break;
        case TokenType::COLON:
            typeName = "COLON";
            break;
        case TokenType::EQUAL:
            typeName = "EQUAL";
            break;
        case TokenType::EQUALITY:
            typeName = "EQUALITY";
            break;
        case TokenType::UNEQUALITY:
            typeName = "UNEQUALITY";
            break;
        case TokenType::MORE:
            typeName = "MORE";
            break;
        case TokenType::LESS:
            typeName = "LESS";
            break;
        case TokenType::MOREOREQUAL:
            typeName = "MOREOREQUAL";
            break;
        case TokenType::LESSOREQUAL:
            typeName = "LESSOREQUAL";
            break;
        case TokenType::FOLLOWING:
            typeName = "FOLLOWING";
            break;
        }

        return typeName;
    }

    void printTokens(std::list<Token> tokens)
    {
        for (auto token : tokens)
        {
            std::cout << getTokenTypeName(token.type) << " |" << token.value << "|\n";
        }
    }
};

class Statement
{
    std::list<Token> tokens;

public:
    Statement(std::list<Token> &t) : tokens(t) {}

    std::list<Token> getTokens()
    {
        return tokens;
    }
};

enum class NodeType
{
    PROGRAM,
    EXPRESSION,
    TERM,
    FACTOR,
    VARIABLE,
    CONSTANT,
    ASSIGNMENT,
    ASSIGNMENT_LEFT,
    ASSIGNMENT_RIGHT,
    IF_STATEMENT,
    IF_BODY,
    IF_COND,

};

class AST
{
public:
    class Node
    {
    public:
        NodeType type;
        std::list<Node> children;
        Token value;

        Node()
        {
        }
    };

    Node node;

    AST()
    {
        node.type = NodeType::PROGRAM;
    }
};

class Parser : public Lexer
{
    std::list<Token> tokens;
    AST tree;
    std::list<Token> curTokensList;
    Token curToken;

public:
    Parser(std::list<Token> tokenCollection)
    {
        tokens = tokenCollection;
    }

    Token getToken()
    {
        Token t = tokens.front();
        tokens.pop_front();
        return t;
    }

    bool getCurToken()
    {
        if (curTokensList.empty())
            return false;
        Token t = curTokensList.front();
        curTokensList.pop_front();
        curToken = t;
        return true;
    }

    AST::Node handleAssignmentStatement()
    {
        Token token = getToken();
        std::list<Token> left;
        while (token.type != TokenType::DEFINITION)
        {
            left.push_back(token);
            token = getToken();
        }

        if (left.empty())
        {
            // THROW ERROR empty assignment
        }

        token = getToken();
        std::list<Token> right;

        while (token.type != TokenType::DELIMITER)
        {
            right.push_back(token);
            token = getToken();
        }

        if (right.empty())
        {
            // THROW ERROR empty assignment
        }

        AST::Node assignmentNode;

        AST::Node leftNode;
        leftNode.type = NodeType::ASSIGNMENT_LEFT;
        leftNode.children.push_back(getNodeFromList(left));

        AST::Node rightNode;
        rightNode.type = NodeType::ASSIGNMENT_RIGHT;
        rightNode.children.push_back(getNodeFromList(right));

        assignmentNode.type = NodeType::ASSIGNMENT;
        assignmentNode.children.push_back(rightNode);
        assignmentNode.children.push_back(leftNode);

        return AST::Node();
    }

    AST::Node getNodeFromList(std::list<Token> tokens)
    {
        if (tokens.size() == 1)
        {
            // const/variable
        }

        else
        {
            // expression

            // if

            // func

            //
        }

        return AST::Node();
    }

    AST::Node getNode()
    {
        Token tk = getToken();

        // Assignment Statement

        if (tk.type == TokenType::NUMBER || tk.type == TokenType::IDENTIFIER || tk.type == TokenType::OPENBRACKET)
        {
            while (tk.type != TokenType::DELIMITER)
            {
                curTokensList.push_back(tk);
                tk = getToken();
            }

            for (auto t : curTokensList)
            {
                std::cout << t.value << ' ';
            }
            std::cout << '\n';

            getCurToken();
            tree.node.children.push_back(parseExpr());

            for (auto t : curTokensList)
            {
                std::cout << t.value << ' ';
            }

            std::cout << "\n";

            printAST(tree.node);
        }

        if (tk.type == TokenType::KEYWORD && tk.value == VAR_KEYWORD)
        {
            return handleAssignmentStatement();
        }

        return AST::Node();
    }

    void analyze()
    {
        while (!tokens.empty())
        {
            AST::Node newNode = getNode();
        }
    }
    AST::Node makeTree(NodeType nodetype, Token value, std::list<AST::Node> children)
    {
        AST::Node newnode;
        newnode.type = nodetype;
        newnode.value = value;
        swap(newnode.children, children);
        return newnode;
    }
    AST::Node parseExpr()
    {
        AST::Node left = parseTerm();
        while (curToken.type == TokenType::PLUS || curToken.type == TokenType::MINUS)
        {
            Token tk = curToken;
            getCurToken();
            left = makeTree(NodeType::EXPRESSION, tk, {left, parseTerm()});
        }

        return left;
    }

    AST::Node parseTerm()
    {
        AST::Node left = parseFactor();
        while (curToken.type == TokenType::MULTIPLY || curToken.type == TokenType::DIVIDE)
        {
            Token tk = curToken;
            getCurToken();
            left = makeTree(NodeType::TERM, tk, {left, parseFactor()});
        }

        return left;
    }

    AST::Node parseFactor()
    {
        AST::Node res;
        Token tk = curToken;
        if (tk.type == TokenType::OPENBRACKET)
        {
            getCurToken(); // skip '('
            res = parseExpr();
            getCurToken(); // skip ')'
        }
        else
        {
            getCurToken();
            res = makeTree(NodeType::FACTOR, tk, {});
        }
        return res;
    }

    std::string getNodeTypeName(NodeType nodetype)
    {
        std::string typeName;

        switch (nodetype)
        {
        case NodeType::PROGRAM:
            typeName = "PROGRAM";
            break;

        case NodeType::ASSIGNMENT:
            typeName = "ASSIGNMENT";
            break;

        case NodeType::FACTOR:
            typeName = "FACTOR";
            break;

        case NodeType::CONSTANT:
            typeName = "CONSTANT";
            break;

        case NodeType::TERM:
            typeName = "TERM";
            break;

        case NodeType::VARIABLE:
            typeName = "VARIABLE";
            break;

        case NodeType::ASSIGNMENT_LEFT:
            typeName = "ASSIGNMENT_LEFT";
            break;

        case NodeType::ASSIGNMENT_RIGHT:
            typeName = "ASSIGNMENT_RIGHT";
            break;

        case NodeType::IF_BODY:
            typeName = "IF_BODY";
            break;

        case NodeType::IF_COND:
            typeName = "IF_COND";
            break;

        case NodeType::IF_STATEMENT:
            typeName = "IF_STATEMENT";
            break;

        case NodeType::EXPRESSION:
            typeName = "EXPRESSION";
            break;
        }

        return typeName;
    }

    void printAST(AST::Node node, std::string tab = "")
    {
        std::string TAB = "   ";
        std::cout << tab << "{\n";
        std::string t = tab;
        tab += TAB;
        std::cout << tab << "\"Token\": {\"type\": \"" << Lexer::getTokenTypeName(node.value.type) << "\", \"value\": \"" << node.value.value << "\"}," << '\n';
        std::cout << tab << "\"Type\": \"" << getNodeTypeName(node.type) << "\",\n";
        if (node.children.empty())
        {
            std::cout << tab << "\"children\": []\n";
        }
        else
        {
            std::cout << tab << "\"children\": [\n";
            for (auto n : node.children)
            {
                printAST(n, tab + TAB);
            }
            std::cout << tab << "]\n";
        }
        std::cout << t << "},\n";
    }
};

int main(int argc, char **argv)
{

    if (argc != 2)
    {
        std::cerr << "ERROR: ";
        return 1;
    }

    Lexer lexer(argv[1]);

    std::list<Token> tokens = lexer.analyze();

    // lexer.printTokens(tokens);

    Parser parser(tokens);
    parser.analyze();

    return 0;
}
