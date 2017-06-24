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

// Cria os 3 arquivos com delimitadores
void Create3Files_Delimitador(char* nome)
    {
    FILE* entrada = fopen(nome, "r");       //tentando abrir o arquivo dado por nome, e salvando seu ponteiro em entrada
    if (entrada==NULL)      //checando se a abertura foi bem sucedida
        {
        printf("Nome do arquivo invalido\n");       // se não foi, imprimindo mensagem de erro e saindo da função
        return;
        }
    FILE* saida1 = fopen("arquivo_de_saida_1.bin", "wb");       //Abrindo ou criando um arquivoi binario e salvando seu ponteiro em saida
    FILE* saida2 = fopen("arquivo_de_saida_2.bin", "wb");
    FILE* saida3 = fopen("arquivo_de_saida_3.bin", "wb");

    long int header = -1;
    fwrite(&header, sizeof(long int), 1, saida1);
    fwrite(&header, sizeof(long int), 1, saida2);
    fwrite(&header, sizeof(long int), 1, saida3);



    Registro_Delimitador r;         //declarando registro r que será passado para funções por referência
    ReadCSV_delimitador(&r, entrada);         //string de wide chars de tamanho limitado por LINE_SIZE
    while(feof(entrada)==0)        //Lendo uma linha do arquivo de entrada e salvando o conteúdo em str, repete até o final do arquivo
        {
        WriteReg_Delimitador(&r, saida1);        //chamando função de escrita dos dados
        WriteReg_Delimitador(&r, saida2);
        WriteReg_Delimitador(&r, saida3);
        ReadCSV_delimitador(&r, entrada);
        }
    fclose(entrada);        //fechando arquivos
    fclose(saida1);
    fclose(saida2);
    fclose(saida3);
    
    }

// Imprime registrador
void print_reg(Registro_Delimitador* reg) {

	printf("Dominio: %ls\n", reg->dominio);
	printf("Documento: %s\n", reg->documento);
	printf("Nome: %ls\n", reg->nome);
	printf("UF: %ls\n", reg->uf);
	printf("Cidade: %ls\n", reg->cidade);
	printf("Data do Cadastro: %s\n", reg->data_cadastro);
	printf("Ult. Atualizacao: %s\n", reg->data_atualiza);
	printf("Ticket: %ld\n", reg->ticket);
	printf("\n");
}


// Passa o conteudo de variavel que contem toda informacao do registro para uma struct
void getReg(Registro_Delimitador* r, void* data) {
	
	int size = 0;
	int temp = 0;

	size = 19 * sizeof(char);
	memcpy(r->documento, data, size);
	r->documento[19] = 0;

	data += size;
	memcpy(r->data_cadastro, data, 19);
	r->data_cadastro[19] = 0;

	data += size;
	memcpy(r->data_atualiza, data, 19);
	r->data_atualiza[19] = 0;

	data += size;
	size = sizeof(long int);
	memcpy(&r->ticket, data, size);



	data += size;
	size = sizeof(int);
	memcpy(&temp, data, size);

	data += size;
	size = temp * sizeof(wchar_t);
	if (temp == 0)
		wcscpy(r->dominio, L"nullo");
	else
		memcpy(r->dominio, data, size);
	r->dominio[temp] = 0;




	data += size;
	size = sizeof(int);
	memcpy(&temp, data, size);

	data += size;
	size = temp * sizeof(wchar_t);
	if (temp == 0)
		wcscpy(r->nome, L"nullo");
	else
		memcpy(r->nome, data, size);
	r->nome[temp] = 0;




	data += size;
	size = sizeof(int);
	memcpy(&temp, data, size);

	data += size;
	size = temp * sizeof(wchar_t);
	if (temp == 0)
		wcscpy(r->uf, L"nullo");
	else
		memcpy(r->uf, data, size);
	r->uf[temp] = 0;




	data += size;
	size = sizeof(int);
	memcpy(&temp, data, size);

	data += size;
	size = temp * sizeof(wchar_t);
	if (temp == 0)
		wcscpy(r->cidade, L"nullo");
	else
		memcpy(r->cidade, data, size);
	r->cidade[temp] = 0;


	return;

}

