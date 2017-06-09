#include "Registro_Delimitador.h"

/* Os includes e defines a seguir foram movidos para o .h, mas vou mant�-los salvo como coment�rio caso seja �til
 *   #include <stdio.h>
 *   #include <stdlib.h>
 *   #include <string.h>
 *   #include <wchar.h>
 *   #include <locale.h>
 *   #define BUFFER_SIZE 100
 *   #define LINE_SIZE 500
 */

/* Defini��o de uma struct capaz de armazenar todos os campos de um registro do arquivo
 * A escolha dos tipos e do tamanho dos vetores foi baseado em uma an�lise dos dados
 */
struct registro_delimitador{
    wchar_t dominio [BUFFER_SIZE]; //O campo "dom�nio" pode receber wide chars e � de tamanho v�riavel, limitado por BUFFER_SIZE
    char documento [20];            //O campo "documento" espera somente chars padr�es e � de tamanho 19, o vetor recebe um char extra para colocar o caractere terminador, facilitando o manuseamento
    wchar_t nome [BUFFER_SIZE];     //O campo "nome" pode receber wide chars e � de tamanho v�riavel, limitado por BUFFER_SIZE
    wchar_t uf [BUFFER_SIZE];       //O campo "uf" pode receber wide chars e � de tamanho v�riavel, limitado por BUFFER_SIZE
    wchar_t cidade[BUFFER_SIZE];    //O campo "cidade" pode receber wide chars e � de tamanho v�riavel, limitado por BUFFER_SIZE
    char data_cadastro [20];        //O campo "documento" espera somente chars padr�es e � de tamanho 19, o vetor recebe um char extra para colocar o caractere terminador, facilitando o manuseamento
    char data_atualiza [20];        //O campo "documento" espera somente chars padr�es e � de tamanho 19, o vetor recebe um char extra para colocar o caractere terminador, facilitando o manuseamento
    long int ticket;                //O campo "ticket" espera um long int
    };

/*
 * O typedef a seguir foi movido para o .h, mas vou mant�-lo salvo como coment�rio caso seja �til
 *  typedef struct registro_delimitador Registro_Delimitador;
 */

/*
 * Fun��o que l� dados contidos em uma linha de file, um arquivo de entrada no formato csv, e escreve esses dados no registro r, passado por refer�ncia
 * Par�metros:
 * Registro_Delimitador *reg - Registro r, j� alocado, onde os valores ser�o escritos
 * FILE* file - Ponteiro para arquivo a ser lido
 */
void ReadCSV_delimitador(Registro_Delimitador *reg, FILE* file)
    {
    fscanf(file, " %l[^;]%*[;]", reg->dominio);        //lendo os dados do arquivo e salvando no regitro reg
    fscanf(file, "%[^;]%*[;]", reg->documento);
    fscanf(file, "%l[^;]%*[;]", reg->nome);
    fscanf(file, "%l[^;]%*[;]", reg->uf);
    fscanf(file, "%l[^;]%*[;]", reg->cidade);
    fscanf(file, "%[^;]%*[;]", reg->data_cadastro);
    fscanf(file, "%[^;]%*[;]", reg->data_atualiza);
    fscanf(file, "%ld", &(reg->ticket));
    fgetc(file);        //jogando fora o newline
    }

/*
 * Fun��o que escreve os conte�dos do registro r, passado por refer�ncia, no arquivo apontado por saida
 * Par�metros:
 * Registro_Delimitador *r - ponteiro do registro que cont�m os dados a serem escritos
 * FILE* saida - ponteiro para o arquivo aonde os dados ser�o escritos
 */
