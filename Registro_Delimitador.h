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
 * Este código requer que a locale utlizada seja uma que suporte os caracteres PT-BR para obter o funcionamento correto
 * A locale deve ser setada externamente
 * As locales recomendadas são a "pt_BR.UTF-8" ou a "pt_BR.ISO-8859-1" para plataformas UNIX e "Portuguese_Brazil.1252" para plataformas Windows
 */


/*
 * Struct contendo todos os campos de um registro
 */
typedef struct registro_delimitador Registro_Delimitador;

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

#endif
