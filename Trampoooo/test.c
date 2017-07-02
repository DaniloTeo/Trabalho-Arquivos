
#include "lista.h"

int main() {


	RemList *n = (RemList *) malloc(sizeof(RemList));
	long int tamanho = 42, offset = 42;
	long int prox = 42;
	n->inicio = NULL;
	n->fim = NULL;
	n->cont = 0;
	char c;

	insercOrdCresc(n, 1, 11, 66);
	print_lista(n);
	insercOrdCresc(n, 2, 9, 43);
	print_lista(n);
	insercOrdCresc(n, 3, 20, 87);
	print_lista(n);
	insercOrdCresc(n, 4, 2, 90);

	// insercOrdDec(n, 10, 11, 12);
	// print_lista(n);
	// insercOrdDec(n, 10, 9, 12);
	// print_lista(n);
	// insercOrdDec(n, 10, 20, 12);
	// print_lista(n);
	// insercOrdDec(n, 10, 2, 12);

	print_lista(n);

	return 0;
}