void WriteReg_Delimitador(Registro_Delimitador *r, FILE* saida)
    {
    int i;                                     //Inteiro de uso gen�rico, utlizado para armazenar o tamanho dos campos de tamanho variavel
    wchar_t nulo[5] = {'n', 'u', 'l', 'l', 0};  //String de wide char utilizada para compara��es
    char nuloc[19] = {'n', 'u', 'l', 'l', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};  //String de char utlizada para compara��es e para escrita, cheia de caracteres terminadores para evitar a possibilidade do caractere reservado ser acidentalmente escrito
    char breaker = '~';         //Caractere reservado para separa��o de registros
    if(strcmp(r->documento, nuloc)!=0)      //comparando strings para checar se o campo � vazio
        fwrite(r->documento, sizeof(char), 19, saida);      //escrevendo o valor do campo
    else
        fwrite(nuloc, sizeof(char), 19, saida);         //escrevendo nulo, pois � um campo de tamanho fixo
    if(strcmp(r->data_cadastro, nuloc)!=0)      //comparando strings para checar se o campo � vazio
        fwrite(r->data_cadastro, sizeof(char), 19, saida);      //escrevendo o valor do campo
    else
        fwrite(nuloc, sizeof(char), 19, saida);     //escrevendo nulo, pois � um campo de tamanho fixo
    if(strcmp(r->data_atualiza, nuloc)!=0)      //idem acima
        fwrite(r->data_atualiza, sizeof(char), 19, saida);
    else
        fwrite(nuloc, sizeof(char), 19, saida);
    fwrite(&(r->ticket), sizeof(long int), 1, saida);       //escrevendo o valor do campo ticket
    if(wcscmp(r->dominio, nulo)!=0)     //comparando strings para checar se o campo � vazio
        {
        i = wcslen(r->dominio);             //escrevendo o indicador de tamanho e seu respectivo campo se n�o for vazio
        fwrite(&i, sizeof(int), 1, saida);
        fwrite(r->dominio, sizeof(wchar_t), i, saida);
        }
    else
        {
        i=0;                //escrevendo o indicador de tamanho como 0 se o campo for vazio
        fwrite(&i, sizeof(int), 1, saida);
        }
    if(wcscmp(r->nome, nulo)!=0)        //comparando strings para checar se o campo � vazio
        {
        i = wcslen(r->nome);            //escrevendo o indicador de tamanho e seu respectivo campo se n�o for vazio
        fwrite(&i, sizeof(int), 1, saida);
        fwrite(r->nome, sizeof(wchar_t), i, saida);
        }
    else
        {
        i=0;            //escrevendo o indicador de tamanho como 0 se o campo for vazio
        fwrite(&i, sizeof(int), 1, saida);
        }
    if(wcscmp(r->uf, nulo)!=0)          //idem acima
        {
        i = wcslen(r->uf);
        fwrite(&i, sizeof(int), 1, saida);
        fwrite(r->uf, sizeof(wchar_t), i, saida);
        }
    else
        {
        i=0;
        fwrite(&i, sizeof(int), 1, saida);
        }
    if(wcscmp(r->cidade, nulo)!=0)          //idem acima
        {
        i = wcslen(r->cidade);
        fwrite(&i, sizeof(int), 1, saida);
        fwrite(r->cidade, sizeof(wchar_t), i, saida);
        }
    else
        {
        i=0;
        fwrite(&i, sizeof(int), 1, saida);
        }
    fwrite(&breaker, sizeof(char), 1, saida);           //escrevendo o caractere delimitador
    }

/*
 * Fun��o que l� um arquivo de nome dado por nome e gera um arquivo de nome "delimitador.bin", com registros separados pelo caractere "~"
 * Par�metros:
 * char* nome - nome do arquivo de entrada a ser lido
 */
void CreateFile_Delimitador(char* nome)
    {
    FILE* entrada = fopen(nome, "r");       //tentando abrir o arquivo dado por nome, e salvando seu ponteiro em entrada
    if (entrada==NULL)      //checando se a abertura foi bem sucedida
        {
        printf("Nome do arquivo invalido\n");       // se n�o foi, imprimindo mensagem de erro e saindo da fun��o
        return;
        }
    FILE* saida = fopen("delimitador.bin", "wb");       //Abrindo ou criando um arquivoi binario e salvando seu ponteiro em saida
    Registro_Delimitador r;         //declarando registro r que ser� passado para fun��es por refer�ncia
    ReadCSV_delimitador(&r, entrada);         //string de wide chars de tamanho limitado por LINE_SIZE
    while(feof(entrada)==0)        //Lendo uma linha do arquivo de entrada e salvando o conte�do em str, repete at� o final do arquivo
        {
        WriteReg_Delimitador(&r, saida);        //chamando fun��o de escrita dos dados
        ReadCSV_delimitador(&r, entrada);
        }
    fclose(entrada);        //fechando arquivos
    fclose(saida);
    }

/*
 * Fun��o que imprime todos os registros do arquivo "delimitador.bin", com os campos separados por linha e pausa entre os registros (pressione enter para prosseguir)
 */
