#ifndef HEADER_H
#define HEADER_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <wchar.h>


#define BUFFER_SIZE 100
#define LINE_SIZE 500
#define IBS 1000 // define tamanho inicial do buffer dos registros a serem lidos




// ESTRUTURAS


/* Definição de uma struct capaz de armazenar todos os campos de um registro do arquivo
 * A escolha dos tipos e do tamanho dos vetores foi baseado em uma análise dos dados
 */
struct registro_delimitador{
    wchar_t dominio [BUFFER_SIZE]; //O campo "domínio" pode receber wide chars e é de tamanho váriavel, limitado por BUFFER_SIZE
    char documento [20];            //O campo "documento" espera somente chars padrôes e é de tamanho 19, o vetor recebe um char extra para colocar o caractere terminador, facilitando o manuseamento
    wchar_t nome [BUFFER_SIZE];     //O campo "nome" pode receber wide chars e é de tamanho váriavel, limitado por BUFFER_SIZE
    wchar_t uf [BUFFER_SIZE];       //O campo "uf" pode receber wide chars e é de tamanho váriavel, limitado por BUFFER_SIZE
    wchar_t cidade[BUFFER_SIZE];    //O campo "cidade" pode receber wide chars e é de tamanho váriavel, limitado por BUFFER_SIZE
    char data_cadastro [20];        //O campo "documento" espera somente chars padrôes e é de tamanho 19, o vetor recebe um char extra para colocar o caractere terminador, facilitando o manuseamento
    char data_atualiza [20];        //O campo "documento" espera somente chars padrôes e é de tamanho 19, o vetor recebe um char extra para colocar o caractere terminador, facilitando o manuseamento
    long int ticket;                //O campo "ticket" espera um long int
    };

/*
 * Struct contendo todos os campos de um registro
 */
typedef struct registro_delimitador Registro_Delimitador;


// Estrutura No de lista ligada
typedef struct node{
	long int offset;
	int tamanho;
	long int prox;
	struct node *proximo;
}Node;
	
// estrutura para percorrer lista de registros removido em RAM
typedef struct remlist{
	Node *inicio;
	Node *fim;
	int cont;

}RemList;

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







// FUNCOES DE REUSO DE ESPACO

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

// Passa lista de remoção para o arquivo.
void RemListToFile(Group* g, RemList* rem);

char *readLine(FILE *buffer);


void showStatus(Group* g);

//Imprime os dados dos tres arquivos de indice
void showThreeStatuses(Group g[3]);








// FUNCOES DE LISTA

//Cria lista de remocao de cada arquivo (ordenando para os arquivos 2 e 3)
RemList *criar_lista(FILE * f, int t);

//Desaloca a memoria utilizada pela lista
void apagar_lista(RemList *l);

//Indica se a lista esta vazia
int lista_vazia(RemList *l);

//Insere um elemento na lista
void inserir_lista(RemList *l, long int offset, int tam, long int prox);

//Imprime a lista de remocao
void print_lista(RemList* l);

//ordena a lista de remocao de acordo com o parametros t
//	t = 1 - ordem crescente
//	t = 2 - ordem decrescente 
void ordenar_lista(RemList *l, int t);

// Funcao de inserir ordenadamente em ordem crescente
void insercOrdCresc(RemList *l, long int offset, int tam, long int prox);

// Funcao de inserir ordenadamente em ordem decrescente
void insercOrdDec(RemList *l, long int offset, int tam, long int prox);







// FUNCOES DE LEITURA E ESCRITA DE REGISTROS



/*
 * Função que lê um arquivo de nome dado por nome e gera um arquivo de nome "delimitador.bin", com registros separados pelo caractere "~"
 * Parâmetros:
 * char* nome - nome do arquivo de entrada a ser lido
 */
void CreateFile_Delimitador(char* nome);

/*
 * Função que imprime todos os registros do arquivo "delimitador.bin", com os campos separados por linha e pausa entre os registros (pressione enter para prosseguir)
 */
void PrintAll_Delimitador();

/*
 * Função que imprime somente o registro indicado por rrn, rrn sendo um número entre 1 e n que representa a posição do registro no arquivo (de primeiro a último)
 * Parâmetros:
 * int rrn - Inteiro contendo a posição relativa do registro no arquivo
 */
void SearchRRN_Delimitador(int rrn);

/*
 * Função que imprime somente o campo representado por field dentro do registro representado por rrn, field deve ser um número entre 1 e 8, representando um determinado campo do registro, detalhado abaixo
 * Parâmetros:
 * int rrn - Inteiro contendo a posição relativa do registro no arquivo
 * int field - Inteiro contendo o número que representa um campo do registro, definido da seguinte maneira: {1 = domínio, 2 = documento, 3 = nome, 4 = uf, 5 = cidade, 6 = data do cadastro, 7 = data da última atualização, 8 = ticket}
 */
void SearchRRNandField_Delimitador(int rrn, int field);

/*
 * Função que imprime todos os registros onde se encomtram dados idênticos a data no campo indicado por field
 * Parâmetros:
 * wchar_t* data - String formada por wchar_t contendo um dado que deverá ser comparado aos dados do arquivo especificados por field
 * int field - Inteiro contendo o número que representa um campo do registro, definido da seguinte maneira: {1 = domínio, 2 = documento, 3 = nome, 4 = uf, 5 = cidade, 6 = data do cadastro, 7 = data da última atualização, 8 = ticket}
 */
void SearchDataandField_Delimitador(wchar_t* data, int field);




/*
 * Função que lê dados contidos em uma linha de file, um arquivo de entrada no formato csv, e escreve esses dados no registro r, passado por referência
 * Parâmetros:
 * Registro_Delimitador *reg - Registro r, já alocado, onde os valores serão escritos
 * FILE* file - Ponteiro para arquivo a ser lido
 */
void ReadCSV_delimitador(Registro_Delimitador *reg, FILE* file);


/*
 * Função que escreve os conteúdos do registro r, passado por referência, no arquivo apontado por saida
 * Parâmetros:
 * Registro_Delimitador *r - ponteiro do registro que contém os dados a serem escritos
 * FILE* saida - ponteiro para o arquivo aonde os dados serão escritos
 */
void WriteReg_Delimitador(Registro_Delimitador *r, FILE* saida);

#endif
