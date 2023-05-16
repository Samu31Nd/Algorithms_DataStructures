/**
 * @author: Serge Eduardo Martínez Ramírez
 * @name: Algoritmo de Huffman (Compresión de Archivos)
 * @date: 07/05/2023
*/

#include "header.h"

int main(int argc, char *argv[]) {

    /* Se manda como parámetro el nombre del archivo a codificar, se calculan
    las frecuencias, se construyen el montículo mínimo y se retorna el árbol de Huffman*/
    int longitud = 0;
    NodoHuffman *root = calcularFrecuencias ("Holaa", &longitud);
    printf("Recorrido en Inorden del arbol de Huffman\n");
    inorden(root);  
    TablaCodigo *tabla_Codigos = generarTablaCodigos (root, longitud);
    imprimirCodigos (tabla_Codigos, longitud);
    
    system("pause");
    return 0;
}