void PrintAll_Delimitador()
    {
    FILE* saida = fopen("delimitador.bin", "rb");       //Abrindo arquivo para leitura
    Registro_Delimitador r;     //Declarando registro que receber� dados
    int fieldsize, counter=0;    //Declarando inteiros, fieldsize receber� o valor dos indicadores de tamanho dos campos, counter armazena o RRN do registro atual
    wchar_t nulo[] = {'n', 'u', 'l', 'l', 0};       //String de wide char usada para compara��es e escrita no registro r
    char trash[BUFFER_SIZE];        //String de char usada como uma "lixeira" para valores n�o utilizados
    while(fread(r.documento, sizeof(char), 19, saida)!=0)      //l� o primeiro campo do registro, repete at� o fim do arquivo
        {
        counter++;      //incrementando rrn
        r.documento[19] = 0;                        //adicionando um caractere terminador a string do campo do registro r
        fread(r.data_cadastro, sizeof(char), 19, saida);        //lendo campo de tamanho fixo do arquivo
        r.data_cadastro[19] = 0;        //adicionando um caractere terminador a string do campo do registro r
        fread(r.data_atualiza, sizeof(char), 19, saida);       //idem acima
        r.data_atualiza[19] = 0;
        fread(&(r.ticket), sizeof(long int), 1, saida);     //lendo campo de tamanho fixo do arquivo
        fread(&fieldsize, sizeof(int), 1, saida);   //lendo o indicador de tamanho do campo
        if(fieldsize==0)        //checando se o campo � vazio
            wcscpy(r.dominio, nulo);        //lendo nulo se for vazio
        else
            {
            fread(r.dominio, sizeof(wchar_t), fieldsize, saida);        //lendo o campo do arquivo se n�o for vazio
            r.dominio[fieldsize] = 0;                   //adicionando um caractere terminador a string do campo do registro r
            }
        fread(&fieldsize, sizeof(int), 1, saida);   //l� o indicador de tamanho do campo do registro
        if(fieldsize==0)            //checando se o campo � vazio
            wcscpy(r.nome, nulo);       //lendo nulo se for vazio
        else
            {
            fread(r.nome, sizeof(wchar_t), fieldsize, saida);       //lendo o campo do arquivo se n�o for vazio
            r.nome[fieldsize] = 0;              //adicionando um caractere terminador a string do campo do registro r
            }
        fread(&fieldsize, sizeof(int), 1, saida);       //idem acima
        if(fieldsize==0)
            wcscpy(r.uf, nulo);
        else
            {
            fread(r.uf, sizeof(wchar_t), fieldsize, saida);
            r.uf[fieldsize] = 0;
            }
        fread(&fieldsize, sizeof(int), 1, saida);       //idem acima
        if(fieldsize==0)
            wcscpy(r.cidade, nulo);
        else
            {
            fread(r.cidade, sizeof(wchar_t), fieldsize, saida);
            r.cidade[fieldsize] = 0;
            }
        fread(trash, sizeof(char), 1, saida);       //lendo caractere delimitador entre os registros
        printf("(%d)\nDominio: %ls\nDocumento: %s\nNome: %ls\nUF: %ls\nCidade: %ls\nData do Cadastro: %s\nUlt. Atualizacao: %s\nTicket: ", counter, r.dominio, r.documento, r.nome, r.uf, r.cidade, r.data_cadastro, r.data_atualiza);     //imprimindo dados do registro
        if(r.ticket!=0)                 //checando se ticket � vazio (0)
            printf("%ld\n\n", r.ticket);      //imprimindo ticket se n�o for vazio
        else
            printf("null\n\n");           //imprimindo "null" se for vazio
        fgets(trash, BUFFER_SIZE, stdin);       //pausa para o usu�rio ler o registro imprimido, pressione enter para continuar
        }
    fclose(saida);      //fechando o arquivo
    }

/*
 * Fun��o que imprime somente o registro indicado por rrn, rrn sendo um n�mero entre 1 e n que representa a posi��o do registro no arquivo (de primeiro a �ltimo)
 * Par�metros:
 * int rrn - Inteiro contendo a posi��o relativa do registro no arquivo
 */
