#include <stdlib.h>
#include <stdio.h>
#include <string.h>
# include <wchar.h>
# include <locale.h>

#include "Registro_Delimitador.h"
#include "dac.h"


#define NOA 3 //numero de arquivos de saida gerados


void imprime_tabela_all(RemList *a, RemList *b, RemList *c){
	printf("Numero de registros removidos no arquivo_de_saida_1: %d\n", a->cont);
	printf("Numero de registros removidos no arquivo_de_saida_2: %d\n", b->cont);
	printf("Numero de registros removidos no arquivo_de_saida_3: %d\n", c->cont);
}


int main(void){
	int op = -1;
	int i, aux;
	long int t;
	RemList *l[3];

	//estabilizacao do padrao da linguagem usada para a manipulacao do arquivo
	setlocale(LC_ALL, "pt_BR.ISO-8859-1");


	printf("Digite o nome do arquivo CSV\n");
	char *filename = readLine(stdin);
	//criacao dos tres arquivos de saida + a criacao de seus indices
	Create3Files_Delimitador("dados_entrada.csv");
	puts("Criando Indice");
	createIndexes();

	//carregamento dos conteudos dos arquivos para memoria primaria
	Group g[3];
	g[0].file = fopen("arquivo_de_saida_1.bin", "r+b");
	readIndex(&g[0], "indice_1.bin");
	g[1].file = fopen("arquivo_de_saida_2.bin", "r+b");
	readIndex(&g[1], "indice_2.bin");
	g[2].file = fopen("arquivo_de_saida_3.bin", "r+b");
	readIndex(&g[2], "indice_3.bin");


	printf("Voce deseja:\n0- apagar um registro dos arquivos;\n1 - inserir um registro nos arquivos;\n2 - visualizar estatisticas dos arquivos indice;\n3 - visualizar estatisticas da lista de registros removidos;\n4 - sair.\n");
	scanf("%d", &op);getchar();

	
	while(op != 4){
		
		switch(op){
			case 0:
				
				printf("Digite o valor do ticket do registro que deseja remover\n");
				scanf("%ld", &t);
				

				//remocao do msm registro de todos os arquivos
				for(i = 0; i < NOA; i++){
					aux = removerReg(&g[i], t);
					if(aux == 0) {
						printf("Removido do arquivo %d com sucesso\n", i);
					}
					else{
						printf("Erro na remoção, fim da execução do programa\n");
						return 1;
					}
				}

				break;
			case 1:
				//insercao de registros
				break;
			case 2:
				showThreeStatuses(g);
				break;
			case 3:
				//estatisticas dos registros removidos
				printf("Digite o arquivo cuja lista de remocao voce deseja analisar(1, 2, ou 3)\n");
				scanf("%d", &aux);
				
				while(aux <= 0){
					printf("Por favor, digite um valor valido(1, 2, ou 3)\n");
					scanf("%d", &aux);
				}


				aux--;

				for(i = 0; i < 3; i++){
					l[i] = criar_lista(g[i].file);
				}

				if(l[0]->cont != 0 || l[1]->cont != 0 || l[2]->cont != 0){
					imprime_tabela_all(l[0], l[1], l[2]);
				}

				print_lista(l[aux]);


				break;
		}


		printf("Voce deseja:\n0 - apagar um registro dos arquivos;\n1 - inserir um registro nos arquivos;\n2 - visualizar estatisticas dos arquivos indice;\n3 - visualizar estatisticas da lista de registros removidos;\n4 - sair.\n");
		scanf("%d", &op);getchar();

	}

	//liberacao de memoria

	for(i = 0; i < 3; i++){
		apagar_lista(l[i]);
		fclose(g[i].file);

	}
	


	return 0;
}


