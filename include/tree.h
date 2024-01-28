#ifndef TREE_H
#define TREE_H

#include <wchar.h>

typedef enum
{
    DIGIT = 1,
    OP = 2,
    VAR = 3,
} type_t;

#define DEF_OP(NAME, op_n, op_name) NAME = op_n,
typedef enum
{
    NONE = 0,
#include "def_op.txt"
} op_t;
#undef DEF_OP

typedef struct
{
    type_t type;
    union
    {
        op_t op;
        double val;
        wchar_t *var;
    };
} node_val;

struct tree_node
{
    node_val val;
    struct tree_node *left;
    struct tree_node *right;

};
typedef struct tree_node tree_node;

tree_node *Node(node_val val, tree_node *left, tree_node *right);

void Del_tree(tree_node *node);

#endif

