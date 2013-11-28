all: 	schat cchat

schat: lista.o schat.o
		gcc schat.o lista.o -o schat -l pthread

cchat: cchat.o
		gcc cchat.o -o cchat -l pthread

cchat.o: cchat.c
		gcc -c cchat.c

schat.o: schat.c lista.h
		gcc -c schat.c

lista.o: lista.c lista.h
		gcc -c lista.c

clean:
		rm -rf *.o cchat schat