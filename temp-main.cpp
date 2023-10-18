#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <regex>
#include <vector>

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
const std::string IN_KEYWORD = "in";

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
        std::string sarr[15] = {VAR_KEYWORD, IF_KEYWORD, THEN_KEYWORD, ELSE_KEYWORD, FOR_KEYWORD, END_KEYWORD,
                                WHILE_KEYWORD, LOOP_KEYWORD, PRINT_KEYWORD, FUNC_KEYWORD, EMPTY_KEYWORD, TRUE_KEYWORD, FALSE_KEYWORD, IS_KEYWORD, IN_KEYWORD};
        for (int i = 0; i < 15; i++)
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

enum class NodeType
{
    PROGRAM,
    EXPRESSION,
    TERM,
    FACTOR,
    IDENTIFIER,
    VARIABLE,
    CONSTANT,
    ASSIGNMENT,
    ASSIGNMENT_LEFT,
    ASSIGNMENT_RIGHT,
    IF_STATEMENT,
    IF_BODY,
    IF_COND,
    FUNCTION,
    FUNCTION_VARIABLES,
    FUNCTION_BODY,
    FORLOOP,
    WHILELOOP,
    RANGE,
    BODY,
    VARS_LIST,
    COMMENT,
    LIST,
    LIST_ELEMENT,
    TUPLE,
    TUPLE_ELEMENT
};

class AST
{
public:
    class Node
    {
    public:
        NodeType type;
        std::vector<Node> children;
        Token value;

        Node()
        {
        }

        Node(NodeType t) : type(t)
        {
        }

        Node(NodeType t, Token v) : type(t), value(v)
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

    AST::Node parseFunction()
    {

        Token funcToken = Token(TokenType::KEYWORD, FUNC_KEYWORD);

        AST::Node functionNode(NodeType::FUNCTION, funcToken);

        functionNode.value.value == FUNC_KEYWORD;
        functionNode.value.type == TokenType::KEYWORD;

        std::list<Token> variables;

        getCurToken();
        if (curToken.type != TokenType::OPENBRACKET)
        {
            throw std::runtime_error("No variables definition in the function!");
        }

        while (curToken.type != TokenType::CLOSEBRACKET)
        {
            variables.push_back(curToken);
            if (curTokensList.size() == 0)
            {
                throw std::runtime_error("Not closed bracket!");
            }

            getCurToken();
        }

        std::list<Token> tokensBackupOne = curTokensList;

        curTokensList = variables;
        getCurToken();

        // vars

        functionNode.children.push_back(parseVarsList());

        curTokensList = tokensBackupOne;

        std::list<Token> body;

        getCurToken();

        if (curToken.type != TokenType::KEYWORD && curToken.value != IS_KEYWORD)
        {
            throw std::runtime_error("Function body was not opened!");
        }

        while (!curTokensList.empty())
        {
            getCurToken();
            body.push_back(curToken);
        }

        AST::Node bodyNode;

        std::list<Token> tokensBackup;
        tokensBackup = tokens;

        tokens = body;

        bodyNode.type = NodeType::FUNCTION_BODY;
        bodyNode.children = parseBody();
        functionNode.children.push_back(bodyNode);

        tokens = tokensBackup;

        return functionNode;
    }

    AST::Node parseExprList()
    {
        AST::Node exprListNode;
        while (curTokensList.size() != 0)
        {
            getCurToken();
            AST::Node newNode;
            newNode.type = NodeType::FACTOR;
            newNode.value = curToken;
            exprListNode.children.push_back(newNode);

            if (curTokensList.size() == 0)
            {
                break;
            }

            getCurToken();

            if (curToken.type != TokenType::COMMA)
            {
                throw std::runtime_error("not found ',' between expressions!");
            }
        }

        exprListNode.type = NodeType::ASSIGNMENT_LEFT;
        return exprListNode;
    }

