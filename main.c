/**
 * @author: Serge Eduardo Martínez Ramírez
 * @name: Algoritmo de Huffman (Compresión de Archivos)
 * @file: main.c 
 * @date: 07/05/2023
 * @authors: Sic Mundus
 * Escuela Superior de Cómputo
*/

#include "header.h"

/* Función Main */
int main(int argc, char *argv[]) {
    //*********************************************************
    // VARIABLES PARA MEDIR TIEMPO
    //*********************************************************
    double utime0, stime0, wtime0,utime1, stime1, wtime1; //Variables para medición de tiempos
	int n; 	//n determina el tamaño del algorito dado por argumento al ejecutar
	int i; //Variables para loops

    //******************************************************************	
	//Iniciar el conteo del tiempo para las evaluaciones de rendimiento
	//******************************************************************	
	uswtime(&utime0, &stime0, &wtime0);

    //*****************************************************************
    //                 ¿CÓMO USAR EL PROGRAMA?                         
    //*****************************************************************

    int tam = 0;
    if (argc != 2 && argc != 3) {
        printf("INSTRUCCIONES PARA LA EJECUCION DEL PROGRAMA\n\n");
        printf("Comprimir Archivo:\n");
        printf("./Huffman <nombreArchivo.extension>\n\n");
        printf("Descomprimir Archivo\n");
        printf("./Huffman <codificacion.dat> <nombreArchivoSalida.extension>\n");
        exit(EXIT_SUCCESS);
    }

    //*****************************************************************
    //                 COMPRESIÓN DE ARCHIVOS                     
    //*****************************************************************

    if (argc == 2) {
        long cantidadBytes = 0;
        long BytesCodificado = 0;
        system("cls");
        printf("COMPRIMIENDO ARCHIVO '%s'...\n", argv[1]);
        /* Se manda como parámetro el nombre del archivo a codificar, se calculan
        las frecuencias, se construyen el montículo mínimo y se retorna el árbol de Huffman*/
        NodoHuffman *root = calcularFrecuencias (argv[1], &tam, &cantidadBytes);
        TablaCodigo *tabla_Codigos = generarTablaCodigos (root);
        codificarArchivo (argv[1], tabla_Codigos, 256, &BytesCodificado);
        printf("Cantidad de Bytes de '%s': %ld.\n", argv[1], cantidadBytes);
        printf("Cantidad de Bytes del archivo codificado: %ld.\n", BytesCodificado);
        float division = (float) cantidadBytes / BytesCodificado;
        printf("Porcentaje de codificacion: %f\n", (float) division * 100);
    }

    //*****************************************************************
    //                 DESCOMPRESIÓN DE ARCHIVO                        
    //*****************************************************************

    else if (argc == 3) {
        int longitudArrFrecuencias = 0; // Para determinar la longitud del arreglo de Frecuencias
        system("cls");
        printf("DESCOMPRIMIENDO ARCHIVO '%s'...\n", argv[1]);
        /* Dado el archivo de frecuencias, se recupera el arreglo del archivo para
        construir el árbol de Huffman */
        int *frecuencias = recuperarFrecuencias ("Frecuencias.txt", &longitudArrFrecuencias);
        NodoHuffman *root = construirArbolHuffman (frecuencias, longitudArrFrecuencias);
        // Construimos la tabla de Códigos
        TablaCodigo *tabla_Codigos = generarTablaCodigos (root);
        // Se comienza a decodificar el archivo
        decodificar (argv[1], argv[2], root);
    }

    //******************************************************************	
	//Evaluar los tiempos de ejecución 
	//******************************************************************
	uswtime(&utime1, &stime1, &wtime1);
	
	//Cálculo del tiempo de ejecución del programa
	printf("\nTiempo de Ejecucion\n");
	printf("real (Tiempo total)  %.10f s\n",  wtime1 - wtime0);
	printf("user (Tiempo de procesamiento en CPU) %.10f s\n",  utime1 - utime0);
	printf("sys (Tiempo en acciónes de E/S)  %.10f s\n",  stime1 - stime0);
	printf("CPU/Wall   %.10f %% \n",100.0 * (utime1 - utime0 + stime1 - stime0) / (wtime1 - wtime0));
	printf("\n");
	
	//Mostrar los tiempos en formato exponecial
	printf("\n");
	printf("real (Tiempo total)  %.10e s\n",  wtime1 - wtime0);
	printf("user (Tiempo de procesamiento en CPU) %.10e s\n",  utime1 - utime0);
	printf("sys (Tiempo en acciónes de E/S)  %.10e s\n",  stime1 - stime0);
	printf("CPU/Wall   %.10f %% \n",100.0 * (utime1 - utime0 + stime1 - stime0) / (wtime1 - wtime0));
	printf("\n");
	//******************************************************************

    system ("pause");
    return 0;
}