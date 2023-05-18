/******************************************************************
//
//  ESCOM - IPN
//  Nombre de equipo: "Los C4"
//  Fecha: 5/12/2022
//  Algoritmo: Huffman (Compresión)
//
******************************************************************/

//*****************************************************************
//                     Librerias incluidas
//*****************************************************************
#include <stdio.h>
#include <stdlib.h>


//*****************************************************************
//               Declaracion de las estructuras
//*****************************************************************
typedef struct nodo{
    unsigned char letra;
    int frecuencia;
    struct nodo *siguiente;
    struct nodo *cero;
    struct nodo *uno;
}Nodos;

typedef struct tabla{
    unsigned char letra;
    unsigned long int bits;
    unsigned char numbits;
    struct tabla *siguiente;
}Tablas;

//*****************************************************************
//               Declaracion de variables globales
//*****************************************************************
Tablas *tabla;


//*****************************************************************
//                 Declaracion de funciones
//*****************************************************************
void verificarArgumentos(int);
void Cuenta(Nodos**, unsigned char );
void Ordenar(Nodos**);
void InsertarOrden(Nodos** , Nodos *);
void CrearTabla(Nodos *, int , int );
void InsertarTabla(unsigned char,int,int);
Tablas *BuscaCaracter(Tablas *, unsigned char );


