#include "../../include/semantic/semantic.h"
#include "../../include/parser/parser.h"

void Semantic::checkDeclar(AST::Node node)
{

    if (node.type == NodeType::DECLARATION)
    {
        for (auto child : node.children)
        {
            if (child.type == NodeType::DECLARATION_LEFT)
            {
                for (auto grandchild : child.children)
                {
                    if (grandchild.type == NodeType::IDENTIFIER)
                    {
                        declarList.push_back(grandchild.value);
                    }
                    else
                    {
                        throw std::runtime_error("Semantic error: unexpected value at definition!");
                    }
                }
                declarList.push_back(child.value);

                break;
            }
        }
        declarList.push_back(node.value);
    }

    if (node.type == NodeType::IDENTIFIER)
    {
        if (std::find(declarList.begin(), declarList.end(), node.value) == declarList.end())
        {
            throw std::runtime_error("Semantic error: undeclared variable!");
        }
    }

    for (auto child : node.children)
    {
        checkDeclar(child);
    }
}