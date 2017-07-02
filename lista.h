#ifndef LISTA_H
#define LISTA_H

#include "Registro_Delimitador.h"
#include "dac.h"



typedef struct node{
	long int offset;
	long int tamanho;
	long int prox;
	struct node *proximo;
}Node;
	

typedef struct remlist{
	Node *inicio;
	Node *fim;
	int cont;

}RemList;


//Cria lista de remocao de cada arquivo (ordenando para os arquivos 2 e 3)
RemList *criar_lista(FILE * f, int t);

//Desaloca a memoria utilizada pela lista
void apagar_lista(RemList *l);

//Indica se a lista esta vazia
int lista_vazia(RemList *l);

//Insere um elemento na lista
void inserir_lista(RemList *l, long int offset, long int tam, long int prox);

//Imprime a lista de remocao
void print_lista(RemList* l);

//ordena a lista de remocao de acordo com o parametros t
//	t = 1 - ordem crescente
//	t = 2 - ordem decrescente 
void ordenar_lista(RemList *l, int t);


#endif