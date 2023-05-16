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
    TablaCodigo *tabla_Codigos = (TablaCodigo*) malloc(tam * sizeof(tabla_Codigos));
    unsigned char *codigo =  (unsigned char*) malloc(tam * sizeof(unsigned char));
    int indice = 0;
    // Llamar función para recorrer el árbol y generar los códigos de Huffman 
    generarCodigosHuffman (root, codigo, tabla_Codigos, 0, indice);
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

void generarCodigosHuffman (NodoHuffman *root, unsigned char *codigo, TablaCodigo *tabla, int i, int indice) {
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
        tabla[indice].byte = root->dato;
        tabla[indice].longitud = i;
        tabla[indice].codigo = (unsigned char *) malloc(sizeof(unsigned char) * i); // Asignamos memoria dinámica a cada código
         
        /* memcpy: Función para copiar un bloque de bytes de una dirección de memoria a otra*/
        memcpy (tabla[indice].codigo, codigo, sizeof(unsigned char) * i);
        tabla[indice].codigo[i] = '\0'; // Agregamos el caracter de fin de cadena
        printf("Byte: %02x\n", tabla[indice].byte);
        printf("Codigo: %s\n", tabla[indice].codigo);
        (indice)++; 
       
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
        printf("Byte: %02x\t", tablaCodigos[i].byte);
        
    }
}
