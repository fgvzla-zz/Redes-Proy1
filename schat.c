// Proyecto 1 - Servidor
// Francisco Gamundi
// Franco Nori

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <strings.h>
#include <ctype.h>
#define MAX 255

/*
    Blah
*/
void echo(int sockfd) {
    char *c;
    int status;
    int i;
    c = (char *) malloc(sizeof(char)*MAX);
    while (read(sockfd, c, MAX-1) != -1)
    {
        /* Echo the character. */
        if (write(sockfd, c, strlen(c)) == -1)
        {
            perror("can't write to socket");
        }
        memset(c, 0, MAX);
    }
}

/*
    Programa main
*/
int main(int argc, char const *argv[]){

    int i = 1;
    char *port = NULL, *sala = NULL;

    int sockfd, newsockfd;
    struct sockaddr_in clientaddr, serveraddr;
    int clientaddrlength;
  
    // Verifica que hayan los argumentos suficientes y necesarios
    if (argc < 5){
        printf("ERROR; No hay argumentos suficientes!\n");
        return(-1);
    } else if (argc > 5){
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

    // Se abre el socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        perror("No se pudo abrir el socket");

    bzero(&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons(atoi(port));
    if (bind(sockfd, (struct sockaddr *) &serveraddr, sizeof(serveraddr)) != 0)
    {
        perror("No se puede afiliar la dirección al socket");
    }
    if (listen(sockfd, 5) < 0)
    {
        perror("No se pueden recibir conexiones");
    }

    while (1) {
        // Se espera por una conexión
        clientaddrlength = sizeof(clientaddr);
        newsockfd = accept(sockfd, (struct sockaddr *) &clientaddr, &clientaddrlength);
        if (newsockfd < 0)
        {
            perror("Error al aceptar la conexión");
        }
        close(sockfd);
        echo(newsockfd);
        echo(0);
        printf("Escribio\n");
        exit(0);
      }

}

// Funcion de conexion al servidor
int conexion(int puerto, char* host){

}

// Funcion de manejo de comandos