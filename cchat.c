// Proyecto 1 - Cliente
// Francisco Gamundi
// Franco Nori
 
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main(int argc, char const *argv[]){

	int i = 1;
	char *host, *port, *nombre, *archivo;
  
	//Verifica que hayan los argumentos suficientes y necesarios
	if (argc < 6){
		printf("ERROR; No hay argumentos suficientes!\n");
		return(-1);
	}else if(argc > 8){
		printf("ERROR; Demasiados argumentos.\n");
		return(-1);
	}

	while(i < argc){
	/*Verifica el argumento -h*/
		if(strcmp(argv[i], "-h") == 0){
			if(host==NULL){
				host=argv[i+1];
			}else{
				printf("ERROR; Argumento repetido.\n");
				return(-1);
			}
		  /*Verifica el argumento -p*/
		}else if(strcmp(argv[i], "-p") == 0){
			if(port==NULL){
				port=argv[i+1];
			}else{
				printf("ERROR; Argumento repetido.\n");
				return(-1);
			}    
		}else if(strcmp(argv[i], "-n") == 0){
			if(nombre==NULL){
				nombre=argv[i+1];
			}else{
				printf("ERROR; Argumento repetido.\n");
				return(-1);
			}
		}else if(strcmp(argv[i], "-a") == 0){
			if(archivo==NULL){
				archivo=argv[i+1];
			}else{
				printf("ERROR; Argumento repetido.\n");
				return(-1);
			}
		}else{
			printf("ERROR; Argumento inválivo.\n");
			return(-1);
		}
		i=i+2;
	}
}

//Funcion de conexion al servidor
int conexion(int puerto, char* host){

}

//Funcion de manejo de comandos