// Funcao de comparacao necessaria para utilizar os metodos qsort e bsearch
int compare (const void* a, const void* b) {
	return ((ChPrim*)a)->ticket - ((ChPrim*)b)->ticket;
}

// Cria um arquivo binario de indice de indice, a partir da leitura do arquivo de registros, utilizando o metodo de leitura pelos delimitadores
void createIndex(char* arq_saida, char* arq_ind) {

	FILE* reg = fopen(arq_saida, "rb"); // ponteiro para encontrar valores de cada registro
	FILE* f = fopen(arq_saida, "rb"); // ponteiro so para pegar o byte offset
	FILE* ind = fopen(arq_ind, "wb"); // ponteiro para escrever em arquivo

	char aux = 0;
    void* temp;

    int n = 0;
    int counter = 0;
    long int offset;

	Registro_Delimitador r;

	ChPrim* array = malloc(IBS * sizeof(ChPrim));

	int n_regs = 0;


	long int trash;
	fread(&trash, sizeof(long int), 1, reg); // pula o cabecalho
	fread(&trash, sizeof(long int), 1, f); // pula o cabecalho


    while(feof(reg)==0) {
    	
    	fread(&aux, sizeof(char), 1, reg);
    	if ( aux == '~' && counter > (3*19+sizeof(long int)) ){
    		offset = ftell(f); // pega offset do inicio do registro
    		temp = malloc(sizeof(char) * counter);
    		fread(temp, sizeof(char), counter, f); // le todo o registro dentro de uma variavel
    		fgetc(f); // pula o delimitador, mantendo-se no inicio do proximo registrador

    		counter = -1;
    		getReg(&r, temp); // pega conteudo da variavel temp e coloca em um registro de verdade



    		// Verifica se mais espaco eh necessario
    		if (n_regs == IBS)
    			array = realloc(array, (n_regs + IBS) * sizeof(ChPrim));

    		array[n_regs].ticket = r.ticket;
    		array[n_regs].offset = offset; 
    		

 			n_regs++;

    	}

    	counter++;
    	aux = 0;

    }


    qsort(array, n_regs, sizeof(ChPrim), compare);




    for (int i = 0; i < n_regs; ++i) {
    	fwrite(&array[i].ticket, sizeof(long int), 1, ind);
    	fwrite(&array[i].offset, sizeof(long int), 1, ind);
    }

    fclose(reg);
    fclose(f);
    fclose(ind);

    return;
}

// Cria os tres arquivos de indice
void createIndexes() {

	createIndex("arquivo_de_saida_1.bin", "indice_1.bin");
    createIndex("arquivo_de_saida_2.bin", "indice_2.bin");
    createIndex("arquivo_de_saida_3.bin", "indice_3.bin");

    return;
    
}

// Imprime arquivo de indice
void printIndexFile(char* name) {

	FILE* ind = fopen(name, "rb");
	long int ticket = -1;
	long int offset = -1;

	int i = 0;


	while(fread(&ticket, sizeof(long int), 1, ind)!=0){
		
		fread(&offset, sizeof(long int), 1, ind);
		printf("(%3d) %10ld %10ld\n", i++, ticket, offset);
	}

	fclose(ind);
}


// Essa funcao atualiza o valor do cabecalho com o offset do ultimo registro removido logicamente.
// O retorno da funcao eh o valor que estava previamente no cabecalho
long int updateHeader(Group* g, long int offset) {

	long int next_offset = -12341234;

	fseek(g->file, 0, SEEK_SET); //vai para o inicio do cabecalho
	fread(&next_offset, sizeof(long int), 1, g->file); // le informacao do cabecalho

	fseek(g->file, 0, SEEK_SET); //volta para o inicio do cabecalho
	if (fwrite(&offset, sizeof(long int), 1, g->file) == 0)
		puts("Erro ao atualizar cabecalho"); // atualiza cabecalho


	return next_offset;
}