    AST::Node parseVarsList()
    {
        AST::Node varsListNode;
        while (curTokensList.size() != 0)
        {
            getCurToken();
            AST::Node newNode;
            newNode.type = NodeType::FACTOR;
            newNode.value = curToken;
            varsListNode.children.push_back(newNode);

            if (curTokensList.size() == 0)
            {
                break;
            }

            getCurToken();

            if (curToken.type != TokenType::COMMA)
            {
                throw std::runtime_error("not found ',' between variables!");
            }
        }

        varsListNode.type = NodeType::VARS_LIST;
        return varsListNode;
    }

    AST::Node parseForLoop()
    {
        AST::Node forNode; // before Loop
        forNode.type = NodeType::FORLOOP;

        Token token = getToken();
        std::vector<Token> left;
        while (token.value != LOOP_KEYWORD)
        {
            left.push_back(token);
            token = getToken();
        }
        token = getToken(); // skip loop keyword

        if (left.size() != 3 || left[0].type != TokenType::IDENTIFIER || left[1].value != IN_KEYWORD || left[2].type != TokenType::RANGE)
        {
            throw std::runtime_error("for condition is empty!");
        }

        forNode.children.push_back({NodeType::IDENTIFIER, left[0]});
        forNode.children.push_back({NodeType::RANGE, left[2]});

        AST::Node bodyNode; // Body
        bodyNode.type = NodeType::BODY;

        std::list<Token> right;

        while (token.type != TokenType::DELIMITER)
        {
            right.push_back(token);
            token = getToken();
        }
        token = getToken(); // skip end keyword

        if (right.empty())
        {
            throw std::runtime_error("var right part is empty!");
        }

        curTokensList = right;
        getCurToken(); // set curToken to the initial pos

        bodyNode.children = parseBody();
        forNode.children.push_back(bodyNode);

        return forNode;
    }

    AST::Node parseWhileLoop()
    {
        AST::Node whileNode;
        whileNode.type = NodeType::WHILELOOP;

        Token token = getToken();
        std::list<Token> left;
        while (token.value != LOOP_KEYWORD)
        {
            left.push_back(token);
            token = getToken();
        }

        if (left.empty())
        {
            throw std::runtime_error("while condition is empty!");
        }

        curTokensList = left;
        getCurToken();

        whileNode.children.push_back(parseCond());

        token = getToken();
        std::list<Token> right;

        while (token.value != END_KEYWORD)
        {
            right.push_back(token);
            token = getToken();
        }

        if (right.empty())
        {
            throw std::runtime_error("while body is empty!");
        }

        curTokensList = right;
        getCurToken();

        whileNode.children.push_back(parseExpr());

        return whileNode;
    }

    AST::Node parseCond()
    {
        AST::Node condNode;
        condNode.type = NodeType::IF_COND;

        Token token = getToken();
        std::list<Token> left;
        while (token.type != TokenType::LESS || token.type != TokenType::MORE || token.type != TokenType::LESSOREQUAL || token.type != TokenType::MOREOREQUAL || token.type != TokenType::EQUALITY || token.type != TokenType::UNEQUALITY)
        {
            left.push_back(token);
            token = getToken();
        }

        if (token.type == TokenType::LESS)
        {
            condNode.value.value = "<";
            condNode.value.type = TokenType::LESS;
        }
        if (token.type == TokenType::MORE)
        {
            condNode.value.value = ">";
            condNode.value.type = TokenType::MORE;
        }
        if (token.type == TokenType::MOREOREQUAL)
        {
            condNode.value.value = ">=";
            condNode.value.type = TokenType::MOREOREQUAL;
        }
        if (token.type == TokenType::LESSOREQUAL)
        {
            condNode.value.value = "<=";
            condNode.value.type = TokenType::LESSOREQUAL;
        }
        if (token.type == TokenType::EQUALITY)
        {
            condNode.value.value = "==";
            condNode.value.type = TokenType::EQUAL;
        }
        if (token.type == TokenType::UNEQUALITY)
        {
            condNode.value.value = "!=";
            condNode.value.type = TokenType::UNEQUALITY;
        }

        if (left.empty())
        {
            throw std::runtime_error("left condition part is empty!");
        }

        curTokensList = left;
        getCurToken();

        condNode.children.push_back(parseExpr());

        token = getToken();
        std::list<Token> right;

        while (token.type != TokenType::DELIMITER)
        {
            right.push_back(token);
            token = getToken();
        }

        if (right.empty())
        {
            throw std::runtime_error("right condition part is empty!");
        }

        curTokensList = right;
        getCurToken();

        condNode.children.push_back(parseExpr());

        return condNode;
    }

