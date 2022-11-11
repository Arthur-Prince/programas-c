//Lista sequencial ordenada
//comando no cygwin: gcc lista.c -o lista.exe
#include<stdio.h>
#include<malloc.h>
#define MAX 50
#define ERRO -1
#define true 1
#define false 0
typedef int bool;
typedef int tipochave;
typedef strict{
	tipochave chave;
}REGISTRO;
typedef struct{
	registro a[max+1];
	int nroElem;
}LISTA;