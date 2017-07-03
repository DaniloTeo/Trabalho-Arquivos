


#include "Registro_Delimitador.h"
#include "dac.h"

#define IBS 1000

// Tobias Mesquita Silva da Veiga - 5268356
// Danilo da Costa Telles Teo - 9293626
// Marcel Otoboni de Lima - 9791069



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


    int i;

    for (i = 0; i < n_regs; ++i) {
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

	//printf("counter: %d\nnext_offset: %d\n", counter, next_offset); 	


 	// puts("removido");

 	// Ajusta a estrutura do indice em RAM
 	fseek(g->file, 0, SEEK_SET); // volta para o ponteiro do arquivo para o lugar inicial. Não é estritamente necessário.
 	*ch = g->array[g->length - 1]; // ultimo elemento do array ocupa lugar do elemento removido
 	g->length--; // atualiza valor da sruct
 	qsort(g->array, g->length, sizeof(ChPrim), compare); // ordena o array

 	// puts("ajustado");
 	//fflush(g->file);
 	//freopen(NULL, "w+b", g->file);
 	return 0;
}
//calcula o tamanho em bytes de r
//65 == soma dos tamanhos dos campos constantes, documento, data_cadastro, data_atualiza
//4 * sizeof(int) == numero que indica o tamanho dos campos variaveis
//utiliza wcslen para pegar a length dos campos variaveis e multiplica pelo sizeof(wchar_t) para obter o numero de bytes
int sizeReg(Registro_Delimitador *r){
	return ((int) wcslen(r->dominio) + (int) wcslen(r->nome) + (int) wcslen(r->uf) + (int) wcslen(r->cidade)) * sizeof(wchar_t) + 4 * sizeof(int) + 65;
}

//Realiza a insercao de r no arquivo g->file utilizando o metodo first fit, e atualiza g->array(indices) e g->length
void firstFit(Group *g, Registro_Delimitador *r){
	RemList *rem = criar_lista(g->file, 0);
	Node *aux = rem->inicio, *aux_ant = rem->inicio;
	//aux eh utilizado para percorrer os nodes dos removidos
	//aux_ant eh o no anterior de aux
	
	int tam_r = sizeReg(r);//tamanho em bytes de r que serao necessarios para a insercao
	char breaker = '~';//usado para escrever no arquivo antes de escrever r, ao encontrar a posicao de insercao
	int fim;

	//buscando o node(o espaco removido logicamente) a ser inserido
	while(aux != NULL && aux->tamanho < tam_r + 13){
		aux_ant = aux;
		aux = aux->proximo;
	}

	if(aux == NULL){//se nao ha espacos colocar no fim do arquivo
		fseek(g->file, 0, SEEK_END);
		fim = ftell(g->file);
		WriteReg_Delimitador(r, g->file);
	}else{//senao colocar no primerio espaco possivel encontrado
		//posicionando para a insercao do registro
		fseek(g->file, aux->offset + (aux->tamanho - tam_r - 1), SEEK_SET);
		fwrite(&breaker, sizeof(char), 1, g->file);//escrevendo o delimitador anterior ao registro
		WriteReg_Delimitador(r, g->file);//escrevendo no registro
		
		if(aux->tamanho - tam_r - 1 < sizeof(int) + sizeof(long int)){
			//caso especifico em que nao ha mais espaco pra inserir nem os 13bytes necessarios para conter pra conter (* tamanho offset)
			aux_ant->proximo = aux->proximo;
			RemListToFile(g, rem);
		}else{//atualizando o novo tamanho do espaco restante
			aux_ant->proximo = aux->proximo;
			inserir_lista(rem, aux->offset, aux->tamanho - tam_r - 1, aux->prox);
			RemListToFile(g, rem);
		}
	}
	
	g->length++;
	g->array = (ChPrim *) realloc(g->array, g->length * sizeof(ChPrim));//atualizando o tamanho de array
	g->array[g->length - 1].ticket = r->ticket;//atualizado os parametros de ChPrim
	if(aux != NULL)g->array[g->length - 1].offset = aux->offset + aux->tamanho + 1;
	else g->array[g->length - 1].offset = fim;
	qsort(g->array, g->length, sizeof(ChPrim), compare); // ordena o array
}

