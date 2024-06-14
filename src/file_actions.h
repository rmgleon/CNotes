#ifndef FILE_ACTIONS_H
#define FILE_ACTIONS_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TEXT_LENGTH 16384 // 1024 * 16
#define MAX_TITLE_LENGTH 105

/* Function to save text to a file */
void save_text(char *text, char* title) {
    
    if (title == NULL) {
        printf("No title, won't save\n");
        return;
    }
    if(title[0]== '\0' || title[0]== '\n'){
        printf("Archivo vacio, no se creo");
        return;
    }

    //char actual=title[0];


    // Reemplaza los espacios por guiones bajos
    /*for(int i=0;i<MAX_TITLE_LENGTH && actual!='\0';i++, actual=title[i]){
        if(actual==' '){
            title[i]='_';
        }
    }*/

    // Crea el archivo con la extension ".txt"
    char filename[MAX_TITLE_LENGTH + 4];  // Espacio extra para ".txt"
    snprintf(filename, sizeof(filename), "%s.txt", title);

    char path[MAX_TITLE_LENGTH+20]={0};

    strcat(path,"notes/");
    strcat(path,filename);

    FILE *file = fopen(path, "w");
    if (file) {
        file = fopen(path, "w");
        fwrite(text, sizeof(char), strlen(text), file);
        fclose(file);
        printf("Text saved to %s\n", filename);
    } else {
        printf("Failed to open file for writing\n");
    }
}

// Toma un texto y un titulo, reescribe el texto al nuevo texto
// Falta devolver al buffer
int load_text(char *text, char *title) {
    char path[MAX_TITLE_LENGTH + 10];
    snprintf(path, sizeof(path), "notes/%s.txt", title);
    FILE *file = fopen(path, "r");
    if (file) {
        size_t length = fread(text, sizeof(char), MAX_TEXT_LENGTH - 1, file);
        text[length] = '\0';  // Null-terminate the string
        fclose(file);
        printf("Text loaded from %s\n", path);
    } else {
        printf("Failed to open file for reading\n");
        return 0;
    }
    return 1;
}

#endif