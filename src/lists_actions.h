#ifndef LISTS_H
#define LISTS_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// Headers nuestros
#include <file_actions.h>
#define MAX_TITLE_LENGTH 105

typedef struct list_node{
    char* titulo;
    list_node* sig;
} list_node;

void load_list_nodes(char* title, list_node* list){
    char text[1024 * 16] = {0};
    calloc(*text, MAX_TITLE_LENGTH);
    load_text(text, title);
}


#endif