//Realiza a insercao de r no arquivo g->file utilizando o metodo best fit, e atualiza g->array(indices) e g->length
void bestFit(Group *g, Registro_Delimitador *r){
	RemList *rem = criar_lista(g->file, 1);
	Node *aux = rem->inicio, *aux_ant = rem->inicio;
	//aux eh utilizado para percorrer os nodes dos removidos
	//passado eh o anterio de aux
	//atual eh o candidato atual a ser inserido
	//aux_ant eh o no anterior de atual
	
	int tam_r = sizeReg(r);//tamanho em bytes de r que serao necessarios para a insercao
	char breaker = '~';//usado para escrever no arquivo antes de escrever r, ao encontrar a posicao de insercao
	long int menor;//menor diferenca entre o espaco possivel e tam_r
	int fim;

	//buscando o node(o espaco removido logicamente) a ser inserido
	if(aux != NULL){
		if(aux->tamanho < tam_r) aux == NULL;

		while(aux->proximo != NULL && aux->proximo->tamanho < tam_r + 13){
			aux_ant = aux;
			aux = aux->proximo;
		}
	}

	if(aux == NULL){//se nao ha espacos colocar no fim do arquivo
		fseek(g->file, 0, SEEK_END);
		fim = ftell(g->file);
		WriteReg_Delimitador(r, g->file);
	}else{//senao colocar no primerio espaco possivel encontrado
		//posicionando para a insercao do registro
		fseek(g->file, aux->offset + (aux->tamanho - tam_r - 1), SEEK_SET);
		fwrite(&breaker, sizeof(char), 1, g->file);//escrevendo o delimitador anterior ao registro
		WriteReg_Delimitador(r, g->file);//escrevendo no registro
		
		if(aux->tamanho - tam_r - 1 < sizeof(int) + sizeof(long int)){
			//caso especifico em que nao ha mais espaco pra inserir nem os 13bytes necessarios para conter pra conter (* tamanho offset)
			aux_ant->proximo = aux->proximo;
			RemListToFile(g, rem);
		}else{
			//atualizando o novo tamanho do espaco restante
			aux_ant->proximo = aux->proximo;
			insercOrdCresc(rem, aux->offset, aux->tamanho - tam_r - 1, aux->prox);
			RemListToFile(g, rem);
		}
	}
	
	g->length++;
	g->array = (ChPrim *) realloc(g->array, g->length * sizeof(ChPrim));//atualizando o tamanho de array
	g->array[g->length - 1].ticket = r->ticket;//atualizado os parametros de ChPrim
	if(aux != NULL)g->array[g->length - 1].offset = aux->offset + aux->tamanho + 1;
	else g->array[g->length - 1].offset = fim;
	qsort(g->array, g->length, sizeof(ChPrim), compare); // ordena o array
}

