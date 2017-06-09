#ifndef DELIMITADOR_H
#define DELIMITADOR_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#define BUFFER_SIZE 100
#define LINE_SIZE 500

/*
 * Thiago de Oliveira Lopes - 8532381
 * ***AVISO***
 * Este c�digo requer que a locale utlizada seja uma que suporte os caracteres PT-BR para obter o funcionamento correto
 * A locale deve ser setada externamente
 * As locales recomendadas s�o a "pt_BR.UTF-8" ou a "pt_BR.ISO-8859-1" para plataformas UNIX e "Portuguese_Brazil.1252" para plataformas Windows
 */


/*
 * Struct contendo todos os campos de um registro
 */
typedef struct registro_delimitador Registro_Delimitador;

/*
 * Fun��o que l� um arquivo de nome dado por nome e gera um arquivo de nome "delimitador.bin", com registros separados pelo caractere "~"
 * Par�metros:
 * char* nome - nome do arquivo de entrada a ser lido
 */
void CreateFile_Delimitador(char* nome);

/*
 * Fun��o que imprime todos os registros do arquivo "delimitador.bin", com os campos separados por linha e pausa entre os registros (pressione enter para prosseguir)
 */
void PrintAll_Delimitador();

/*
 * Fun��o que imprime somente o registro indicado por rrn, rrn sendo um n�mero entre 1 e n que representa a posi��o do registro no arquivo (de primeiro a �ltimo)
 * Par�metros:
 * int rrn - Inteiro contendo a posi��o relativa do registro no arquivo
 */
void SearchRRN_Delimitador(int rrn);

/*
 * Fun��o que imprime somente o campo representado por field dentro do registro representado por rrn, field deve ser um n�mero entre 1 e 8, representando um determinado campo do registro, detalhado abaixo
 * Par�metros:
 * int rrn - Inteiro contendo a posi��o relativa do registro no arquivo
 * int field - Inteiro contendo o n�mero que representa um campo do registro, definido da seguinte maneira: {1 = dom�nio, 2 = documento, 3 = nome, 4 = uf, 5 = cidade, 6 = data do cadastro, 7 = data da �ltima atualiza��o, 8 = ticket}
 */
void SearchRRNandField_Delimitador(int rrn, int field);

/*
 * Fun��o que imprime todos os registros onde se encomtram dados id�nticos a data no campo indicado por field
 * Par�metros:
 * wchar_t* data - String formada por wchar_t contendo um dado que dever� ser comparado aos dados do arquivo especificados por field
 * int field - Inteiro contendo o n�mero que representa um campo do registro, definido da seguinte maneira: {1 = dom�nio, 2 = documento, 3 = nome, 4 = uf, 5 = cidade, 6 = data do cadastro, 7 = data da �ltima atualiza��o, 8 = ticket}
 */
void SearchDataandField_Delimitador(wchar_t* data, int field);

#endif
