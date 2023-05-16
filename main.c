/**
 * @author: Serge Eduardo Martínez Ramírez
 * @name: Algoritmo de Huffman (Compresión de Archivos)
 * @date: 07/05/2023
*/

#include "header.h"

int main(int argc, char *argv[]) {

    /* Se manda como parámetro el nombre del archivo a codificar, se calculan
    las frecuencias, se construyen el montículo mínimo y se retorna el árbol de Huffman*/
    int tam = 0;
    NodoHuffman *root = calcularFrecuencias ("Holaa", &tam);
    /* Se determina la tabla de bytes con su código correspondiente */
    TablaCodigo *tabla_Codigos = generarTablaCodigos (root, tam);
    imprimirCodigos (tabla_Codigos, tam);
    codificarArchivo ("archivo.txt", tabla_Codigos, tam);
    system("pause");
    return 0;
}