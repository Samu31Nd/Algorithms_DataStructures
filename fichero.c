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
    fclose(archivo_frecuencia);
    return construirArbolHuffman (frecuencias, *longitud);
}
/**
 * Función para guardar la tabla de códigos para realizar la decodificación del archivo
 * @param tabla: Tabla de Códigos
 * @param tam: Tamaño de la tabla
*/

void guardarTabla (TablaCodigo *tabla, int tam) {
    FILE *apf = fopen ("Tabla.bin", "wb");

    if (apf == NULL) {
        fprintf(stderr, "Error al crear el archivo.\n");
        exit(EXIT_FAILURE);
    }

    // Escribimos la tabla de códigos
    fwrite (&tam, sizeof(int), 1, apf);
    fwrite (tabla, sizeof(TablaCodigo), tam, apf);
    fclose (apf);
}

TablaCodigo *recuperarTabla (char *tableName, int *tam_Tabla) {
    FILE *apf = fopen (tableName, "rb");

    if (apf == NULL) {
        fprintf(stderr, "Error al intentar abrir el archivo.\n");
        exit(EXIT_FAILURE);
    }

    int tam;
    fread(&tam, sizeof(int), 1, apf);
    printf("El tam de la tabla es: %d\n", tam);
    TablaCodigo *tabla_Recuperada = (TablaCodigo *) malloc (sizeof(TablaCodigo) * tam);
    if (tabla_Recuperada == NULL) {
        fprintf(stderr, "Error al asignar memoria a la tabla.\n");
        exit(EXIT_FAILURE);
    }

    // Recuperamos la tabla
    fread (tabla_Recuperada, sizeof(TablaCodigo), tam, apf);
    fclose (apf);
    return tabla_Recuperada;
}