void SearchRRN_Delimitador(int rrn)
    {
    if(rrn <1)      //checando se o rrn n�o � inv�lido
        {
        printf("RRN inv�lido\n");       //imprimindo erro se for
        return;         //saindo da fun��o
        }
    FILE* saida = fopen("delimitador.bin", "rb");       //idem a fun��o anterior
    Registro_Delimitador r;                     //idem a fun��o anterior
    wchar_t nulo[] = {'n', 'u', 'l', 'l', 0};       //idem a fun��o anterior
    char aux=0;         //char que receber� caracteres lidos do arquivo
    char trash[BUFFER_SIZE];        //idem a fun��o anterior
    int i = rrn;        //salvando o valor do rrn
    int fieldsize;      //idem a fun��o anterior
    while(rrn>1)        //checa se estamos no rrn correto, repete at� chegarmos no rrn correto
        {
        rrn--;          //decrementa o rrn
        while((aux!='~')&&(feof(saida)==0))     //checa se lemos o caractere delimitador ou chegamos ao fim do arquivo, repete at� um dos dois acontecer
            aux = fgetc(saida);             //l� um caractere do arquivo e salva se valor em aux
        if(aux!='~')        //checa se aux � o caractere delimitador, se n�o for, siginifica que chegamos ao fim do arquivo
            {
            printf("RRN inv�lido\n");       //imprime erro
            fclose(saida);      //fecha arquivo
            return;         //sai da fun��o
            }
        aux=0;
        }
    fread(r.documento, sizeof(char), 19, saida);        //l� e depois imprime todos os campos do registro, idem a fun��o anterior
    r.documento[19] = 0;
    fread(r.data_cadastro, sizeof(char), 19, saida);
    r.data_cadastro[19] = 0;
    fread(r.data_atualiza, sizeof(char), 19, saida);
    r.data_atualiza[19] = 0;
    fread(&(r.ticket), sizeof(long int), 1, saida);
    fread(&fieldsize, sizeof(int), 1, saida);
    if(fieldsize==0)
        wcscpy(r.dominio, nulo);
    else
        {
        fread(r.dominio, sizeof(wchar_t), fieldsize, saida);
        r.dominio[fieldsize] = 0;
        }
    fread(&fieldsize, sizeof(int), 1, saida);
    if(fieldsize==0)
        wcscpy(r.nome, nulo);
    else
        {
        fread(r.nome, sizeof(wchar_t), fieldsize, saida);
        r.nome[fieldsize] = 0;
        }
    fread(&fieldsize, sizeof(int), 1, saida);
    if(fieldsize==0)
        wcscpy(r.uf, nulo);
    else
        {
        fread(r.uf, sizeof(wchar_t), fieldsize, saida);
        r.uf[fieldsize] = 0;
        }
    fread(&fieldsize, sizeof(int), 1, saida);
    if(fieldsize==0)
        wcscpy(r.cidade, nulo);
    else
        {
        fread(r.cidade, sizeof(wchar_t), fieldsize, saida);
        r.cidade[fieldsize] = 0;
        }
    fread(trash, sizeof(char), 1, saida);
    printf("(%d)\nDominio: %ls\nDocumento: %s\nNome: %ls\nUF: %ls\nCidade: %ls\nData do Cadastro: %s\nUlt. Atualizacao: %s\nTicket: ", i, r.dominio, r.documento, r.nome, r.uf, r.cidade, r.data_cadastro, r.data_atualiza);
    if(r.ticket!=0)
        printf("%ld\n\n", r.ticket);
    else
        printf("null\n\n");
    fgets(trash, BUFFER_SIZE, stdin);
    fclose(saida);
    }

/*
 * Fun��o que imprime somente o campo representado por field dentro do registro representado por rrn, field deve ser um n�mero entre 1 e 8, representando um determinado campo do registro, detalhado abaixo
 * Par�metros:
 * int rrn - Inteiro contendo a posi��o relativa do registro no arquivo
 * int field - Inteiro contendo o n�mero que representa um campo do registro, definido da seguinte maneira: {1 = dom�nio, 2 = documento, 3 = nome, 4 = uf, 5 = cidade, 6 = data do cadastro, 7 = data da �ltima atualiza��o, 8 = ticket}
 */
