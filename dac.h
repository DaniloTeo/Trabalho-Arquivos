#ifndef DESTROY_AND_CREATE_H
#define DESTROY_AND_CREATE_H

#include "Registro_Delimitador.h"


#define IBS 1000


// Estrutura de uma chave: contem ticket e offset do registro correspondente
typedef struct chprim {

	long int ticket;
	long int offset;
} ChPrim;



// Struct para lidar com os diferentes tipos de implementacao de aproveitamento de espaco
typedef struct group {
	ChPrim* array;
	int length;
	FILE* file;

} Group;



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



RemList *criar_lista(FILE * f);
void apagar_lista(RemList *l);
int lista_vazia(RemList *l);
void inserir_lista(RemList *l, long int offset, long int tam, long int prox);
void print_lista(RemList* l);



// Cria os 3 arquivos com delimitadores
void Create3Files_Delimitador(char* nome);

// Imprime registrador
void print_reg(Registro_Delimitador* reg);

// Passa o conteudo de variavel que contem toda informacao do registro para uma struct
void getReg(Registro_Delimitador* r, void* data);

// Funcao de comparacao necessaria para utilizar os metodos qsort e bsearch
int compare (const void* a, const void* b);

// Cria um arquivo binario de indice de indice, a partir da leitura do arquivo de registros, utilizando o metodo de leitura pelos delimitadores
void createIndex(char* arq_saida, char* arq_ind);

// Cria os tres arquivos de indice
void createIndexes();


// Imprime arquivo de indice
void printIndexFile(char* name);

// Essa funcao atualiza o valor do cabecalho com o offset do ultimo registro removido logicamente.
// O retorno da funcao eh o valor que estava previamente no cabecalho
long int updateHeader(Group* g, long int offset);


// Remove logicamente um registro do arquivo e remove fisicamente a chave correspondente
int removerReg(Group* g, long int ticket);

// Pega o indice que estava em disco e coloca em um array.
void readIndex(Group* g, char* f_name);

//Realiza a insercao de r no arquivo g->file utilizando o metodo first fit, e atualiza g->array(indices) e g->length
void firstFit(Group *g, Registro_Delimitador *r);

//Realiza a insercao de r no arquivo g->file utilizando o metodo best fit, e atualiza g->array(indices) e g->length
void bestFit(Group *g, Registro_Delimitador *r);

//Realiza a insercao de r no arquivo g->file utilizando o metodo worst fit, e atualiza g->array(indices) e g->length
void worstFit(Group *g, Registro_Delimitador *r);

char *readLine(FILE *buffer);


void showStatus(Group* g);

//Imprime os dados dos tres arquivos de indice
void showThreeStatuses(Group g[3]);




#endif
