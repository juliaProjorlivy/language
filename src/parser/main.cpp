#include <stdio.h>
#include <locale.h>
#include <wchar.h>
#include <stdlib.h>
#include "tree.h"
#include "parser.h"
#include "dump.h"

int main()
{
    setlocale(LC_ALL, "");
    wchar_t *line = (wchar_t *)calloc(sizeof(wchar_t), max_len);
    wscanf(L"%L[^\n]", line);
    wprintf(L"%S\n", line);
    int n_lexs = 0;
    node_val *lexes = parse_to_lexems(line);
    tree_node *node = getG(lexes); tree_dump(node, "check_tree.dot"); 
    Del_tree(node);
    free(lexes);
    free(line);
    return 0;
}

