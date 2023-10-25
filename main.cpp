#include "include/lexer/lexer.h"
#include "include/parser/parser.h"

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
    parser.analyze();

    return 0;
}
