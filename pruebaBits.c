#include "header.h"
#include "bits.h"


int main (int argc, char *argv[]) {
    unsigned char numero = 78;

    // Consultar el valor de cada bit
    for (int i = 7; i >= 0; i--)
        printf("%d", CONSULTARBIT(numero, i)); // 01001110
    printf("\n");   

    // Modifica el valor de bits
    numero = PONE_1(numero, 7); // 11001110
    for (int i = 7; i >= 0; i--)
        printf("%d", CONSULTARBIT(numero, i));
        printf("\n");  

    printf("Ahora el valor es: %u\n", numero); 

    // Establecemos el bit 2 en 0
    numero = PONE_0(numero, 2); // 11001010
    for (int i = 7; i >= 0; i--)
        printf("%d", CONSULTARBIT(numero, i));
    printf("\n");  

    printf("Ahora el valor es: %u\n", numero); 

    // Negar el bit 5
    numero = CAMBIA(numero, 5); // 11101010
    for (int i = 7; i >= 0; i--)
        printf("%d", CONSULTARBIT(numero, i));
    printf("\n");  

    printf("Ahora el valor es: %u\n", numero); 
    

    
}