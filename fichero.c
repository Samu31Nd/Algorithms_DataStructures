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
    FILE *apf = fopen ("Tabla.dat", "wb");

    if (apf == NULL) {
        fprintf(stderr, "Error al crear el archivo.\n");
        exit(EXIT_FAILURE);
    }

    // Escribimos la tabla de códigos
    int i;
    for (i = 0; i < tam; i++) {
        fwrite (&tabla[i].byte, sizeof(unsigned char), 1, apf);
        fwrite (&tabla[i].longitud, sizeof(int), 1, apf);
        fwrite (&tabla[i].codigo,sizeof(unsigned char), 1, apf);
    }

    fclose (apf);
}

TablaCodigo *recuperarTabla (char *tableName) {
    FILE *apf = fopen (tableName, "rb");

    if (apf == NULL) {
        fprintf(stderr, "Error al intentar abrir el archivo.\n");
        exit(EXIT_FAILURE);
    }

    // Determinar el tamaño de la tabla
    fseek (apf, 0, SEEK_END); 
    long tam_Archivo = ftell (apf);

    /**
     *  Se calcula el tamaño de la tabla, tam_Archivo almacena la cantidad de bytes del archivo
     *  restamos la cantidad de bytes de un entero y dividimos entre la suma del tamaño de
     *  bytes que ocupa un byte, un entero, y otro byte
     * */
    int tam = (tam_Archivo - sizeof(int)) / (sizeof(unsigned char) + sizeof(int) + sizeof(unsigned char));

    // Asginamos memoria a nuestra tabla
    TablaCodigo *tabla = (TablaCodigo*) malloc(sizeof(unsigned char) * tam);
    int i;

    /* Para leer los datos, debemos de tomar en cuenta el orden en el que fueron escritos */
    for (i = 0; i < tam; i++) {
        fread (&tabla[i].byte, sizeof(unsigned char), 1, apf);
        fread (&tabla[i].longitud, sizeof(int), 1, apf);
        tabla[i].codigo =  (unsigned char*) malloc(sizeof(unsigned char) * tabla[i].longitud);
        fread (&tabla[i].codigo,sizeof(unsigned char), 1, apf);   
    }
    fclose (apf);
    return tabla;
}