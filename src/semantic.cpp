#include "../../include/semantic/semantic.h"
#include "../../include/parser/parser.h"

Semantic::Semantic(AST::Node node)
    : node(node)
{
}

AST::Node Semantic::analyze()
{
    AST::Node newNode = node;

    return newNode;
}


