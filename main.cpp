#include "include/lexer/lexer.h"
#include "include/parser/parser.h"
#include "include/semantic/semantic.h"

int main(int argc, char **argv)
{

    if (argc != 2)
    {
        std::cerr << "ERROR: ";
        return 1;
    }

    Lexer lexer(argv[1]);

    std::list<Token> tokens = lexer.analyze();

    lexer.printTokens(tokens);

    Parser parser(tokens);
    AST::Node rootNode = parser.analyze();

    // Semantic semantic(rootNode);
    // AST::Node updNode = semantic.analyze();
    
    return 0;
}
