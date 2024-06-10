#ifndef LISTS_H
#define LISTS_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TEXT_LENGTH 100
#define MAX_TITLE_LENGTH 105

typedef struct list_node{
    char* titulo;
    list_node* sig;
}



#endif