#include "header.h"

/**
 * Función para realizar el recorrido en inorden del árbol de Huffman
 * @param root: Es la raíz del árbol de Huffman
*/
void inorden (NodoHuffman *root) {
    if(root == NULL) 
        return;
    inorden(root->left);
    printf("Caracter: %c Frecuencia: %d\n", root->dato, root->frecuencia);
    inorden(root->right);
}

/** Función para determinar la altura del árbol de Huffman */

int calcular_altura(NodoHuffman* root) {
    if (root == NULL)
        return 0;

    int altura_izquierda = calcular_altura(root->left);
    int altura_derecha = calcular_altura(root->right);

    return 1 + (altura_izquierda > altura_derecha ? altura_izquierda : altura_derecha);
}

/**
 * Función encargada de generar la tabla de códigos
 * @param root: Es la raíz del árbol de Huffman
 * @param tam: Es el tamaño que va a tener nuestra tabla
 * @return: Nos retorna la tabla con el código correspondiente a cada byte
*/

TablaCodigo *generarTablaCodigos (NodoHuffman *root, int tam) {
    TablaCodigo *tabla_Codigos = (TablaCodigo*) malloc(tam * sizeof(TablaCodigo));
    int altura = calcular_altura (root);
    unsigned char *codigo =  (unsigned char*) malloc(altura * sizeof(unsigned char));
    int indice = 0;
    // Llamar función para recorrer el árbol y generar los códigos de Huffman 
    generarCodigosHuffman (root, codigo, tabla_Codigos, 0, &indice);
    return tabla_Codigos;
}

/**
 * Función que determina los códigos que le correponden a cada byte del árbol de Huffman.
 * @param root: Raíz del árbol de Huffman
 * @param codigo: Para almacenar el codigo obtenido de un byte
 * @param tabla: tabla de códigos
 * @param i: Se irá incrementando segun vaya asignando ceros o unos para determinar el tamaño del codigo
 * @param indice: Se irá incrementando cada vez que tengamos el código de un byte
*/

void generarCodigosHuffman (NodoHuffman *root, unsigned char *codigo, TablaCodigo *tabla, int i, int *indice) {
    // Si se tiene un hijo izquierdo
    if (root->left) {
        codigo[i] = '0';
        generarCodigosHuffman (root->left, codigo, tabla, i + 1, indice);
    }

    // Si se tiene un hijo derecho
    if (root->right) {
        codigo[i] = '1';
        generarCodigosHuffman (root->right, codigo, tabla, i + 1, indice);
    }

    // Si tenemos un nodo hoja, entonces ya llegamos a un byte.
    if (root->right == NULL && root->left == NULL) {
        tabla[*indice].byte = root->dato;
        tabla[*indice].longitud = i;
        tabla[*indice].codigo = (unsigned char *) malloc(sizeof(unsigned char) * i); // Asignamos memoria dinámica a cada código
         
        /* memcpy: Función para copiar un bloque de bytes de una dirección de memoria a otra*/
        memcpy (tabla[*indice].codigo, codigo, sizeof(unsigned char) * i);
        tabla[*indice].codigo[i] = '\0'; // Agregamos el caracter de fin de cadena
        (*indice)++; 
       
    }
}


/**
 * Función encargada de mostrar los codigos asignados a cada byte
 * @param tablaCodigos: Es una matriz donde se almacenaron los códigos correspondientes
 * después de realizar el recorrido del arbol de huffman
 **/

void imprimirCodigos (TablaCodigo *tablaCodigos, int tam) {
    int i, j;
    for(i = 0; i < tam; i++) {
        printf("Byte: %02x\t Caracter %c\t", tablaCodigos[i].byte, tablaCodigos[i].byte);
        printf("Codigo generado: %s\n", tablaCodigos[i].codigo);   
    }
}

/**
 * Función implementada para codificar el archivo 
 * @param inputFile: Es el archivo que vamos a codificar
 * @param tabla: Es la tabla que contiene los bytes y los códigos que le corresponde a cada caracter
 * @param tam: Tamaño de la tabla
*/

void codificarArchivo (char *inputFile, TablaCodigo *tabla, int tam) {
    FILE *archivo_entrada = fopen (inputFile, "rb");
    FILE *archivo_salida = fopen ("codificacion.dat", "wb");

    if (archivo_entrada == NULL || archivo_salida == NULL) {
        fprintf(stderr, "Error al abrir los archivos.\n");
        exit(EXIT_FAILURE);
    }

    unsigned char byte;
    unsigned char byte_Salida =  0;
    int indiceBit = 0;

    while (fread(&byte, sizeof(unsigned char), 1, archivo_entrada) > 0) {
        int pos = buscarCodigoLineal (tabla, tam, byte);
        int longitud = tabla[pos].longitud;
        unsigned char *codigo = tabla[pos].codigo;
        int i;
        
        for (i = 0; i < longitud; i++) {
            byte_Salida = byte_Salida << 1;
            if (codigo[i] == '1')
                PONE_1(byte_Salida, 0);
            else   
                PONE_0(byte_Salida, 0);
            
            indiceBit++;
            if(indiceBit == 8) {
                fwrite (&byte_Salida, sizeof(unsigned char), 1, archivo_salida);
                byte_Salida = 0;
                indiceBit = 0;
            }
        }

        printf("Byte de salida: %x, Caracter correspondiente: %c\n", byte_Salida, byte_Salida);
    }

    fclose (archivo_entrada);
    fclose (archivo_salida);
}

int buscarCodigoLineal (TablaCodigo *tabla, int tam, unsigned char byte) {
    int i;
    for (i = 0; i < tam; i++)
        if (tabla[i].byte == byte)
            return i;
        
    return -1;
}

/* Función de búsqueda binaria para buscar el byte */
int buscarCodigo (TablaCodigo *tabla, int inicio, int final, unsigned char byte) {
    if (inicio > final) 
        return -1; // En caso de que no se encuentre el byte

    int centro = (inicio + final) / 2;
    if (tabla[centro].byte == byte)
        return centro;
    else if (byte < tabla[centro].byte)
        return buscarCodigo (tabla, inicio, centro - 1, byte);
    else 
        return buscarCodigo (tabla, centro + 1, final, byte);
}
