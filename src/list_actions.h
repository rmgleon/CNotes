#ifndef LISTS_H
#define LISTS_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// Headers nuestros
#include "file_actions.h"
#define MAX_TITLE_LENGTH 105

typedef struct list_node{
    char* titulo;
    struct list_node* sig;
} list_node;

void add_list_node(char* title, list_node** list){
    // buscar si existe con hash, todavia no se implemento
    if(title==NULL)return;
    list_node* aux=(list_node*)malloc(sizeof(list_node));
    if(*list==NULL){
        *list=aux;
        (*list)->titulo=(char*)malloc(sizeof(title) + 1);
        strcpy((*list)->titulo, title);
        (*list)->sig=NULL;
    }
    else{
        aux->sig=*list;
        *list=aux;
        (*list)->titulo=(char*)malloc(sizeof(title));
        strcpy((*list)->titulo,title);
    }
}

void load_list_nodes(list_node** list){
    char list_file[] = "notes/list.txt";
    char current_title[MAX_TITLE_LENGTH];

    FILE *file = fopen(list_file, "r");

    if(file==NULL){
        perror("No se pudo abrir list.txt");
        return;
    }

    size_t len;
    while(fgets(current_title, MAX_TITLE_LENGTH, file)) {
        // Carga los nodos linea por linea
        // Quita el caracter de nueva linea
        // Y lo reemplaza por un caracter 
        // terminador de string
        len = strlen(current_title);
        for(int i=0; i<len; i++){
            if(current_title[i]== '\n')current_title[i]='\0';
        }
        add_list_node(current_title, list);
    }

    fclose(file);
    return;
}

void save_list_nodes(list_node** list){
    char list_file[] = "notes/list.txt";
    FILE *file = fopen(list_file, "w");
    list_node* temp;

    if (file == NULL) {
        perror("No se abrio el archivo");
        return;
    }

    for(temp=*list;temp!=NULL;temp=temp->sig){
        fprintf(file, "%s\n", temp->titulo);
    }

    fclose(file);
}

void show_list_nodes(list_node** list){
    //printf("If you see this, it means this aint adding any nodes\n");
    list_node* temp;
    for(temp=*list;temp!=NULL;temp=temp->sig){
        printf("%s\n",temp->titulo);
    }

}


#endif