//*****************************************************************
//                  Programa principal
//*****************************************************************
int main(int argc, char *argv[]){
    //Variables para medicion de tiempos.
    //double utime0, stime0, wtime0, utime1, stime1, wtime1;
    
    //long del fichero originak
    long int longitud=0;
    //Palabra usada durante la codificacion
    unsigned long int palabra;
    //Elementos en la tabla y el no. de bits usados en palabra
    int numelementos,numbits;
    //Lista de letras y frecuencias
    Nodos *lista;
    //Arbol de letras y frecuencias
    Nodos *Arbol,*frec;
    //Se cren los ficheros de entrada y salida
    FILE *fentrada,*fsalida,*tfrecuencias;
    //variables auxiliares
    unsigned char c;
    Nodos *p;
    Tablas *t;
    verificarArgumentos(argc);
    lista=NULL;
    //Parte 1: Contar frecuencias
    //Se abre el fichero de entrada para lectura

    //******************************************************************
    //  Iniciar el conteo del tiempo para las evaluaciones de rendimiento
    //******************************************************************
    //uswtime(&utime0, &stime0, &wtime0);

    fentrada=fopen(argv[1],"rb");
    //Aqui leera caracter por caracter del archivo y lo almacenara en lista
    while(!feof(fentrada)){
        c=fgetc(fentrada);
        longitud++;
        Cuenta(&lista,c);
    }
    /*Una vez leidos y almacenados todos los caracteres se cierra
    el fichero entrada que ya no se ocupa*/
    fclose(fentrada);
    //Aqui se manda a llamar la funcion y la ordena de menor a mayor
    Ordenar(&lista);

    //se cre un arbol con los valores de lista
    Arbol=lista;
    frec=lista->siguiente;
    tfrecuencias=fopen("frecuencias.txt","wt");
    fprintf(tfrecuencias,"%s\n","Tabla de frecuencias");
    while(frec){
        fprintf(tfrecuencias,"%s\n","-----------------------------------------------");
        fprintf(tfrecuencias,"%s","Symbolo: ");
        fprintf(tfrecuencias,"%c",frec->letra);
        fprintf(tfrecuencias,"\tFrecuencia: %d\n",frec->frecuencia);
        frec=frec->siguiente;
    }
    free(frec);
    fprintf(tfrecuencias,"%s\n","-----------------------------------------------");
    fclose(tfrecuencias);
    //Mientras existan al menos 2 elementos en la lista
    while(Arbol&&Arbol->siguiente){
        p=(Nodos*)malloc(sizeof(Nodos));//Se crea un nuevo arbol p
        p->letra=0;//No corresponde a ninguna letra
        p->uno=Arbol;//rama
        Arbol=Arbol->siguiente;
        p->cero=Arbol;//rama
        Arbol=Arbol->siguiente;
        p->frecuencia=p->uno->frecuencia+p->cero->frecuencia;//suma las frecuencias
        InsertarOrden(&Arbol, p);//inserta en un nuevo nodo su orden de frecuencias
    }
    //Se construye la tabla con codigos binarios
    tabla=NULL;
    //Se inicializa la tabla
    CrearTabla(Arbol,0,0);
    //Se abre el fichero salida para escritura, donde se comprime
    fsalida=fopen(argv[2],"wb");
    //Se escribe la longitud del fichero
    fwrite(&longitud,sizeof(long int),1,fsalida);
    //cant de elementos en la tabla
    numelementos=0;
    //Se crea una estructura tabla auxiliar
    t=tabla;
    //Se hace el conteo de los elementos en la tabla
    while(t){
        numelementos++;
        t=t->siguiente;
    }
    //Escribe el numero de elementos en la tabla
    fwrite(&numelementos,sizeof(int),1,fsalida);
    //Se escribe la tabla en el fichero
    t=tabla;
    while(t){
        fwrite(&t->letra,sizeof(char),1,fsalida);
        fwrite(&t->bits,sizeof(unsigned long int),1,fsalida);
        fwrite(&t->numbits,sizeof(char),1,fsalida);
        t=t->siguiente;
    }
    //Codificaciion del fichero de entrada
    fentrada=fopen(argv[1],"rb");
    palabra=0;
    numbits=0;
    while(!feof(fentrada)){
        c=fgetc(fentrada);
        if(feof(fentrada))
            break;
        t=BuscaCaracter(tabla,c);
        /* Si nBits + t->nbits > 32, sacar un byte */
        while(numbits+t->numbits>32){
            c=palabra>>(numbits-8); // Extrae los 8 bits de mayor peso
            fwrite(&c,sizeof(char),1,fsalida); //Los escribe en el fichero de salida
            numbits-=8;//Se tiene hueco para 8 bits mas
        }
        palabra<<=t->numbits;//Se hace sitio para otro caracter
        palabra|=t->bits;//Se agrega el caracter nuevo
        numbits+=t->numbits;//Se actualiza la cuenta de bits
    }    
    //Se extraen los 4 bits restantes que sobran en palabra
    while(numbits>0){
        if(numbits>=8)
            c=palabra>>(numbits-8);
        else
            c=palabra<<(8-numbits);
        fwrite(&c,sizeof(char),1,fsalida);
        numbits-=8;
    }
    //Se cierran los ficheros, se borra el arbol y la tabla
    fclose(fentrada);
    fclose(fsalida);
    //******************************************************************
    //           Evaluar los tiempos de ejecucion
    //******************************************************************                
    //uswtime(&utime1, &stime1, &wtime1);
    
    //printf("%d - %d -----------------------------------------------------------------------", longitudNum, elementoBuscar);
    //Calculo del tiempo de ejecucion del programa
    //printf("\n");
    //printf("real (Tiempo total)  %.10f s\n", wtime1 - wtime0);
    //printf("%.10f\n", wtime1 - wtime0);
    //printf("-----------------------------------------------------------------------------------\n\n\n");          

    return 0;
}

//************************************************************************
//               Definicion de funciones
//************************************************************************

/**
 * Verifica que se hayan pasado los suficientes argumentos a la hora
 * de ejecutar el programa.
 *
 * @param  {int} Cantidad de argumentos pasados en la ejecucion del programa
 *
 * @return {void}
 */
void verificarArgumentos(int argumentos){
    if (argumentos < 3) {
        printf("\nIndique <ficheroDeEntrada> <FicherodeSalida>.\nEjemplo: [user@equipo]$ ./programa 1000\n\n");
        exit(-1);
    } 

}