void SearchRRNandField_Delimitador(int rrn, int field)
    {
    FILE* saida = fopen("delimitador.bin", "rb");       //idem a fun��o anterior
    char aux=0;                             //idem a fun��o anterior
    wchar_t data_str[BUFFER_SIZE];          //vetor capaz de receber uma string de wide char contendo um dos dados do arquivo
    wchar_t nulo[] = {'n', 'u', 'l', 'l', 0};       //idem a fun��o anterior
    char data_c[20];               //vetor capaz de receber uma string de char contendo um dos dados do arquivo
    long int data_num;                      //long int capaz de receber o dado de um dos campos do arquivo
    int i = rrn;                    //idem a fun��o anterior
    int fieldsize;                  //idem a fun��o anterior
    while(rrn>1)            //idem a fun��o anterior
        {
        rrn--;
        while((aux!='~')&&(feof(saida)==0))
            fread(&aux, sizeof(char), 1, saida);
        if(aux!='~')
            {
            printf("RRN inv�lido");
            fclose(saida);
            return;
            }
        aux = 0;
        }
    if(field == 2)         //checa se esse � o campo buscado
        {
        fread(data_c, sizeof(char), 19, saida);     //l� o campo buscado
        data_c[19] = 0;             //adiciona um caractere terminador
        printf("(%d)\nDocumento: %s\n\n", i, data_c);     //imprime o campo
        fclose(saida);       //fecha o arquivo
        return;         //sai da fun��o
        }
    else
        fseek(saida, 19*sizeof(char), SEEK_CUR);      //move o ponteiro do arquivo para frente se esse n�o for o campo buscado
    if(field == 6)      //idem acima
        {
        fread(data_c, sizeof(char), 19, saida);
        data_c[19] = 0;
        printf("(%d)\nData do Cadastro: %s\n\n", i, data_c);
        fclose(saida);
        return;
        }
    else
        fseek(saida, 19*sizeof(char), SEEK_CUR);        //idem acima
    if(field == 7)      //idem acima
        {
        fread(data_c, sizeof(char), 19, saida);
        data_c[19] = 0;
        printf("(%d)\nUlt. Atualizacao: %s\n\n", i, data_c);
        fclose(saida);
        return;
        }
    else
        fseek(saida, 19*sizeof(char), SEEK_CUR);        //idem acima
    if(field == 8)      //idem acima
        {
        fread(&data_num, sizeof(long int), 1, saida);
        if(data_num!=0)
            printf("(%d)\nTicket: %ld\n\n", i, data_num);
        else
            printf("(%d)\nTicket: null\n\n", i);
        fclose(saida);
        return;
        }
    else
        fseek(saida, 1*sizeof(long int), SEEK_CUR);     //idem acima
    fread(&fieldsize, sizeof(int), 1, saida);       //l� o indicador de tamanho do primeiro campo do registro
    if(field == 1)
        {
        if(fieldsize!=0)        //checa se o campo n�o � vazio
            {
            fread(data_str, sizeof(wchar_t), fieldsize, saida);         //l� o campo buscado
            data_str[fieldsize] = 0;                //adiciona um caractere terminador
            printf("(%d)\nDominio: %ls\n\n", i, data_str);      //imprime o campo
            fclose(saida);          //fecha o arquivo
            return;         //sai da fun��o
            }
        else
            {
            printf("(%d)\nDominio: %ls\n\n", i, nulo);      //imprime nulo
            fclose(saida);          //fecha o arquivo
            return;         //sai da fun��o
            }
        }
    else
        fseek(saida, fieldsize*sizeof(wchar_t), SEEK_CUR);      //move o ponteiro do arquivo para frente se esse n�o for o campo buscado
    fread(&fieldsize, sizeof(int), 1, saida);           //l� o indicador de tamanho do campo
    if(field == 3)          //idem acima
        {
        if(fieldsize!=0)        //checa se o campo n�o � vazio
            {
            fread(data_str, sizeof(wchar_t), fieldsize, saida);         //l� o campo buscado
            data_str[fieldsize] = 0;                //adiciona um caractere terminador
            printf("(%d)\nNome: %ls\n\n", i, data_str);      //imprime o campo
            fclose(saida);          //fecha o arquivo
            return;         //sai da fun��o
            }
        else
            {
            printf("(%d)\nNome: %ls\n\n", i, nulo);      //imprime nulo
            fclose(saida);          //fecha o arquivo
            return;         //sai da fun��o
            }
        }
    else
        fseek(saida, fieldsize*sizeof(wchar_t), SEEK_CUR);      //idem acima
    fread(&fieldsize, sizeof(int), 1, saida);           //idem acima
    if(field == 4)          //idem acima
        {
        if(fieldsize!=0)        //checa se o campo n�o � vazio
            {
            fread(data_str, sizeof(wchar_t), fieldsize, saida);         //l� o campo buscado
            data_str[fieldsize] = 0;                //adiciona um caractere terminador
            printf("(%d)\nUF: %ls\n\n", i, data_str);      //imprime o campo
            fclose(saida);          //fecha o arquivo
            return;         //sai da fun��o
            }
        else
            {
            printf("(%d)\nUF: %ls\n\n", i, nulo);      //imprime nulo
            fclose(saida);          //fecha o arquivo
            return;         //sai da fun��o
            }
        }
    else
        fseek(saida, fieldsize*sizeof(wchar_t), SEEK_CUR);      //idem acima
    fread(&fieldsize, sizeof(int), 1, saida);       //idem acima
    if(field == 5)      //idem acima
        {
        if(fieldsize!=0)        //checa se o campo n�o � vazio
            {
            fread(data_str, sizeof(wchar_t), fieldsize, saida);         //l� o campo buscado
            data_str[fieldsize] = 0;                //adiciona um caractere terminador
            printf("(%d)\nCidade: %ls\n\n", i, data_str);      //imprime o campo
            fclose(saida);          //fecha o arquivo
            return;         //sai da fun��o
            }
        else
            {
            printf("(%d)\nCidade: %ls\n\n", i, nulo);      //imprime nulo
            fclose(saida);          //fecha o arquivo
            return;         //sai da fun��o
            }
        }
    else
        fseek(saida, fieldsize*sizeof(wchar_t), SEEK_CUR);      //idem acima
    printf("Erro: Campo invalido\n");       //imprime erro
    fclose(saida);      //fecha arquivo
    }

