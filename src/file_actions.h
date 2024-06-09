#ifndef FILE_ACTIONS_H
#define FILE_ACTIONS_H

typedef struct tree{
    int dato;
    struct tree* left;
    struct tree* right;
}tree;

tree *createTreeNode (int val);
void addTreeNode (tree **arbol, int val);
void showTree (tree *arbol);
tree *findTreeNode (tree *arbol, int bus);
void shuffle(int *arreglo, int n);

#endif /* ARBOL_BINARIO_H */