#include <stdlib.h>
#include <stdio.h>

#include "lista.h"
#include "Registro_Delimitador.h"
#include "dac.h"






RemList *criar_lista(FILE *f, int t){
	RemList *n = (RemList *) malloc(sizeof(RemList));
	long int tamanho = 42, offset = 42;
	long int prox = 42;
	n->inicio = NULL;
	n->fim = NULL;
	n->cont = 0;
	char c;


	fseek(f, 0, SEEK_SET);
	fread(&prox, sizeof(long int), 1, f);
	// if(prox == -1){
	// 	printf("Nenhum registro removido deste arquivo ainda.\n");
	// }
	if(prox != -1){
		while(prox != -1 && (feof(f) == 0)){	
			
			fseek(f, prox, SEEK_SET);	
			offset = prox; //guardo o offset atual
			fread(&c, sizeof(char), 1, f);
			//printf("%c\n\n", c);
			fread(&tamanho, sizeof(int), 1, f); //guardo o tamanho do registro que havia aqui
			fread(&prox, sizeof(long int), 1, f); //armazeno o valor do proximo offset sem registro
			//printf("offset: %-ld\ttamanho: %-ld\tprox: %-ld\n",offset, tamanho, prox);
			inserir_lista(n, offset, tamanho, prox);
			n->cont++;


		}

	}

	if(t != 0) ordenar_lista(n, t);




	return n;
}

void apagar_lista(RemList *l){
	Node *rem = l->inicio, *aux = NULL;

	while(!lista_vazia(l)){
		rem = l->inicio;
		l->inicio = l->inicio->proximo;
		free(rem);
	}

	free(l);

}

int lista_vazia(RemList *l){
	return (l->inicio == NULL);
}

void inserir_lista(RemList *l, long int offset, long int tam, long int prox){
	Node *n = (Node *) malloc(sizeof(Node));
	n->offset = offset;
	n->tamanho = tam;
	n->prox = prox;
	n->proximo = NULL;

	if(lista_vazia(l)){
		l->inicio = n;
		l->fim = n;
	}
	else{
		l->fim->proximo = n;
		l->fim = n;
	}
}

void print_lista(RemList* l){
	Node *aux = l->inicio;
	int cont = 0;

	if(l->cont == 0){
		printf("Nenhum registro removido deste arquivo ainda.\n");
		return;
	}

	while(aux != NULL){
		printf(" %-ld| %-ld| %-ld| -->", aux->offset, aux->tamanho, aux->prox);
		aux = aux->proximo;

		if(cont % 5 == 0 && cont != 0){
			printf("\n");
		}
		// if(aux->prox == -1) printf(" %-ld| %-ld| %-ld| ", aux->offset, aux->tamanho, aux->prox);;
		// cont++;
	}

	printf("\n");
}


void ordenar_lista(RemList *l, int t){
	Node *a = l->inicio;
	Node *b = a->proximo;
	long int aux = -1;

	if(lista_vazia(l)) return;


	//ordenacao crescente
	if(t == 1){

		while(a != NULL){
			b = a->proximo;
			while(b != NULL){
					if(a->tamanho > b->tamanho){
						//ordenacao dos tamanhos
						aux = a->tamanho;
						a->tamanho = b->tamanho;
						b->tamanho = aux;

						//reorganizacao dos campos restantes
						//offset
						aux = a->offset;
						a->offset = b->offset;
						b->offset = aux;

						//offset do proximo
						aux = a->prox;
						a->prox = b->prox;
						b->prox = aux;


					}

				b = b->proximo;
			}
			a = a->proximo;
		}
	}

	//ordenacao decrescente
	if(t == 2){
		while(a != NULL){
			b = a->proximo;
			while(b != NULL){
					if(a->tamanho < b->tamanho){
						//ordenacao dos tamanhos
						aux = a->tamanho;
						a->tamanho = b->tamanho;
						b->tamanho = aux;

						//reorganizacao dos campos restantes
						//offset
						aux = a->offset;
						a->offset = b->offset;
						b->offset = aux;

						//offset do proximo
						aux = a->prox;
						a->prox = b->prox;
						b->prox = aux;


					}

				b = b->proximo;
			}
			a = a->proximo;
		}
	}




}





