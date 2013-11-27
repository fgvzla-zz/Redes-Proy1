all: 	schat.o cchat.o

schat.o: 
		gcc schat.c -o schat -l pthread
cchat.o: 
		gcc cchat.c -o cchat -l pthread
clean:
		rm -rf *.o cchat schat