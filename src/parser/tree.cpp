#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include "tree.h"
#include "verror.h"



tree_node *Node(node_val val, tree_node *left, tree_node *right)
{
  tree_node *node = (tree_node *)calloc(sizeof(tree_node), 1);
  if(!node)
  {
    VERROR_MEM;
    return NULL;
  }

  node->left = left;
  node->right = right;

  node->val = val;
  if(val.type == VAR)
  {
    (node->val).var = wcsdup(val.var);
  }

  return node;
}

void Del_tree(tree_node *node)
{
  if(!node)
  {
    return;
  }
  if(node->left)
  {
    Del_tree(node->left);
  }
  if(node->right)
  {
    Del_tree(node->right);
  }
  
  if(node->val.type == VAR)
  {
    free(node->val.var);
  }
  free(node);
}

