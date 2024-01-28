#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include "tree.h"
#include "parser.h"
#include "verror.h"

struct tree_node *getE(Lexems *lexems)
{
    struct tree_node *val = getT(lexems);

    if(!val)
    {
        return NULL;
    }

    while(lexems->lexems[lexems->p].type == OP && (lexems->lexems[lexems->p].op == ADD || lexems->lexems[lexems->p].op == SUB))
    {
        int old_p = lexems->p;
        op_t op = lexems->lexems[lexems->p].op;
        (lexems->p)++;
        struct tree_node *val2 = getT(lexems);

        if(!val2)
        {
            Del_tree(val);
            return NULL;
        }
        
        struct tree_node *tmp_val = Node({.type = OP, .op = op}, val, val2);
        if(!tmp_val)
        {
            Del_tree(val);
            Del_tree(val2);
            VERROR_MEM;
            return NULL;
        }

        val = tmp_val;
    }
    return val;
}


struct tree_node *getPow(Lexems *lexems)
{
    struct tree_node *val = getP(lexems);

    if(!val)
    {
        return NULL;
    }

    while(lexems->lexems[lexems->p].type == OP && lexems->lexems[lexems->p].op == POW)
    {
        (lexems->p)++;
        struct tree_node *val2 = getP(lexems);

        if(!val2)
        {
            Del_tree(val);
            return NULL;
        }

        struct tree_node *tmp_val = Node({.type = OP, .op = POW}, val, val2);
        if(!tmp_val)
        {
            Del_tree(val);
            Del_tree(val2);
            VERROR_MEM;
            return NULL;
        }
        val = tmp_val;
    }
    return val;
}

struct tree_node *getT(Lexems *lexems)
{
    struct tree_node *val = getPow(lexems);

    if(!val)
    {
        return NULL;
    }

    while(lexems->lexems[lexems->p].type == OP && (lexems->lexems[lexems->p].op == MUL || lexems->lexems[lexems->p].op == DIV))
    {
        int old_p = lexems->p;
        op_t op = lexems->lexems[lexems->p].op;
        (lexems->p)++;
        struct tree_node *val2 = getPow(lexems);

        if(!val2)
        {
            Del_tree(val);
            return NULL;
        }

        struct tree_node *tmp_val = Node({.type = OP, .op = op}, val, val2);
        if(!tmp_val)
        {
            VERROR_MEM;
            Del_tree(val);
            Del_tree(val2);
            return NULL;
        }
        val = tmp_val;
    }

    return val;
}


struct tree_node *getP(Lexems *lexems)
{
    if(lexems->lexems[lexems->p].type == OP && lexems->lexems[lexems->p].op == OPB)
    {
        (lexems->p)++;
        struct tree_node *val = getE(lexems);

        if(!val)
        {
            return NULL;
        }

        if(lexems->lexems[lexems->p].type != OP && lexems->lexems[lexems->p].op != CLB)
        {
            VERROR("syntax error\nexpected \")\"");
            Del_tree(val);
            return NULL;
        }
        (lexems->p)++;
        return val;
    }
    else if(lexems->lexems[lexems->p].type == VAR)
    {
        struct tree_node *val = Node(lexems->lexems[lexems->p], NULL, NULL);

        if(!val)
        {
            return NULL;
        }

        if(lexems->lexems[lexems->p].type == OP && lexems->lexems[lexems->p].op == OPB)
        {
            (lexems->p)++;
            struct tree_node *val2 = getE(lexems);

            if(!val2)
            {
                Del_tree(val);
                return NULL;
            }

            if(lexems->lexems[lexems->p].type != OP || lexems->lexems[lexems->p].op != CLB)
            {
                VERROR("syntax error\nexpected \")\"");
                Del_tree(val);
                Del_tree(val2);
                return NULL;
            }
            (lexems->p)++;
            val->right = val2;
            return val;
        }
        return val;
    }
    

    return getN(lexems);
}

struct tree_node *getN(Lexems *lexems)
{
    struct tree_node *val = Node(lexems->lexems[lexems->p], NULL, NULL);

    if(!val)
    {
        VERROR_MEM;
        return NULL;
    }

    return val;
}


struct tree_node *getG(node_val *lexems)
{
    Lexems lexs = {.lexems = lexems, .p = 0};

    struct tree_node *res = getE(&lexs);

    /* if((lexems.lexems[lexems.p]).op != END) */
    /* { */
    /*     VERROR("syntax error\nexpected '\\0'"); */
    /*     Del_tree(res); */
    /*     return NULL; */
    /* } */
    return res;
}
#undef DEF_KEY

