#ifndef __header__
#define __header__

// Librerías
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "bits.h"
// Estructura de un nodo de árbol de Huffman
typedef struct nodoHuffman {
    unsigned char dato;
    unsigned int frecuencia;
    struct nodoHuffman *left, *right;
}   NodoHuffman;

// Estructura de un Heap
typedef struct heap {
    int size;
    int capacity;
    NodoHuffman **arreglo;
}   minHeap;

// Estructura de Tabla de Códigos Huffman
typedef struct tabla {
    unsigned char byte;
    unsigned char *codigo;
    int longitud;
}   TablaCodigo;

/**** PROTOTIPOS FUNCIONES MINHEAP.C (minHeap.c) ****/
NodoHuffman *crearNodoHuffman (unsigned char, int);
minHeap *crearHeap (int capacity);
void insertarNodo (minHeap*, NodoHuffman*);
void minHeapify (minHeap*, int);
NodoHuffman *extraerNodo (minHeap*);
NodoHuffman *construirArbolHuffman (int *, int);

//Mostrar el Heap para ver las frecuencias
void mostrarHeap (minHeap *, int);

/***** PROTOTIPOS FUNCIONES FICHERO.C *****/
NodoHuffman *calcularFrecuencias (char *filename, int *longitud);
void guardarTabla (TablaCodigo *tabla, int tam);
TablaCodigo *recuperarTabla (char *tableName, int *tam_Tabla);
/***** Árbol de Huffman (huffman.c) *****/
void inorden(NodoHuffman *root);
int calcular_altura(NodoHuffman *root);
TablaCodigo *generarTablaCodigos (NodoHuffman *root, int tam);
void generarCodigosHuffman (NodoHuffman *root, unsigned char *codigo, TablaCodigo *tabla, int i, int *indice);
void imprimirCodigos (TablaCodigo *tablaCodigos, int tam);
void codificarArchivo (char *inputFile, TablaCodigo *tabla, int tam);

/***** Algoritmo de Búsqueda Binaria ******/
int buscarCodigo (TablaCodigo *tabla, int inicio, int final, unsigned char byte);
int buscarCodigoLineal (TablaCodigo *tabla, int tam, unsigned char byte);

#endif 