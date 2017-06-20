#include "Registro_Delimitador.h"

typedef struct chprim {

	long int ticket;
	long int offset;
} ChPrim;

ChPrim* newChPrim(long int ticket, int offset) {
	ChPrim* new = (ChPrim*) malloc (sizeof(ChPrim));
	new->ticket = ticket;
	new->offset = offset;
	return new;
}

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

void createIndex(char* arq_saida, char* arq_ind) {

	FILE* reg = fopen(arq_saida, "rb");
	FILE* f = fopen(arq_saida, "rb");
	FILE* ind = fopen(arq_ind, "wb");

	char aux = 0;
    void* temp;

    int n = 0;
    int counter = 0;
    long int offset;

	Registro_Delimitador r;


    while(feof(reg)==0) {
    	// aux = fgetc(reg1);
    	fread(&aux, sizeof(char), 1, reg);
    	if ( aux == '~' && counter > (3*19+sizeof(long int)) ){
    		offset = ftell(f);
    		temp = malloc(sizeof(char) * counter);
    		fread(temp, sizeof(char), counter, f);
    		fgetc(f);
    		//printf("(%d) %d %ld\n", n++, counter, ftell(reg1));
    		counter = -1;
    		getReg(&r, temp);
 			//print_reg(&r);
 			fwrite(&r.ticket, sizeof(long int), 1, ind);
 			fwrite(&offset, sizeof(long int), 1, ind);

    	}

    	counter++;
    	aux = 0;

    }
    

    fclose(reg);
    fclose(f);
    fclose(ind);

    return;
}

void createIndexes() {

	createIndex("arquivo_de_saida_1.bin", "indice_1.bin");
    createIndex("arquivo_de_saida_2.bin", "indice_2.bin");
    createIndex("arquivo_de_saida_3.bin", "indice_3.bin");

    return;
    
}

void printIndex(char* name) {

	FILE* ind = fopen(name, "rb");
	long int ticket = -1;
	long int offset = -1;
	while(feof(ind)==0){
		fread(&ticket, sizeof(long int), 1, ind);
		fread(&offset, sizeof(long int), 1, ind);
		printf("%10ld %10ld", ticket, offset);
	}

	fclose(ind);
}



int main() {

	setlocale(LC_ALL, "pt_BR.ISO-8859-1");

	Create3Files_Delimitador("dados_entrada.csv");
	puts("Criando Indice");
	createIndexes();
	puts("Indice criado. Imprimindo: ...");
	//printIndex("indice_1.bin");
	// PrintAll_Delimitador2();
	puts("Fim");

	printf("%ld\n", sizeof(long int));
	printf("%ld\n", sizeof(char));


	// criar 3 arquivos de indice
	// onde fica o começo da pilha para remoção?

	return 0;
}