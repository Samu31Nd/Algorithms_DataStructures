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

TablaCodigo *generarTablaCodigos (NodoHuffman *root) {
    TablaCodigo *tabla_Codigos = calloc (sizeof(TablaCodigo), 256);
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

    /* Si ambos apuntan a NULL, quiere decir que hemos llegado a un nodo hoja, entonces vamos a copiar
    los datos en nuestra tabla en la posición 'root->dato' porque representar un número de la tabla ASCII */
    if (root->right == NULL && root->left == NULL) {
        tabla[root->dato].byte = root->dato;
        tabla[root->dato].longitud = i;
        tabla[root->dato].codigo = (unsigned char *) malloc(sizeof(unsigned char) * i); // Asignamos memoria dinámica a cada código
         
        /* memcpy: Función para copiar un bloque de bytes de una dirección de memoria a otra dirección de memoria*/
        memcpy (tabla[root->dato].codigo, codigo, sizeof(unsigned char) * i);
        tabla[root->dato].codigo[i] = '\0'; // Agregamos el caracter de fin de cadena
        (*indice)++; 
       
    }
}


/**
 * Función encargada de mostrar los codigos asignados a cada byte
 * @param tablaCodigos: Es la tabla doinde almacenamos los bytes con su correspondiente
 * código asignado por el recorrido del árbol de Huffman.
 * @param tam: Es el tamaño de la tabla, su valo máximo es 255
 **/

void imprimirCodigos (TablaCodigo *tablaCodigos, int tam) {
    int i, j;
    for(i = 0; i < tam; i++) {
        if (tablaCodigos[i].codigo != NULL) {
            printf("Byte: %02d\t", tablaCodigos[i].byte, tablaCodigos[i].byte);
            printf("Codigo generado: %s\n", tablaCodigos[i].codigo); 
        }  
    }
}

/**
 * Función implementada para codificar el archivo 
 * @param inputFile: Es el archivo que vamos a codificar
 * @param tabla: Es la tabla que contiene los bytes y los códigos que le corresponde a cada caracter
 * @param tam: Tamaño de la tabla que es a lo mucho de 255 
 * @param bytesCodificado: Almacena la cantidad de bytes después de la codificación
*/

void codificarArchivo (char *inputFile, TablaCodigo *tabla, int tam, long *bytesCodificado) {
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
        int pos = buscarCodigoLineal (tabla, tam, byte); // Buscamos el byte leído
        int longitud = tabla[pos].longitud; 
        unsigned char *codigo = tabla[pos].codigo;
        int i;
        for (i = 0; i < longitud; i++) {
            if (codigo[i] == '1')
                PONE_1(byte_Salida, indiceBit);

            if (codigo[i] == '0')
                PONE_0(byte_Salida, indiceBit);

            indiceBit++;
            if(indiceBit == 8) {
                fwrite (&byte_Salida, sizeof(unsigned char), 1, archivo_salida);
                printf("\n");
                byte_Salida = 0;
                indiceBit = 0;
            }
        }
    }

    // En caso de que queden bit sin escribir, debemos completar el byte con ceros
    if (indiceBit > 0) {
        printf("Sobran %d Bits, byte %x\n", indiceBit, byte_Salida);
        while (indiceBit < 8) {
            PONE_0(byte_Salida, indiceBit);
            indiceBit++;
        }
        // printf("Byte de salida: %x\n", byte_Salida);
        fwrite(&byte_Salida, sizeof (unsigned char), 1, archivo_salida);
        printf("Ultimo Byte: %x\n", byte_Salida);
    }

    fclose (archivo_entrada);
    rewind (archivo_salida);
    fseek (archivo_salida, 0, SEEK_END);
    *bytesCodificado = ftell (archivo_salida);
    fclose (archivo_salida);
    printf("CODIFICACION EXITOSA\n");
   

}

/**
 * Función para decodificar el archivo comprimido
 * @param name_codificado: Es el nombre del archivo codificado con extensión ".dat".
 * @param name_decodificado: Es el nombre del archivo original.
 * @param raiz: Es el arbol de Huffman
*/

void decodificar (char *nombre_codificado, char *nombre_decodificado, NodoHuffman *raiz) {
    FILE *codificado = fopen (nombre_codificado, "rb");
    FILE *decodificado = fopen (nombre_decodificado, "wb");

    if (codificado == NULL || decodificado ==  NULL) {
        fprintf(stderr, "Error al abrir los archivos.\n");
        exit(EXIT_FAILURE);
    }

    NodoHuffman *nodo = raiz;
    unsigned char byte;
    int bitsLeidos = 0;
    int totalBits = 0;
    // Calculamos los bytes del archivo codificado
    fseek(codificado, 0, SEEK_END);
    long bytes = ftell(codificado); // Calculamos la cantidad de Bytes
    rewind (codificado); // Regresamos al inicio del archivo
    printf("Cantidad de Bytes de archivo codificado: %ld\n", bytes);

    while (fread(&byte, sizeof(unsigned char), 1, codificado) > 0) {
        int i;
        byte = invertirIzqDer(byte);
        // Iniciamos por el bit más significativo
        for (i = 7; i >= 0; i--) {
            if (CONSULTARBIT (byte, i) == 1) {
                if(nodo->right != NULL)
                    nodo = nodo->right;
            }   else {
                if (nodo->left != NULL) 
                    nodo = nodo->left;
            }
            
            bitsLeidos++;
            totalBits++;

            // En caso de que lleguemos con un nodo hijo
            if (nodo->left == NULL && nodo->right == NULL) {
                fwrite(&(nodo->dato), sizeof(unsigned char), 1, decodificado);
                nodo = raiz; // Se regresa a la raíz
            }

            if (bitsLeidos == 8) {
                bitsLeidos = 0;
                break;
            }
        }
    }

    /* Van a quedar bits que sobran entonces eso hará que decodifique bytes que no pertenecen
    al archivo original */



    // Cerramos ambos archivos
    fclose (codificado);
    fclose (decodificado);
}

/**
 * Función encargada de invertir los bits de izquierda a derecha para la decodificación del archivo
 * @param byte: Byte que vamos a invertir
*/
unsigned char invertirIzqDer(unsigned char byte) {
    unsigned char resultado = 0;
    int i;

    for (i = 0; i < 8; i++) {
        if (CONSULTARBIT (byte, i))
            CAMBIA(resultado, 7 - i); // Macro para invertir los bits de izquierda a derecha
    }

    return resultado;
}

int buscarCodigoLineal (TablaCodigo *tabla, int tam, unsigned char byte) {
    int i;
    for (i = 0; i < tam; i++)
        if (tabla[i].byte == byte)
            return i;
        
    return -1;
}