    // AST::Node getNodeRightAss() {

    // }

    AST::Node parseAssignment()
    {
        AST::Node assignmentNode;
        assignmentNode.type = NodeType::ASSIGNMENT;

        assignmentNode.value.value = ":=";
        assignmentNode.value.type = TokenType::DEFINITION;

        Token token = getToken();
        std::list<Token> left;

        while (token.type != TokenType::DEFINITION)
        {
            left.push_back(token);
            token = getToken();

            if (tokens.empty())
            {
                throw std::runtime_error("no declaration after var!");
            }
        }

        if (left.empty())
        {
            throw std::runtime_error("var left part is empty!");
        }

        curTokensList = left;

        AST::Node assignmentLeft = parseVarsList();
        assignmentLeft.type = NodeType::ASSIGNMENT_LEFT;
        assignmentNode.children.push_back(assignmentLeft);

        token = getToken();

        std::list<Token> right;
        std::list<Token> empty;

        AST::Node assignmentRight;
        assignmentRight.type = NodeType::ASSIGNMENT_RIGHT;

        bool allClosed = false;

        while (token.type != TokenType::DELIMITER)
        {
            if (token.type == TokenType::COMMA)
            {
                curTokensList = right;

                getCurToken();

                if (curToken.type == TokenType::NUMBER || curToken.type == TokenType::IDENTIFIER)
                {
                    assignmentRight.children.push_back(parseExpr());
                }

                else
                {
                    throw std::runtime_error("Unknown assignment type!");
                }

                right = empty;
                allClosed = false;
            }

            else if (token.value == FUNC_KEYWORD)
            {
                curTokensList = extractFunction();

                assignmentRight.children.push_back(parseFunction());
                allClosed = true;
            }

            else if (token.type == TokenType::OPENSQUAREBRACKET)
            {

                curTokensList = extractList();

                assignmentRight.children.push_back(parseList());
                allClosed = true;
            }

            else if (token.type == TokenType::OPENFIGUREBRACKET)
            {
                assignmentRight.children.push_back(parseTuple());
                allClosed = true;
            }

            else
            {
                right.push_back(token);
                allClosed = false;
            }


            token = getToken();
        }

        if (!allClosed)
        {

            if (right.empty())
            {
                throw std::runtime_error("var right part is not complete!");
            }

            curTokensList = right;
            Token firstToken = curTokensList.front();

            getCurToken();

            if (firstToken.type == TokenType::NUMBER || firstToken.type == TokenType::IDENTIFIER)
            {
                assignmentRight.children.push_back(parseExpr());
            }
            else
            {
                throw std::runtime_error("Unknown assignment type!");
            }
        }

        assignmentNode.children.push_back(assignmentRight);

        return assignmentNode;
    }

    AST::Node parseTuple()
    {

        Token tk = getToken();

        AST::Node tupleNode(NodeType::TUPLE);

        std::list <Token> curelement;
        std::list <Token> empty;

        
        while (!tokens.empty())
        {

            // std::cout << tk.value << " " << getTokenTypeName(tk.type) << "\n";

            if (tk.type == TokenType::CLOSEFIGUREBRACKET)
            {
                curTokensList = curelement;
                getCurToken();
                
                tupleNode.children.push_back(parseExpr());
                return tupleNode;
            }
            else if (tk.type == TokenType::COMMA)
            {
                curTokensList = curelement;
                getCurToken();
                tupleNode.children.push_back(parseExpr());
                curelement = empty;
            }

            else
            {
                curelement.push_back(tk);
            }

            tk = getToken();
        }

        throw std::runtime_error("tuple was not closed!");
    }