void worstFit(Group *g, Registro_Delimitador *r){
    RemList *rem = criar_lista(g->file, 2);
    Node *aux = rem->inicio, *aux_ant = rem->inicio;
	//aux eh utilizado para percorrer os nodes dos removidos
	//passado eh o anterio de aux
	//atual eh o candidato atual a ser inserido
	//aux_ant eh o no anterior de atual
	
	int tam_r = sizeReg(r);//tamanho em bytes de r que serao necessarios para a insercao
	char breaker = '~';//usado para escrever no arquivo antes de escrever r, ao encontrar a posicao de insercao
	long int maior = 0;//maior diferenca entre o espaco possivel e tam_r
	int fim;

	if(aux->tamanho < tam_r) aux == NULL;

	if(aux == NULL){//se nao ha espacos colocar no fim do arquivo
		fseek(g->file, 0, SEEK_END);
		fim = ftell(g->file);
		WriteReg_Delimitador(r, g->file);
	}else{//senao colocar no primerio espaco possivel encontrado
		//posicionando para a insercao do registro
		fseek(g->file, aux->offset + (aux->tamanho - tam_r - 1), SEEK_SET);
		fwrite(&breaker, sizeof(char), 1, g->file);//escrevendo o delimitador anterior ao registro
		WriteReg_Delimitador(r, g->file);//escrevendo no registro
	
		if(aux->tamanho - tam_r - 1 < sizeof(int) + sizeof(long int)){
			aux_ant->proximo = aux->proximo;
			RemListToFile(g, rem);
		}else{
			//caso especifico em que nao ha mais espaco pra inserir nem os 13bytes necessarios para conter pra conter (* tamanho offset)
			//atualizando o novo tamanho do espaco restante
			rem->inicio = aux->proximo;
			insercOrdCresc(rem, aux->offset, aux->tamanho - tam_r - 1, aux->prox);
			RemListToFile(g, rem);
		}
	}
	
	g->length++;
	g->array = (ChPrim *) realloc(g->array, g->length * sizeof(ChPrim));//atualizando o tamanho de array
	g->array[g->length - 1].ticket = r->ticket;//atualizado os parametros de ChPrim
	if(aux != NULL)g->array[g->length - 1].offset = aux->offset + aux->tamanho + 1;
	else g->array[g->length - 1].offset = fim;
	qsort(g->array, g->length, sizeof(ChPrim), compare); // ordena o array
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




char *readLine(FILE *buffer){
	char *str = NULL;
	char aux = '\0';
	int i = 0;
	do{
		aux = fgetc(buffer);
		if(aux != '\n' && !feof(buffer)){
			str = (char *) realloc (str, (sizeof(char) * (i + 1)));
			str[i] = aux;
			i++;
		}
	}while(aux != '\n' && !feof(buffer));
	str = (char *) realloc (str, (sizeof(char) * (i + 1)));
	str[i] = '\0';
	return str; 

}

void showStatus(Group* g) {

	int i;
	int cont = 0;
	printf("(id)     ticket    offset\n");	
	for (i = 0; i < g->length; ++i)
	{	
		if(cont == 50){
			printf("pressione ENTER para continuar ou digite sair\n");
			char * s = readLine(stdin);
			cont = 0;
			if(strcmp(s, "sair") == 0){
				free(s);
				return;
			}
		}


		printf("(%3d) %10ld %10ld|\n", i, g->array[i].ticket, g->array[i].offset);
		cont++;
	}

	long int header = -99;

	fseek(g->file, 0, SEEK_SET); //vai para o inicio do cabecalho
	fread(&header, sizeof(long int), 1, g->file); // le informacao do cabecalho

	fseek(g->file, 0, SEEK_SET); //vai para o inicio do cabecalho

	printf("cabecalho:  %10ld\n",  header);
	printf("Numero de ocorrencias neste indice:  %5d\n",  g->length);

}
void showThreeStatuses(Group g[3]){
	
	int i;
	long int header = -99;
	char * aux = NULL;
	printf("(id)     ticket    offset  | (id)     ticket    offset  | (id)     ticket    offset  |\n\n");	
	for(i =0 ; i < g[0].length; i++){
		printf("(%-3d) %-10ld %-10ld| (%-3d) %-10ld %-10ld| (%-3d) %-10ld %-10ld|\n", i, g[0].array[i].ticket, g[0].array[i].offset, i, g[1].array[i].ticket, g[1].array[i].offset, i, g[2].array[i].ticket, g[2].array[i].offset);
		if(i % 50 == 0 && i != 0){
			printf("Pressione Enter para continuar ou digite sair\n");
			aux = readLine(stdin);
			if(strcmp(aux, "sair") == 0){
				free(aux);
				return;
			}
		}
	}

	printf("Numero total de ocorrencias\n");
	for(i = 0; i < 3; i++){
		fseek(g[i].file, 0, SEEK_SET); //vai para o inicio do cabecalho
		fread(&header, sizeof(long int), 1, g[i].file); // le informacao do cabecalho

		fseek(g[i].file, 0, SEEK_SET); //vai para o inicio do cabecalho

		//printf("cabecalho:  %10ld\n",  header);
		printf("Numero de ocorrencias no indice %d:  %5d\n",  (i + 1),g->length);
	}

	free(aux);
}

// int main() {

// 	setlocale(LC_ALL, "pt_BR.ISO-8859-1");

// 	Create3Files_Delimitador("dados_entrada.csv");
// 	puts("Criando Indice");
// 	createIndexes();

// 	Group g[3];
// 	g[0].file = fopen("arquivo_de_saida_1.bin", "r+b");
// 	readIndex(&g[0], "indice_1.bin");
// 	g[1].file = fopen("arquivo_de_saida_2.bin", "r+b");
// 	readIndex(&g[1], "indice_2.bin");
// 	g[2].file = fopen("arquivo_de_saida_3.bin", "r+b");
// 	readIndex(&g[2], "indice_3.bin");




// 	// puts("Indice criado. Imprimindo: ...");
// 	// printIndexFile("indice_1.bin");

// 	puts("status: ...");
// 	//removerReg(&g[0], 0);
// 	//showStatus(&g[0]);
// 	showThreeStatuses(g);
// 	// PrintAll_Delimitador2();
	
// 	puts("Fim");


// 	// criar 3 arquivos de indice
// 	// onde fica o começo da pilha para remoção?

// 	return 0;
// }
