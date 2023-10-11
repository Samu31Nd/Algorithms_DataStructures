/**
 * @author: Edgardo Adrián Franco Martínez
 * @name: Algoritmo de Huffman (Compresión de Archivos)
 * @file: bits.h
 * @date: 07/05/2023
 * Escuela Superior de Cómputo **/

#ifndef __BITS__
#define __BITS__

#define PESOBIT(bpos) 1<<bpos
#define CONSULTARBIT(var,bpos) (*(unsigned*)&var & PESOBIT(bpos))?1:0
#define PONE_1(var,bpos) *(unsigned*)&var |= PESOBIT(bpos)
#define PONE_0(var,bpos) *(unsigned*)&var &= ~(PESOBIT(bpos))
#define CAMBIA(var,bpos) *(unsigned*)&var ^= PESOBIT(bpos)

#endif 