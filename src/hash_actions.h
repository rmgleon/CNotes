#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MOD_ADLER 65521
#define tam_hash 1000

// Inicializa tabla hash en NULL
void initialize_hash(list_node *hash_table[]){
	for(int i=0; i<tam_hash; i++){
		hash_table[i] = NULL;
	}
}


// Ni idea que hace esto pero algo hace
uint32_t adler32(const char *data, size_t len) {
    uint32_t a = 1, b = 0;

    for (size_t i = 0; i < len; ++i) {
        a = (a + data[i]) % MOD_ADLER;
        b = (b + a) % MOD_ADLER;
    }

    return (b << 16) | a;
}

// Comprueba que el titulo no exista en la tabla hash
int title_exists(const char *title, list_node *hash_table[]) {
    uint32_t hash = adler32(title, strlen(title));
    uint32_t index = hash % tam_hash;

    list_node *aux = hash_table[index];
    while (aux) {
        if (strcmp(aux->titulo, title) == 0) {
            return 1; // El título ya existe
        }
        aux = aux->sig;
    }
    return 0; // El título no existe
}


void add_hash_node(char *current_title, list_node *hash_table[], list_node **list){

    // Comprobar si el título ya existe
    if (title_exists(current_title, hash_table)) {
        return; // No agregar si el título ya existe
    }

    list_node *new_node = (list_node *)malloc(sizeof(list_node));
    if (!new_node) {
        perror("Error al asignar memoria\n");
        return;
    }

    new_node->titulo = strdup(current_title); // Copiar la cadena de entrada
    new_node->sig = NULL;

	uint32_t hash = adler32(current_title, strlen(current_title));
    uint32_t index = hash % tam_hash;

    new_node->sig = hash_table[index];
    hash_table[index] = new_node;

	list_node *list_node_copy = (list_node *)malloc(sizeof(list_node));
    if (!list_node_copy) {
        perror("Error al asignar memoria\n");
        return;
    }
    list_node_copy->titulo = strdup(current_title);
    list_node_copy->sig = *list;
    *list = list_node_copy;
    
}

// Función para eliminar un nodo de la tabla hash y también de la lista
void delete_hash_node(char *current_title, list_node *hash_table[], list_node **list) {
    uint32_t hash = adler32(current_title, strlen(current_title));
    uint32_t index = hash % tam_hash;

    list_node *current = hash_table[index];
    list_node *prev = NULL;

    // Buscar el nodo en la tabla hash
    while (current != NULL && strcmp(current->titulo, current_title) != 0) {
        prev = current;
        current = current->sig;
    }

    if (current == NULL) {
        // El título no se encontró en la tabla hash
        return;
    }

    // Eliminar el nodo de la tabla hash
    if (prev == NULL) {
        // El nodo a eliminar es el primero en la lista
        hash_table[index] = current->sig;
    } else {
        prev->sig = current->sig;
    }

    // Buscar el nodo en la lista enlazada
    list_node *list_current = *list;
    list_node *list_prev = NULL;

    while (list_current != NULL && strcmp(list_current->titulo, current_title) != 0) {
        list_prev = list_current;
        list_current = list_current->sig;
    }

    if (list_current == NULL) {
        // El título no se encontró en la lista enlazada
        return;
    }

    // Eliminar el nodo de la lista enlazada
    if (list_prev == NULL) {
        // El nodo a eliminar es el primero en la lista
        *list = list_current->sig;
    } else {
        list_prev->sig = list_current->sig;
    }

    // Libera memoria del nodo eliminado
    free(current->titulo);
    free(current);
    free(list_current->titulo);
    free(list_current);

    // Ahora actualizamos el archivo list.txt
    FILE *file = fopen("notes/list.txt", "w");
    if (file == NULL) {
        perror("No se pudo abrir list.txt para escritura");
        return;
    }

    // Escribir los títulos que quedan en la lista en memoria al archivo
    list_node *temp = *list;
    while (temp != NULL) {
        fprintf(file, "%s\n", temp->titulo);
        temp = temp->sig;
    }

    fclose(file);
}

void load_hash_node(list_node** hash_table, list_node **list){
    char list_file[] = "notes/list.txt";
    char current_title[MAX_TITLE_LENGTH];

    FILE *file = fopen(list_file, "r");

    if(file==NULL){
        perror("No se pudo abrir list.txt");
        return;
    }

    size_t len;
    while(fgets(current_title, MAX_TITLE_LENGTH, file)) {
        // Quita el caracter de nueva linea
        // Y lo reemplaza por un caracter 
        // terminador de string
        len = strlen(current_title);
        if(current_title[len-1] == '\n') current_title[len-1] = '\0'; // Remove newline
        add_hash_node(current_title, hash_table, list);
    }

    fclose(file);
}

void show_hash(list_node *hash_table[]) {
    for (int i = 0; i < tam_hash; ++i) {
        list_node *current = hash_table[i];
        if (current) {
            printf("Index %d:\n", i);
            while (current) {
                printf("  %s\n", current->titulo);
                current = current->sig;
            }
        }
    }
}