    AST::Node parseList()
    {
        AST::Node listNode;
        listNode.type = NodeType::LIST;
        while (!curTokensList.empty())
        {
            getCurToken();
            AST::Node newNode(NodeType::LIST_ELEMENT, curToken);
            listNode.children.push_back(newNode);
        }

        return listNode;
    }

    std::list<Token> extractList()
    {

        Token tk = getToken();

        std::list<Token> result;

        while (!tokens.empty())
        {

            if (tk.type == TokenType::CLOSESQUAREBRACKET)
            {
                return result;
            }
            else if (tk.type == TokenType::COMMA)
            {
            }

            else
            {
                result.push_back(tk);
            }

            tk = getToken();
        }

        return result;
    }

    std::list<Token> extractFunction()
    {
        int endNum = 0;
        std::list<Token> functionList;
        while (!tokens.empty())
        {
            Token tk = getToken();

            if (tk.value == IS_KEYWORD || tk.value == THEN_KEYWORD)
            {
                endNum++;
            }
            if (tk.value == END_KEYWORD)
            {
                endNum--;

                if (endNum == 0)
                {
                    return functionList;
                }
            }
            functionList.push_back(tk);
        }

        throw std::runtime_error("Not finished function!");
    }

    AST::Node getNode()
    {

        Token tk = getToken();


        // Comment - skip it in parseBody
        if (tk.type == TokenType::COMMENT)
        {
            return AST::Node(NodeType::COMMENT, tk);
        }

        // single delimiter
        if (tk.type == TokenType::DELIMITER)
        {
            return AST::Node(NodeType::COMMENT, tk);
        }

        // List
        if (tk.type == TokenType::OPENSQUAREBRACKET)
        {
            curTokensList = extractList();
            return parseList();
        }

        // Tuple
        if (tk.type == TokenType::OPENFIGUREBRACKET)
        {
            return parseTuple();
        }

        // Expression
        if (tk.type == TokenType::NUMBER || tk.type == TokenType::IDENTIFIER || tk.type == TokenType::OPENBRACKET)
        {
            while (tk.type != TokenType::DELIMITER)
            {
                curTokensList.push_back(tk);
                tk = getToken();
            }

            getCurToken();
            return parseExpr();
        }

        // Assignment
        else if (tk.type == TokenType::KEYWORD && tk.value == VAR_KEYWORD)
        {
            return parseAssignment();
        }

        // Function
        else if (tk.type == TokenType::KEYWORD && tk.value == FUNC_KEYWORD)
        {
            curTokensList = extractFunction();
            return parseFunction();
        }

        // nothing matched
        else
        {

            throw std::runtime_error("Statement did not match any pattern!");
        }
        return AST::Node();
    }

    std::vector<AST::Node> parseBody()
    {
        std::vector<AST::Node> children;
        while (!tokens.empty())
        {
            AST::Node newNode = getNode();
            if (newNode.type != NodeType::COMMENT)
                children.push_back(newNode);
        }
        return children;
    }

    void analyze()
    {
        tree.node.children = parseBody();

        printAST(tree.node);
    }
    AST::Node makeTree(NodeType nodetype, Token value, std::vector<AST::Node> children)
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

        case NodeType::FUNCTION:
            typeName = "FUNCTION";
            break;

        case NodeType::FUNCTION_BODY:
            typeName = "FUNCTION_BODY";
            break;

        case NodeType::FUNCTION_VARIABLES:
            typeName = "FUNCTION_VARIABLES";
            break;

        case NodeType::FORLOOP:
            typeName = "FORLOOP";
            break;

        case NodeType::WHILELOOP:
            typeName = "WHILELOOP";
            break;

        case NodeType::RANGE:
            typeName = "RANGE";
            break;

        case NodeType::VARS_LIST:
            typeName = "VARS_LIST";
            break;

        case NodeType::LIST:
            typeName = "LIST";
            break;

        case NodeType::LIST_ELEMENT:
            typeName = "LIST_ELEMENT";
            break;

        case NodeType::TUPLE:
            typeName = "TUPLE";
            break;

        case NodeType::TUPLE_ELEMENT:
            typeName = "TUPLE_ELEMENT";
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