// Remove logicamente um registro do arquivo e remove fisicamente a chave correspondente
int removerReg(Group* g, long int ticket) {


	// 1. Encontrar a chave no array	
 	ChPrim* ch = bsearch(&ticket, g->array, g->length, sizeof(ChPrim), compare); // esse ch deve receber o ultimo da array
 	if (ch == NULL)
 		return -1;

 	// printf("offset: %ld\n", ch->offset);

 	// 2. Obter tamanho do registro
 	fseek(g->file, ch->offset, SEEK_SET);
    int counter = 0; // tamanho do registro removido logicamente
    char aux = 0;
    while (feof(g->file) == 0 && aux != '~' ) {
    	counter++;
    	fread(&aux, sizeof(char), 1, g->file);
    }
    if ( aux != '~' || counter <= (3*19+sizeof(long int)) )
    	return -2; // Se esse erro acontecer, entao ha problemas nas funcoes de escrita / insercao ou na escolha do char delimitador

    // printf("tamanho: %d\n", counter);

    // 3. Pega valor que esta no cacalho e atualiza-o
    long int next_offset = updateHeader(g, ch->offset); // offset do proximo registro removido logicamente

    // printf("next offset: %ld\n", next_offset);

    // 4. Faz a remocao logica
    fseek(g->file, ch->offset, SEEK_SET); // volta para o inicio do registro
 	putc('*', g->file); // escreve caracter de remocao logica
 	fwrite(&counter, sizeof(int), 1, g->file); // escrever tamanho do registro removido logicamente
 	fwrite(&next_offset, sizeof(long int), 1, g->file); // escreve offset do proximo registro removido logicamente

 	// puts("removido");

 	// Ajusta a estrutura do indice em RAM
 	fseek(g->file, 0, SEEK_SET); // volta para o ponteiro do arquivo para o lugar inicial. Não é estritamente necessário.
 	*ch = g->array[g->length - 1]; // ultimo elemento do array ocupa lugar do elemento removido
 	g->length--; // atualiza valor da sruct
 	qsort(g->array, g->length, sizeof(ChPrim), compare); // ordena o array

 	// puts("ajustado");

 	return 0;
}

// Pega o indice que estava em disco e coloca em um array.
void readIndex(Group* g, char* f_name) {

	FILE* ind = fopen(f_name, "rb");

	ChPrim* array = malloc(IBS * sizeof(ChPrim));

	long int ticket = -1;
	long int offset = -1;
	int n_regs = 0;

	while(fread(&ticket, sizeof(long int), 1, ind)!=0){
		
		fread(&offset, sizeof(long int), 1, ind);
		array[n_regs].ticket = ticket;
    	array[n_regs].offset = offset;
    	n_regs++;
		
	}

	fclose(ind);

	g->array = array;
	g->length = n_regs;
}


void showStatus(Group* g) {

	for (int i = 0; i < g->length; ++i)
	{	
		printf("(%3d) %10ld %10ld\n", i, g->array[i].ticket, g->array[i].offset);
	}

	long int header = -99;

	fseek(g->file, 0, SEEK_SET); //vai para o inicio do cabecalho
	fread(&header, sizeof(long int), 1, g->file); // le informacao do cabecalho

	fseek(g->file, 0, SEEK_SET); //vai para o inicio do cabecalho

	printf("cabecalho:  %10ld\n",  header);
	printf("length:  %5d\n",  g->length);

}

int main() {

	setlocale(LC_ALL, "pt_BR.ISO-8859-1");

	Create3Files_Delimitador("dados_entrada.csv");
	puts("Criando Indice");
	createIndexes();

	Group g[3];
	g[0].file = fopen("arquivo_de_saida_1.bin", "r+b");
	readIndex(&g[0], "indice_1.bin");
	g[1].file = fopen("arquivo_de_saida_2.bin", "r+b");
	readIndex(&g[1], "indice_2.bin");
	g[2].file = fopen("arquivo_de_saida_3.bin", "r+b");
	readIndex(&g[2], "indice_3.bin");




	// puts("Indice criado. Imprimindo: ...");
	// printIndexFile("indice_1.bin");
	
	puts("status: ...");
	removerReg(&g[0], 0);
	showStatus(&g[0]);
	// PrintAll_Delimitador2();
	
	puts("Fim");


	// criar 3 arquivos de indice
	// onde fica o começo da pilha para remoção?

	return 0;
}