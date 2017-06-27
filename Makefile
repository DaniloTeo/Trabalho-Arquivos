all:  Registro_Delimitador.o dac.o main.o
	gcc -g  Registro_Delimitador.o  dac.o main.o -o t -Wall -lm

Registro_Delimitador.o:
	gcc -g -c Registro_Delimitador.c


dac.o:
	gcc -g -c dac.c

main.o:
	gcc -g -c main.c

clean:
	rm -rf  *.o *.bin t

run:
	./t
