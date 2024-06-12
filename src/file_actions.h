#ifndef FILE_ACTIONS_H
#define FILE_ACTIONS_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TEXT_LENGTH 100
#define MAX_TITLE_LENGTH 105

/* Function to save text to a file */
void save_text(char *text, char* title) {
    system("mkdir notes");
    if (title == NULL) {
        printf("No title, won't save\n");
        return;
    }
    if(title[0]== '\0' || title[0]== '\n'){
        printf("Archivo vacio, no se creo");
        return;
    }

    char actual=title[0];


    // Create the filename with ".txt" extension
    char filename[MAX_TITLE_LENGTH + 4];  // Extra space for ".txt" and null terminator
    snprintf(filename, sizeof(filename), "%s.txt", title);


    char path[MAX_TITLE_LENGTH+20]={0};

    strcat(path,"notes/");
    strcat(path,filename);

    FILE *file = fopen(path, "w");
    if (file) {
        fwrite(text, sizeof(char), strlen(text), file);
        fclose(file);
        printf("Text saved to %s\n", filename);
    } else {
        printf("Failed to open file for writing\n");
    }
}

/* Function to load text from a file */
void load_text(char *text) {

    char path[MAX_TITLE_LENGTH+20]={0};

    strcat(path,"notes/");
    strcat(path,text);
    
    FILE *file = fopen(path, "r");
    if (file) {
        size_t length = fread(text, sizeof(char), MAX_TEXT_LENGTH - 1, file);
        text[length] = '\0';  // Null-terminate the string
        fclose(file);
        printf("Text loaded from %s\n", path);
    } else {
        printf("Failed to open file for reading\n");
    }
}
#endif