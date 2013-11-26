// Proyecto 1 - Cliente
// Francisco Gamundi
// Franco Nori
 
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <strings.h>
#include <ctype.h>

void copy(int sockfd) {
  char* outbuffer;
  char* inbuffer;
  int max = 255;

    while (1) {
        /* Write a character to the socket. */
        outbuffer = (char *) malloc(sizeof(char)*max);
        inbuffer = (char *) malloc(sizeof(char)*max);
        getline(&outbuffer, &max, stdin);
        if (write(sockfd, outbuffer, strlen(outbuffer)-1) == -1)
        {
            perror("can't write to socket");
        }
        /* Read the response and print it. */
        if (read(sockfd, inbuffer, max) == -1)
            perror("can't read from socket");
        printf("%s\n", inbuffer);
    }
}

int main(int argc, char const *argv[]){

    int i = 1;
    char *host = NULL, *port = NULL, *nombre = NULL, *archivo = NULL;
    int sockfd;
    struct sockaddr_in serveraddr;
  
    // Verifica que hayan los argumentos suficientes y necesarios
    if (argc < 7){
        printf("ERROR; No hay argumentos suficientes!\n");
        exit(EXIT_FAILURE);
    }else if(argc > 9){
        printf("ERROR; Demasiados argumentos.\n");
        exit(EXIT_FAILURE);
    }

    // Se leen los argumentos
    while(i < argc){
        if(strcmp(argv[i], "-h") == 0){
            if (host == NULL){
                host = argv[i+1];
            } else {
                printf("ERROR; Argumento repetido.\n");
                exit(EXIT_FAILURE);
            }
        } else if (strcmp(argv[i], "-p") == 0){
            if(port == NULL){
                port = argv[i+1];
            } else {
                printf("ERROR; Argumento repetido.\n");
                exit(EXIT_FAILURE);
            }    
        } else if (strcmp(argv[i], "-n") == 0){
            if (nombre == NULL){
                nombre = argv[i+1];
            } else {
                printf("ERROR; Argumento repetido.\n");
                exit(EXIT_FAILURE);
            }
        } else if (strcmp(argv[i], "-a") == 0){
            if (archivo == NULL){
                archivo = argv[i+1];
            } else {
                printf("ERROR; Argumento repetido.\n");
                exit(EXIT_FAILURE);
            }
        } else {
            printf("ERROR; Argumento inválivo %s.\n", argv[i]);
            exit(EXIT_FAILURE);
        }
        i = i + 2;
    }

    // Se verifica que se introdujeron los argumentos necesarios
    if (host == NULL)
    {
        printf("ERROR; No se especificó HOST (-h).\n");
        exit(EXIT_FAILURE);
    }
    if (port == NULL)
    {
        printf("ERROR; No se especificó PUERTO (-p).\n");
        exit(EXIT_FAILURE);
    }
    if (nombre == NULL)
    {
        printf("ERROR; No se especificó NOMBRE (-n).\n");
        exit(EXIT_FAILURE);
    }

    // Se establece la dirección y puerto a conectar
    bzero(&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = inet_addr(host);
    serveraddr.sin_port = htons(atoi(port));

    // Se abre el socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("No se pudo abrir el socket");
        exit(EXIT_FAILURE);
    }

    // Intento de conexión al servidor
    if (connect(sockfd, (struct sockaddr *) &serveraddr, sizeof(serveraddr)) < 0)
    {
        perror("Error al conectar al servidor");
        exit(EXIT_FAILURE);
    }

    /* Copy input to the server. */
    copy(sockfd);
    close(sockfd);
    printf("%s\n", "No hice una verga");
    exit(EXIT_SUCCESS);
}
//Funcion de conexion al servidor
int conexion(int puerto, char* host){

}

//Funcion de manejo de comandos