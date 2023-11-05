#include "../lexer/lexer.h"
#include "../parser/parser.h"

class Semantic
{
public:
    AST::Node node;
    std::list<Token> declarList;

    Semantic(AST::Node n);

    AST::Node analyze();

    void checkDeclar(AST::Node node);
    AST::Node optimize(AST::Node node);
    void checkTypes(AST::Node node);
    
};