/**
 * Hace el conteo para las frecuencias del caracter
 *
 * @param  {Nodos**} Doble puntero a un arbol de nodo
 *
 * @param  {unsigned char} caracter que se lee 
 * 
 * @return {void}
 */
void Cuenta(Nodos** lista,unsigned char c){
    Nodos *p,*a,*q;
    if(!*lista){
        *lista=(Nodos*)malloc(sizeof(Nodos));
        (*lista)->letra=c;
        (*lista)->frecuencia=1;
        (*lista)->siguiente=NULL;
        (*lista)->cero=NULL;
        (*lista)->uno=NULL;
    }else{
        p=*lista;
        a=NULL;
        while(p&&p->letra<c){
            a=p;
            p=p->siguiente;
        }
        if(p&&p->letra==c)
            p->frecuencia++;
        else{
            q=(Nodos*)malloc(sizeof(Nodos));
            q->letra=c;
            q->frecuencia=1;
            q->cero=q->uno=NULL;
            q->siguiente=p;
            if(a)
                a->siguiente=q;
            else
                *lista=q;
        }
    }
}

/**
 * Ordenea el arbol de huffman
 *
 * @param  {Nodos**} Doble puntero a un arbol de nodo
 *
 * @return {void}
 */
void Ordenar(Nodos** lista){
    Nodos *lista2,*a;
    if(!*lista)
        return;
    lista2=*lista;
    *lista=NULL;
    while(lista2){
        a=lista2;
        lista2=a->siguiente;
        InsertarOrden(lista,a);
    }
}

/**
 * Se ordena la tabla de frecuencias
 *
 * @param  {Nodos**} Doble puntero a un arbol de nodo
 *
 * @param  {Nodos*} Puntero
 * 
 * @return {void}
 */
void InsertarOrden(Nodos** Cabeza,Nodos*e){
    Nodos *p,*a;
    if(!*Cabeza){
        *Cabeza=e;
        (*Cabeza)->siguiente=NULL;
    }else{
        p=*Cabeza;
        a=NULL;
        while(p&&p->frecuencia<e->frecuencia){
            a=p;
            p=p->siguiente;
        }
        e->siguiente=p;
        if(a)
            a->siguiente=e;
        else
            *Cabeza=e;
    }
}

/**
 * Crea la tabla de binarios
 *
 * @param  {Nodos*} Puntero a un arbol de nodo
 * 
 * @param  {int} valor 1
 *
 * @param  {int} valor 2
 * 
 * @return {void}
 */
void CrearTabla(Nodos *n,int l,int v){
    if(n->uno)
        CrearTabla(n->uno,l+1,(v<<1)|1);
    if(n->cero)
        CrearTabla(n->cero,l+1,v<<1);
    if(!n->uno&&!n->cero)
        InsertarTabla(n->letra,l,v);
}

/**
 * Se crea una nueva tabla de binarios ordenada para comprimir
 *
 * @param  {unsigned char} 
 *
 * @param  {int} valor del bit
 * 
 * @param  {int} cantidad de bits
 * 
 * @return {void}
 */
void InsertarTabla(unsigned char c,int l, int v){
    Tablas *t,*p,*a;
    t=(Tablas *)malloc(sizeof(Tablas));
    t->letra=c;
    t->bits=v;
    t->numbits=l;
    if(!tabla){
        tabla=t;
        tabla->siguiente=NULL;
    }else{
        p=tabla;
        a=NULL;
        while(p&&p->letra<t->letra){
            a=p;
            p=p->siguiente;
        }
        t->siguiente=p;
        if(a)
            a->siguiente=t;
        else
            tabla=t;
    }
}

/**
 * Busca un caracter en la tabla
 *
 * @param  {Tablas *} Apuntador a una estructura tipo Tablas
 * 
 * @param  {unsigned char} El caracter que se esta buscando
 *
 * @return {void}
 */
Tablas *BuscaCaracter(Tablas *Tabla, unsigned char c)
{
    Tablas *t;
    t = Tabla;
    while(t && t->letra != c) t = t->siguiente;
    return t;
}
