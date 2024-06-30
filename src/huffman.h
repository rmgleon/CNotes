#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>

#define MAX_TREE_HT 256

// Estructura para un nodo del árbol de Huffman
struct MinHeapNode {
    char data;
    unsigned freq;
    struct MinHeapNode *left, *right;
};

// Estructura para la cola de prioridad (Min Heap)
struct MinHeap {
    unsigned size;
    unsigned capacity;
    struct MinHeapNode** array;
};

// Crear un nuevo nodo del árbol de Huffman
struct MinHeapNode* newNode(char data, unsigned freq) {
    struct MinHeapNode* temp = (struct MinHeapNode*) malloc(sizeof(struct MinHeapNode));
    temp->left = temp->right = NULL;
    temp->data = data;
    temp->freq = freq;
    return temp;
}

// Crear una cola de prioridad (Min Heap) de una capacidad dada
struct MinHeap* createMinHeap(unsigned capacity) {
    struct MinHeap* minHeap = (struct MinHeap*) malloc(sizeof(struct MinHeap));
    minHeap->size = 0;
    minHeap->capacity = capacity;
    minHeap->array = (struct MinHeapNode**) malloc(minHeap->capacity * sizeof(struct MinHeapNode*));
    return minHeap;
}

// Intercambiar dos nodos de la cola de prioridad
void swapMinHeapNode(struct MinHeapNode** a, struct MinHeapNode** b) {
    struct MinHeapNode* t = *a;
    *a = *b;
    *b = t;
}

// Ordenar la cola de prioridad
void minHeapify(struct MinHeap* minHeap, int idx) {
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;

    if (left < minHeap->size && minHeap->array[left]->freq < minHeap->array[smallest]->freq)
        smallest = left;

    if (right < minHeap->size && minHeap->array[right]->freq < minHeap->array[smallest]->freq)
        smallest = right;

    if (smallest != idx) {
        swapMinHeapNode(&minHeap->array[smallest], &minHeap->array[idx]);
        minHeapify(minHeap, smallest);
    }
}

// Verificar si el tamaño de la cola de prioridad es 1
int isSizeOne(struct MinHeap* minHeap) {
    return (minHeap->size == 1);
}

// Extraer el nodo con la frecuencia mínima
struct MinHeapNode* extractMin(struct MinHeap* minHeap) {
    struct MinHeapNode* temp = minHeap->array[0];
    minHeap->array[0] = minHeap->array[minHeap->size - 1];
    --minHeap->size;
    minHeapify(minHeap, 0);
    return temp;
}

// Insertar un nodo en la cola de prioridad
void insertMinHeap(struct MinHeap* minHeap, struct MinHeapNode* minHeapNode) {
    ++minHeap->size;
    int i = minHeap->size - 1;

    while (i && minHeapNode->freq < minHeap->array[(i - 1) / 2]->freq) {
        minHeap->array[i] = minHeap->array[(i - 1) / 2];
        i = (i - 1) / 2;
    }
    minHeap->array[i] = minHeapNode;
}

// Construir la cola de prioridad y ordenar los nodos
void buildMinHeap(struct MinHeap* minHeap) {
    int n = minHeap->size - 1;
    int i;

    for (i = (n - 1) / 2; i >= 0; --i)
        minHeapify(minHeap, i);
}

// Verificar si el nodo es una hoja
int isLeaf(struct MinHeapNode* root) {
    return !(root->left) && !(root->right);
}

// Crear y construir la cola de prioridad para el árbol de Huffman
struct MinHeap* createAndBuildMinHeap(char data[], int freq[], int size) {
    struct MinHeap* minHeap = createMinHeap(size);

    for (int i = 0; i < size; ++i)
        minHeap->array[i] = newNode(data[i], freq[i]);

    minHeap->size = size;
    buildMinHeap(minHeap);
    return minHeap;
}

// Construir el árbol de Huffman
struct MinHeapNode* buildHuffmanTree(char data[], int freq[], int size) {
    struct MinHeapNode *left, *right, *top;
    struct MinHeap* minHeap = createAndBuildMinHeap(data, freq, size);

    while (!isSizeOne(minHeap)) {
        left = extractMin(minHeap);
        right = extractMin(minHeap);
        top = newNode('$', left->freq + right->freq);
        top->left = left;
        top->right = right;
        insertMinHeap(minHeap, top);
    }
    return extractMin(minHeap);
}

// Guardar los códigos de Huffman en un array
void storeCodes(struct MinHeapNode* root, int arr[], int top, char* codes[], int codeLengths[]) {
    if (root->left) {
        arr[top] = 0;
        storeCodes(root->left, arr, top + 1, codes, codeLengths);
    }

    if (root->right) {
        arr[top] = 1;
        storeCodes(root->right, arr, top + 1, codes, codeLengths);
    }

    if (isLeaf(root)) {
        codes[(unsigned char)root->data] = malloc(top + 1);
        for (int i = 0; i < top; i++) {
            codes[(unsigned char)root->data][i] = arr[i] + '0';
        }
        codes[(unsigned char)root->data][top] = '\0';
        codeLengths[(unsigned char)root->data] = top;
    }
}

