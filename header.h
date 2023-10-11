    //*****************************************************************
    //                          HEADER.H                   
    //*****************************************************************

/**
 * @author: Serge Eduardo Martínez Ramírez
 * @name: Algoritmo de Huffman (Compresión de Archivos)
 * @file: header.h 
 * @date: 07/05/2023
 * @authors: Sic Mundus
 * Escuela Superior de Cómputo **/


#ifndef __header__
#define __header__

// Librerías
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "tiempo.h"
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

//*****************************************************************
//                Declaración de Funciones                    
//*****************************************************************
NodoHuffman *crearNodoHuffman (unsigned char, int);
minHeap *crearHeap (int capacity);
void insertarNodo (minHeap*, NodoHuffman*);
void minHeapify (minHeap*, int);
NodoHuffman *extraerNodo (minHeap *);
NodoHuffman *construirArbolHuffman (int *frecuencias, int longitudArrFrecuencias);

/***** PROTOTIPOS FUNCIONES FICHERO.C *****/
NodoHuffman *calcularFrecuencias (char *filename, int *longitud, long *cantidadBytesArchivo);
int *recuperarFrecuencias (char *filename, int *longitud);

/***** Árbol de Huffman (huffman.c) *****/
int calcular_altura(NodoHuffman *root);

/***** Funciones para generar tabla de códigos ******/
TablaCodigo *generarTablaCodigos (NodoHuffman *root);
void generarCodigosHuffman (NodoHuffman *root, unsigned char *codigo, TablaCodigo *tabla, int i);
void imprimirCodigos (TablaCodigo *tablaCodigos, int tam);

/***** Codificación y decodificación *****/
void codificarArchivo (char *inputFile, TablaCodigo *tabla, int tam, long *bytesCodificado);
void decodificar (char *nombre_codificado, char *name_decodificado, NodoHuffman *raiz);
unsigned char invertirIzqDer(unsigned char byte);

/***** Algoritmo de Búsqueda ******/
int buscarCodigoLineal (TablaCodigo *tabla, int tam, unsigned char byte);

#endif 