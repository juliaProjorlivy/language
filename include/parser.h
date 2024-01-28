#ifndef PARSER_H
#define PARSER_H

#include "tree.h"

typedef enum
{
    STOP = 0,
    NEXT = 1,
    SYNTAX = 2,
} RetVal;

typedef struct
{
    wchar_t *prog;
    int p;
    int len;
} Data;

typedef struct
{
    node_val *lexems;
    int p;
} Lexems;

#define DEF_OP(NAME, op_n, op_name)         \
RetVal get##NAME(Data *data, Lexems *lexems) ;
#include "def_op.txt"
#undef DEF_OP

int skip_spaces(Data *data);

int is_unar_minus(Data *data, Lexems *lexems);

RetVal getDigit(Data *data, Lexems *lexems);

node_val *parse_to_lexems(wchar_t *prog);



struct tree_node *getG(node_val *lexs);

struct tree_node *getN(Lexems *lexems);

struct tree_node *getID(Lexems *lexems);

struct tree_node *getP(Lexems *lexems);

struct tree_node *getT(Lexems *lexems);

struct tree_node *getPow(Lexems *lexems);

struct tree_node *getE(Lexems *lexems);

static const int max_len = 100;
#endif

