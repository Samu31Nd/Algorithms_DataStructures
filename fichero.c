/**
 * fichero.c:
 * Archivo que contiene las funciones para manipular los archivos de entrada y salida,
 * generar el archivo con la tabla de frecuencias, etc.
*/

#include "header.h"
#include "bits.h"

/**
 * Función encargada de determinar las frecuencias de cada byte, generar el archivo 'Frecuencias.txt' y 
 * determinar la longitud del arreglo de frecuencias para generar el montículo mínimo
 * @param filename: Nombre del archivo que vamos a codificar
 * @param longitud: Longitud del arreglo de frecuencias
*/

NodoHuffman *calcularFrecuencias (char *filename, int *longitud) {
    FILE *apf = fopen ("archivo.txt", "rb");
    int *frecuencias = (int*) calloc (256, sizeof(int));
    unsigned char *buffer = (unsigned char*) malloc (sizeof(unsigned char) * 1024);
    if (apf == NULL) {
        fprintf(stderr, "Error al abrir el archivo\n");
        exit(EXIT_FAILURE);
    }

    //Calcular la cantidad de bytes del archivo
    size_t bytesleidos = fread(buffer, sizeof(unsigned char), 1024, apf);
    printf("Bytes leidos: %d\n", bytesleidos);
    rewind(apf);
    
    // Calculamos frecuencias de caracteres
    unsigned char byte;
    while(fread(&byte, sizeof(unsigned char), 1, apf) > 0) 
        frecuencias[byte]++;

    // Construimos el archivo con la tabla de frecuencias
    FILE *archivo_frecuencia = fopen ("Frecuencias.txt", "w");
    for (int i = 0; i < 256; i++)
        if(frecuencias[i] != 0) {
            fprintf(archivo_frecuencia, "Byte: %x Frecuencias: %d\n", i, frecuencias[i]);
            (*longitud)++; // Para determinar la capacidad del Heap
        }

    fclose(apf);
    return construirArbolHuffman (frecuencias, *longitud);
}



void consultarByte (unsigned char x) {
    for(int i = 7; i >= 0; i--)
        printf("%d", CONSULTARBIT(x,i));
    printf("\n");
}