// Función principal para construir el árbol de Huffman y generar los códigos
void HuffmanCodes(char data[], int freq[], int size, char* codes[], int codeLengths[]) {
    struct MinHeapNode* root = buildHuffmanTree(data, freq, size);
    int arr[MAX_TREE_HT], top = 0;
    storeCodes(root, arr, top, codes, codeLengths);
}

// Función para contar la frecuencia de los caracteres en un texto
void countFrequency(const char* text, int freq[]) {
    // Inicializar el array de frecuencias a 0
    for (int i = 0; i < 256; ++i) {
        freq[i] = 0;
    }

    // Contar la frecuencia de cada carácter en el texto
    for (int i = 0; text[i] != '\0'; ++i) {
        freq[(unsigned char)text[i]]++;
    }
}


// Función para escribir la tabla de códigos en el archivo comprimido
void writeCodesTable(FILE *file, char* codes[], int codeLengths[]) {
    for (int i = 0; i < 256; i++) {
        if (codeLengths[i] > 0) {
            fputc(i, file);
            fputc(codeLengths[i], file);
            for (int j = 0; j < codeLengths[i]; j++) {
                fputc(codes[i][j], file);
            }
        }
    }
    fputc('\0', file);  // Fin de la tabla de códigos
}

// Función para leer la tabla de códigos del archivo comprimido
void readCodesTable(FILE *file, struct MinHeapNode* root) {
    unsigned char ch;
    while ((ch = fgetc(file)) != '\0') {
        int length = fgetc(file);
        struct MinHeapNode* currentNode = root;
        for (int i = 0; i < length; i++) {
            int bit = fgetc(file) - '0';
            if (bit == 0) {
                if (!currentNode->left) {
                    currentNode->left = newNode('$', 0);
                }
                currentNode = currentNode->left;
            } else {
                if (!currentNode->right) {
                    currentNode->right = newNode('$', 0);
                }
                currentNode = currentNode->right;
            }
        }
        currentNode->data = ch;
    }
}

// Función para comprimir un buffer de texto y guardarlo en un archivo
void compressBuffer(const char* text, const char* title) {
    char outputFilename[256];
    snprintf(outputFilename, sizeof(outputFilename), "notes/%s.hff", title);

    int freq[256] = {0};
    countFrequency(text, freq);

    char data[256];
    int nonZeroFreq[256];
    int size = 0;

    for (int i = 0; i < 256; ++i) {
        if (freq[i]) {
            data[size] = (char)i;
            nonZeroFreq[size] = freq[i];
            size++;
        }
    }

    char* codes[256];
    int codeLengths[256] = {0};

    HuffmanCodes(data, nonZeroFreq, size, codes, codeLengths);

    FILE *outputFile = fopen(outputFilename, "wb");
    if (!outputFile) {
        fprintf(stderr, "Error abriendo archivo %s.\n", outputFilename);
        exit(EXIT_FAILURE);
    }

    // Escribir la tabla de códigos en el archivo comprimido
    writeCodesTable(outputFile, codes, codeLengths);

    unsigned char ch;
    int buffer = 0, bufferSize = 0;

    for (int i = 0; text[i] != '\0'; ++i) {
        ch = text[i];
        char* code = codes[ch];
        for (int j = 0; j < codeLengths[ch]; j++) {
            buffer = (buffer << 1) | (code[j] - '0');
            bufferSize++;

            if (bufferSize == 8) {
                fputc(buffer, outputFile);
                bufferSize = 0;
                buffer = 0;
            }
        }
    }

    if (bufferSize > 0) {
        buffer = buffer << (8 - bufferSize);
        fputc(buffer, outputFile);
    }

    fclose(outputFile);
}

// Función para descomprimir un archivo y llenar un buffer de texto
int decompressBuffer(char* text, const char* title) {
    char inputFilename[256];
    snprintf(inputFilename, sizeof(inputFilename), "notes/%s.hff", title);

    FILE *inputFile = fopen(inputFilename, "rb");
    if (!inputFile) {
        fprintf(stderr, "Error abriendo archivo %s.\n", inputFilename);
        return 0;
    }

    struct MinHeapNode* root = newNode('$', 0);
    readCodesTable(inputFile, root);

    struct MinHeapNode* currentNode = root;
    unsigned char buffer;
    int bit;
    int pos = 0;

    while (fread(&buffer, 1, 1, inputFile) == 1) {
        for (int i = 7; i >= 0; i--) {
            bit = (buffer >> i) & 1;
            if (bit == 0) {
                currentNode = currentNode->left;
            } else {
                currentNode = currentNode->right;
            }

            if (isLeaf(currentNode)) {
                text[pos++] = currentNode->data;
                currentNode = root;
            }
        }
    }

    text[pos] = '\0';  // Final del texto descomprimido
    fclose(inputFile);
    return 1;
}