# Algoritmo de Huffman 
## Compilar el programa.
El programa utiliza una librería de __UNIX__ para la medición de tiempos de ejecución
del programa.

    gcc main.c fichero.c minHeap.c huffman.c tiempo.c -o Huffman

Una vez compilado el programa, podrás ejecutarlo para comprimir y descomprimir un archivo.

### Comprimir un archivo
Para comprimir un archivo, debemos de mandar como parámetro el nombre del archivo que queremos comprimir indicando la extensión de este.

    ./Huffman nombreArchivo.extensión

### Descomprimir un archivo
Para descomprimir un archivo, debemos de mandar dos parámetros. Uno será el nombre del archivo  comprimido ***'codificacion.dat'*** y el otro será el nombre de nuestro nuevo archivo, especificando la extensión que era antes de comprimirse.  
   
    ./Huffman codificacion.dat archivo_Salida.extensión
    