/*
 * Fun��o que imprime todos os registros onde se encomtram dados id�nticos a data no campo indicado por field
 * Par�metros:
 * wchar_t* data - String formada por wchar_t contendo um dado que dever� ser comparado aos dados do arquivo especificados por field
 * int field - Inteiro contendo o n�mero que representa um campo do registro, definido da seguinte maneira: {1 = dom�nio, 2 = documento, 3 = nome, 4 = uf, 5 = cidade, 6 = data do cadastro, 7 = data da �ltima atualiza��o, 8 = ticket}
 */
void SearchDataandField_Delimitador(wchar_t* data, int field)
    {
    if((field<1)||(field>8))        //checa se o campo passado � v�lido
        {
        printf("Erro: Campo invalido\n");       //imprime erro
        return;     //sai da fun��o
        }
    FILE* saida = fopen("delimitador.bin", "rb");       //idem acima
    char datac[BUFFER_SIZE];        //Declara um vetor capaz de receber uma string de char contendo uma convers�o de data
    wcstombs(datac, data, BUFFER_SIZE);     //Converte data de uma string de wide char para uma string de char e salva o resultado em datac
    datac[wcslen(data)] = 0;        //insere um caractere terminador
    long ticket = atol(datac);      //converte o valor de datac para um long int e salva o resultado em ticket
    Registro_Delimitador r;         //declara um registro r capaz de armazenar os dados de um registro lido do arquivo
    int fieldsize, counter=0, flag=0;        //declara inteiros, fieldsize idem a fun��es anteriores, counter idem, flag quando 0 marca o registro para impress�o, quando 1, n�o imprime
    wchar_t nulo[] = {'n', 'u', 'l', 'l', 0};       //idem a fun��o anterior
    char trash[BUFFER_SIZE];            //idem a fun��es anteriores
    while(fread(r.documento, sizeof(char), 19, saida)!=0)      //idem a fun��es anteriores
        {
        counter++;          //idem a fun��es anteriores
        r.documento[19] = 0;
        if ((field==2)&&(strcmp(datac, r.documento)!=0))        //checa se esse � o campo que tem que ser comparado, e se ele � diferente de data
            flag = 1;
        if(flag==0)             //se a flag � zero, l� o campo normalmente
            {
            fread(r.data_cadastro, sizeof(char), 19, saida);
            r.data_cadastro[19] = 0;
            if ((field==6)&&(strcmp(datac, r.data_cadastro)!=0))                //checa se esse � o campo que tem que ser comparado, e se ele � diferente de data
                flag = 1;
            }
        else
            fseek(saida, 19*sizeof(char), SEEK_CUR);                //idem acima
        if(flag==0)             //idem acima
            {
            fread(r.data_atualiza, sizeof(char), 19, saida);
            r.data_atualiza[19] = 0;
            if ((field==7)&&(strcmp(datac, r.data_atualiza)==0))            //idem acima
                flag = 1;
            }
        else
            fseek(saida, 19*sizeof(char), SEEK_CUR);                //idem acima
        if(flag==0)             //idem acima
            {
            fread(&(r.ticket), sizeof(long int), 1, saida);             //idem acima
            if ((field==8)&&(ticket==r.ticket))                     //idem acima
                flag = 1;
            }
        else
            fseek(saida, 1*sizeof(long int), SEEK_CUR);             //idem acima
        fread(&fieldsize, sizeof(int), 1, saida);    //idem a fun��es anteriores
        if(flag==0)         //se a flag � zero, l� o campo normalmente
            {
            if(fieldsize==0)        //idem a fun��es anteriores
                wcscpy(r.dominio, nulo);        //idem a fun��es anteriores
            else
                {
                fread(r.dominio, sizeof(wchar_t), fieldsize, saida);        //idem a fun��es anteriores
                r.dominio[fieldsize] = 0;               //idem a fun��es anteriores
                }
            if ((field==1)&&(wcscmp(data, r.dominio)!=0))       //checa se esse � o campo que tem que ser comparado, e se ele � diferente de data
                flag = 1;           //se sim, a flag vira 1 e o campo n�o ser� imprimido
            }
        else
            fseek(saida, fieldsize*sizeof(wchar_t), SEEK_CUR);        //sen�o, pula o campo
        fread(&fieldsize, sizeof(int), 1, saida);       //idem a fun��es anteriores
        if(flag==0)         //idem acima
            {
            if(fieldsize==0)            //idem acima
                wcscpy(r.nome, nulo);
            else
                {
                fread(r.nome, sizeof(wchar_t), fieldsize, saida);           //idem acima
                r.nome[fieldsize] = 0;
                }
            if ((field==3)&&(wcscmp(data, r.nome)!=0))              //idem acima
                flag = 1;
            }
        else
            fseek(saida, fieldsize*sizeof(wchar_t), SEEK_CUR);          //idem acima
        fread(&fieldsize, sizeof(int), 1, saida);               //idem acima
        if(flag==0)                 //idem acima
            {
            if(fieldsize==0)
                wcscpy(r.uf, nulo);         //idem acima
            else
                {
                fread(r.uf, sizeof(wchar_t), fieldsize, saida);         //idem acima
                r.uf[fieldsize] = 0;
                }
            if ((field==4)&&(wcscmp(data, r.uf)!=0))                //idem acima
                flag = 1;
            }
        else
            fseek(saida, fieldsize*sizeof(wchar_t), SEEK_CUR);          //idem acima
        fread(&fieldsize, sizeof(int), 1, saida);           //idem acima
        if(flag==0)             //idem acima
            {
            if(fieldsize==0)            //idem acima
                wcscpy(r.cidade, nulo);
            else
                {
                fread(r.cidade, sizeof(wchar_t), fieldsize, saida);         //idem acima
                r.cidade[fieldsize] = 0;
                }
            if ((field==5)&&(wcscmp(data, r.cidade)!=0))            //idem acima
                flag = 1;
            }
        else
            fseek(saida, fieldsize*sizeof(wchar_t), SEEK_CUR);          //idem acima
        fread(trash, sizeof(char), 1, saida);               //idem a fun��es anteriores
        if(flag==0)                 //se a flag for zero, imprime os conteudos salvos em r
            {
            printf("(%d)\nDominio: %ls\nDocumento: %s\nNome: %ls\nUF: %ls\nCidade: %ls\nData do Cadastro: %s\nUlt. Atualizacao: %s\nTicket: ", counter, r.dominio, r.documento, r.nome, r.uf, r.cidade, r.data_cadastro, r.data_atualiza); //idem a fun��es anteriores
            if(r.ticket!=0)
                printf("%ld\n\n", r.ticket);      //idem a fun��es anteriores
            else
                printf("null\n\n");
            fgets(trash, BUFFER_SIZE, stdin);           //idem a fun��es anteriores
            }
        flag = 0;               //reseta a flag para zero
        }
    fclose(saida);
    }
