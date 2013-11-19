// Proyecto 1 - Servidor
// Francisco Gamundi
// Franco Nori
 
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main(int argc, char const *argv[]){

	int i = 1;
	char *port = NULL, *sala = NULL;
  
	// Verifica que hayan los argumentos suficientes y necesarios
	if (argc < 5){
		printf("ERROR; No hay argumentos suficientes!\n");
		return(-1);
	}else if(argc > 5){
		printf("ERROR; Demasiados argumentos.\n");
		return(-1);
	}

	// Se leen los argumentos
	while (i < argc){
		if (strcmp(argv[i], "-s") == 0){
			if (sala == NULL){
				sala = argv[i+1];
			} else {
				printf("ERROR; Argumento repetido.\n");
				return(-1);
			}
		} else if (strcmp(argv[i], "-p") == 0){
			if (port == NULL){
				port = argv[i+1];
			} else {
				printf("ERROR; Argumento repetido.\n");
				return(-1);
			}    
		} else {
			printf("ERROR; Argumento inválivo %s.\n", argv[i]);
			return(-1);
		}
		i = i + 2;
	}

	// Se verifica que se introdujeron los argumentos necesarios
	if (sala == NULL)
	{
		printf("ERROR; No se especificó SALA (-s).\n");
		return(-1);
	}
	if (port == NULL)
	{
		printf("ERROR; No se especificó PUERTO (-p).\n");
		return(-1);
	}
}

//Funcion de conexion al servidor
int conexion(int puerto, char* host){

}

//Funcion de manejo de comandos