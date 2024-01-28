#include <cstddef>
#include <stdio.h>
#include <wchar.h>
#include <string.h>
#include <stdlib.h>
#include "tree.h"
#include "verror.h"

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

#define DEF_OP(NAME, op_n, op_name)                             \
RetVal get##NAME(Data *data, Lexems *lexems)                    \
{                                                               \
    wchar_t name[] = L##op_name;                                \
    wprintf(L"p = %d, len = %d, " #NAME, data->p, data->len);\
    if(!wcsncmp(data->prog + data->p, name, wcslen(name)))      \
    {                                                           \
        lexems->lexems[lexems->p] = {.type = OP, .op = NAME};   \
        (lexems->p)++;                                          \
        (data->p) += data->len;                                 \
        (data->len) = 1;                                        \
        wprintf(L" STOP\n");\
        return STOP;                                            \
    }                                                           \
    wprintf(L" NEXT\n");\
    return NEXT;                                                \
}
#include "def_op.txt"
#undef DEF_OP

int skip_spaces(Data *data)
{
    int counter = 0;
    while(data->prog[data->p] == L' ')
    {
        (data->p)++;
        counter++;
    }
    return counter;
}

int is_unar_minus(Data *data, Lexems *lexems)
{
    if(lexems->p == 0)
    {
        (data->p)++;
        return 1;
    }
    
    node_val prev = lexems->lexems[lexems->p - 1];
    wchar_t next = data->prog[data->p + 1];
    
    if(L'0' <= next && next <= L'9')
    {
        if(prev.type == OP)
        {
            if((prev.op == ADD || prev.op == SUB  || prev.op == MUL || prev.op == DIV ||
                prev.op == OPB || prev.op == OPCB))
            {
                (data->p)++;
                (data->len)++;
                return 1;
            }
        }
    }

    return 0;
}

RetVal getDigit(Data *data, Lexems *lexems)
{
    int dot = 0;
    double mult = 1;
    int minus = 1;
    double val = 0;

    int old_p = data->p;

    if(data->prog[data->p] == L'-')
    {
        if(is_unar_minus(data, lexems))
        {
            minus = -1;
        }
    }
    
    if(data->len != 1)
    {
        return NEXT;
    }
    while((L'0' <= data->prog[data->p] && data->prog[data->p] <= L'9') || data->prog[data->p] == L'.')
    {
        if(data->prog[data->p] == L'.')
        {
            if(dot)
            {
                VERROR("syntax error in %d position\nexpected space character or a digit", data->p);
                return SYNTAX;
            }
            dot = 1;
            (data->p)++;
        }
        if(dot)
        {
            mult *= 0.1;
            val = val + mult *(data->prog[data->p] - L'0');
        }
        else
        {
            val = val * 10 + (data->prog[data->p] - L'0');
        }
        (data->p)++;
    }
    
    val *= minus;

    if(old_p == data->p)
    {
        VERROR("syntax error in %d position\nexpected a digit", data->p);
        return SYNTAX;
    }

    lexems->lexems[lexems->p] = {.type = DIGIT, .val = val};
    (lexems->p)++;
    return STOP;
}

#define DEF_OP(NAME, op_n, op_name)\
get##NAME,

node_val *parse_to_lexems(wchar_t *prog)
{
    Data data = {.prog = prog, .p = 0, .len = 1};

    node_val *lexems = (node_val *)calloc(sizeof(node_val), wcslen(prog));
    if(!lexems)
    {
        VERROR_MEM;
        return NULL;
    }
    Lexems lexs = {.lexems = lexems, .p = 0};

    RetVal (*funcs[])(Data *data, Lexems *lexems) = {
#include "def_op.txt"
getDigit
    };

    size_t Nfuncs = sizeof(funcs) / sizeof(funcs[0]);
    RetVal ret = NEXT;
    
    while(data.prog[data.p] != L'\0')
    {
        for(size_t i_func = 0; i_func < Nfuncs; i_func++)
        {
            ret = funcs[i_func](&data, &lexs);
            if(ret == SYNTAX)
            {
                VERROR("error");
                free(lexems);
                return NULL;
            }
            if(ret == STOP)
            {
                data.len = 1;
                break;
            }
        }
        int sk_p = skip_spaces(&data);
        if(ret == NEXT && sk_p)
        {
            wchar_t *var = (wchar_t *)calloc(sizeof(wchar_t), data.len);
            if(!var)
            {
                VERROR_MEM;
                return NULL;
            }
            var = wcsncpy(var, data.prog + data.p, data.len);
            if(!var)
            {
                VERROR_MEM;
                return NULL;
            }
            lexs.lexems[lexs.p] = {.type = VAR, .var = var};
            (lexs.p)++;
            data.len = 1;
        }
        else if(ret == NEXT)
        {
            data.len++;
        }
    }
    
    lexems = (node_val *)realloc(lexems, sizeof(node_val) * lexs.p);
    if(!lexems)
    {
        VERROR_MEM;
        return NULL;
    }
    return lexems;
}

#undef DEF_OP

