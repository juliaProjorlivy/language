#include <stdio.h>
#include <wchar.h>
#include "tree.h"
#include "parser.h"
#include "dump.h"
#include "verror.h"

#define DEF_OP(NAME, op_n, op_name)\
,L##op_name

static wchar_t ops[][max_len] = {
L" "
#include "def_op.txt"
};
#undef DEF_OP

void node_dump(FILE *file, tree_node *node)
{
    if(!node)
    {
        return;
    }
    fwprintf(file, L"\tnode[color=\"blue\", frontsize=14, shape=\"rectangle\", style=\"rounded\", fillcolor=\"lightblue\"];\n");
    fwprintf(file, L"\tedge[color=\"deepskyblue\", fontsize=12;\n]");
    switch(node->val.type)
    {
        case OP:
            {
                fwprintf(file, L"\tnode_%p[shape=record, label=\"{%ls\\n | {<f0> left | <f1> right}}\", style=\"filled\", fillcolor=\"lightblue\"];\n", node, ops[node->val.op]);
                break;
            }
        case DIGIT:
            {
                fwprintf(file,  L"\tnode_%p[shape=record, label=\"{%lf\\n | {<f0> left | <f1> right}}\", style=\"filled\", fillcolor=\"lightgreen\"];\n", node, node->val.val);
                break;
            }
        case VAR:
            {
                fwprintf(file,  L"\tnode_%p[shape=record, label=\"{%ls\\n | {<f0> left | <f1> right}}\", style=\"filled\", fillcolor=\"lightpink\"];\n", node, node->val.var);
                break;
            }
        default:
            {
                break;
            }
    }
    if(node->left)
    {
        fwprintf(file, L"\tnode_%p:<f0>->node_%p;\n", node, node->left);
        node_dump(file, node->left);
    }
    else
    {
        fwprintf(file, L"\tL_NULL_%p[label=\"NULL\", color=\"red\", style=\"filled\", fillcolor=\"tomato\"];\n", node);
        fwprintf(file, L"\tnode_%p:<f0>->L_NULL_%p;\n", node, node);
    }
    if(node->right)
    {
        fwprintf(file, L"\tnode_%p:<f1>->node_%p;\n", node, node->right);
        node_dump(file, node->right);
    }
    else
    {
        fwprintf(file, L"\tL_NULL_%p[label=\"NULL\", color=\"red\", style=\"filled\", fillcolor=\"tomato\"];\n", node);
        fwprintf(file, L"\tnode_%p:<f1>->L_NULL_%p;\n", node, node);
    }
    
    return;
}

int tree_dump(tree_node *node, const char *filename)
{
    FILE *file = fopen(filename, "w");
    if(!file)
    {
        VERROR_FOPEN(filename);
        return 1;
    }
    
    if(fwprintf(file, L"digraph tree{\n\trankdir=TB;\n") < 0)
    {
        VERROR_FWRITE(filename);
        return 1;
    }

    node_dump(file, node);

    if(fwprintf(file, L"}\n") < 0)
    {
        VERROR_FWRITE(filename);
        return 1;
    }

    if(fclose(file))
    {
        VERROR_FCLOSE(filename);
        return 1;
    }

    return 0;
}


