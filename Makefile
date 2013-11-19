all: 
		gcc schat.c -o schat
		gcc cchat.c -o cchat

		
clean:
		rm -rf *.o cchat schat