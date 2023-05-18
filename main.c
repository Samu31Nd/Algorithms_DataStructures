/**
 * @author: Serge Eduardo Martínez Ramírez
 * @name: Algoritmo de Huffman (Compresión de Archivos)
 * @date: 07/05/2023
*/

#include "header.h"

/* Función Main */
int main(int argc, char *argv[]) {
    int tam = 0;
    if (argc != 2 && argc != 3) {
        printf("INSTRUCCIONES PARA LA EJECUCION DEL PROGRAMA\n\n");
        printf("Comprimir Archivo:\n");
        printf("./Huffman <nombreArchivo.extension>\n\n");
        printf("Descomprimir Archivo\n");
        printf("./Huffman <codificacion.dat> <Frecuencias.txt>\n");
        exit(EXIT_SUCCESS);
    }

    if (argc == 2) {
        long cantidadBytes = 0;
        long BytesCodificado = 0;
        system("cls");
        printf("\tALGORITMO DE HUFFMAN\n");
        printf("COMPRIMIENDO ARCHIVO '%s'...\n", argv[1]);
        /* Se manda como parámetro el nombre del archivo a codificar, se calculan
        las frecuencias, se construyen el montículo mínimo y se retorna el árbol de Huffman*/
        NodoHuffman *root = calcularFrecuencias (argv[1], &tam, &cantidadBytes);
        TablaCodigo *tabla_Codigos = generarTablaCodigos (root);
        imprimirCodigos (tabla_Codigos, 256);
        codificarArchivo (argv[1], tabla_Codigos, 256, &BytesCodificado);
        printf("Cantidad de Bytes de %s: %ld.\n", argv[1], cantidadBytes);
        printf("Cantidad de Bytes del archivo codificado: %ld\n", BytesCodificado);
    }

    else if (argc == 3) {
        int longitudArrFrecuencias = 0;
        system("cls");
        printf("\tALGORITMO DE HUFFMAN\n");
        printf("Ingrese nombre y extension correspondiente del archivo de salida:\n");
        printf("DESCOMPRIMIENDO ARCHIVO '%s'...\n", argv[1]);
        /* Dado el archivo de frecuencias, se recupera el arreglo del archivo para
        construir el árbol de Huffman */
        int *frecuencias = recuperarFrecuencias (argv[2], &longitudArrFrecuencias);
        NodoHuffman *root = construirArbolHuffman (frecuencias, longitudArrFrecuencias);
        TablaCodigo *tabla_Codigos = generarTablaCodigos (root);
        decodificar (argv[1], "Descomprimido.pdf", root);
    }

    system ("pause");
    return 0;
}