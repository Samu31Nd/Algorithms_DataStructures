#include "header.h"

/**
 * Función encargada de crear un nodo del árbol de Huffman dada
 * su frecuencia y el byte más recurrente 
 * @param dato: Es un byte
 * @param frecuencia: Es la frecuencia de ese byte
 * @return: Nos retorna un nodo
 **/

NodoHuffman *crearNodoHuffman (unsigned char dato, int frecuencia) {
    NodoHuffman *nuevoNodo = (NodoHuffman*) malloc(sizeof(NodoHuffman));
    nuevoNodo->dato = dato;
    nuevoNodo->frecuencia  = frecuencia;
    nuevoNodo->left = nuevoNodo->right = NULL;
    return nuevoNodo;
}

/** Crea el montículo mínimo donde sus nodos son estructuras NodoHuffman 
 * @param capacity: Es la capacidad del montículo, determinada por la cantidad de bytes 
 **/

minHeap *crearHeap (int capacity) {
    minHeap *heap = (minHeap*) malloc(sizeof(minHeap));
    heap->arreglo = (NodoHuffman**) malloc(sizeof(NodoHuffman) * capacity);
    heap->capacity = capacity;
    heap->size = 0;
    return heap;
}

/**
 * Función encargada de insertar un nuevo nodo en el montículo mínimo.
 * El nodo se insertar al final, si se determina que la frecuencia de este nodo
 * es mayor que el nodo de su padre, se realiza el recorrido hacia arriba buscando la posición
 * que le corresponde para respetar la propiedad de montículo
 * 
 * @param heap: Es el montículo en el que se va a realizar la inserción
 * @param nodo: Nodo que se va a insertar
 **/

void insertarNodo (minHeap *heap, NodoHuffman *nodo) { 
    // En caso de que el minHeap esté lleno
    if(heap->capacity == heap->size) {
        printf("El montículo esta lleno");
        exit(1);
    }

    int i = heap->size++;
    // Mientras i y la frecuencia del nodo padre sea mayor que la frecuencia del nodo por insertar
    while (i && (nodo->frecuencia < heap->arreglo[(i - 1)/2]->frecuencia)) {
        heap->arreglo[i] = heap->arreglo[(i - 1)/2];
        i = (i - 1)/2;
    }

    heap->arreglo[i] = nodo; // Se inserta el nuevo nodo
}

/**
 * minHeapify:
 * Se encarga de mantener la propiedad de un heap mini después de haber realizado la extracción
 * del nodo con la frecuencia mínima
 * @param heap: Montículo donde se van a reordenar los nodos
 * @param indice: Es la posición de un nodo
*/

void minHeapify (minHeap *heap, int indice) {
    int small = indice;
    int left = 2 * small + 1; // Posición del hijo izquierdo
    int right = 2 * small + 2; // Posición del hijo derechos

    // Comparar el nodo actual con su hijo izquierdo
    if ((left < heap->size) && (heap->arreglo[left]->frecuencia < heap->arreglo[small]->frecuencia))
        small = left;
    
    // Comparar el nodo actual con su hijo derecho
    if ((right < heap->size) && (heap->arreglo[right]->frecuencia < heap->arreglo[small]->frecuencia))
        small = right;
    
    /**
     * Si el nodo actual contiene una frecuencia mayor que la de sus hijos, intercambiamos
     * por el valor menor, que sería la posición 'small'.
    */

    if (small != indice) {
        NodoHuffman *temporal = heap->arreglo[indice];
        heap->arreglo[indice] = heap->arreglo[small];
        heap->arreglo[small] = temporal;
        // Se llama de nuevo a la función
        minHeapify (heap, small);
    }
}

/**
 * extraerNodo:
 * Se encarga de extraer el nodo mínimo del montículo y llama a la función minHeapify
 * para ajustar la posición de los nodos y la estructura siga siendo un minHeap.
 * 
 * Retorno: La función regresa el nodo de Huffman extraído
*/

NodoHuffman *extraerNodo (minHeap *heap) {
    NodoHuffman *extraido = heap->arreglo[0];
    heap->arreglo[0] = heap->arreglo[heap->size - 1];
    (heap->size)--; // Decrementa el tamaño del arreglo
    minHeapify (heap, 0);
    return extraido;
}

/* Función encargada de construir un heap, dadas las frecuencias y los bytes
    @param frecuencias: Es un arreglo que contiene las frecuencias de cada byte */

NodoHuffman *construirArbolHuffman (int *frecuencias, int capacity) {
    int i;
    minHeap *heap = crearHeap(capacity);
    for(i = 0; i < 256; i++)
        if (frecuencias[i] != 0)
            insertarNodo (heap, crearNodoHuffman (i, frecuencias[i]));

    // Creamos el árbol de Huffman
    while (heap->size > 1) {
        NodoHuffman *left = extraerNodo(heap);
        NodoHuffman *right = extraerNodo(heap);
        NodoHuffman *padre = crearNodoHuffman ('\0', left->frecuencia + right->frecuencia);
        padre->left = left;
        padre->right = right;
        insertarNodo (heap, padre);
    }

    return extraerNodo (heap);
}

/**
 *  Función creada para poder visualizar cómo estaban los nodos en el heap 
 * @param heap: Es el montículo
 * @param capacity: Es la capacidad del montículo
 **/
void mostrarHeap (minHeap *heap, int capacity) {
    int i;
    for (i = 0; i < capacity; i++)
        printf("Byte: %x Frecuencia: %u\n", heap->arreglo[i]->dato, heap->arreglo[i]->frecuencia);
    printf("\n");
}
