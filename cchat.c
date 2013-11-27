// Proyecto 1 - Cliente
// Francisco Gamundi
// Franco Nori
 
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <ctype.h>
#include <pthread.h>
#define MAX 255

typedef struct
{
    int *socket;
    char *archivo;
} Param;

/**
 * Lee permanentemente el socket esperando cualquier mensaje del servidor.
 *
 * @param  socket  Apuntador al socket abierto al servidor
 */
void recibir(void *socket) {
    int *sockfd;
    char *inbuffer;

    sockfd = (int *) socket;
    inbuffer = (char *) malloc(sizeof(char)*MAX);
    if (inbuffer == NULL)
    {
        perror("No se pudo reservar memoria\n");
        exit(EXIT_FAILURE);
    }
    do {
        // Se limpia el string de lectura y se lee el socket.
        memset(inbuffer, 0, 255);
        if (read(*sockfd, inbuffer, MAX-1) == -1)
            perror("No se puede leer del socket");
        printf("%s\n", inbuffer);
    } while(1);
    free(inbuffer);
    pthread_exit(NULL);
}

/**
 * Envia por el socket el comando recibido.
 *
 * @param  cmd     Comando a envar al servidor.
 * @param  sockfd  File descriptor del socket abierto al servidor.
 */
void enviar(char *cmd, int *sockfd) {
    if (write(*sockfd, cmd, strlen(cmd)-1) == -1)
    {
        perror("No se puede escribir al socket");
    }
}


/**
 * Lee el archivo que se recibió por argumento y se pasa cada comando a la 
 * funcion "enviar" para ser enviada al servidor.
 *
 * @param  narchivo  Nombre del archivo a abrir.
 * @param  sockfd    Apuntador al File Descriptor del socket.
 */
void leerArchivo(char *narchivo, int *sockfd){
    FILE *fd;
    int i;
    char *line;
    line = (char *) malloc(sizeof(char)*MAX);
    if (line == NULL)
    {
        perror("No se pudo reservar memoria\n");
        exit(EXIT_FAILURE);
    }

    // Abre el archivo.
    fd = fopen(narchivo, "r");
    // Extrae cada linea y se llama a la función enviar pasando el comando leido.
    while(fgets(line, MAX, fd) != NULL)
   {
        enviar(line, sockfd);
        memset(line, 0, MAX);
   }
   free(line);
   fclose(fd); 
}

/**
 * Revisa si se recibió un archivo para procesar los comando, y luego lee por
 * entrada estandar comando para pasarlos al servidor a través de la función
 * "enviar".
 *
 * @param  pEntrada  Tipo de dato que contiene un apuntador al File Descriptor
 *                   del socket abierto al servidor y el nombre del archivo.
 */
void leerComando(void * pEntrada){
    Param *param;
    char *cmd;
    int max = 255;

    param = (Param *) pEntrada;
    // Si se recibió un archivo, se llama a la función correspondiente.
    if (param->archivo != NULL)
    {
        leerArchivo(param->archivo, param->socket);
    }

    cmd = (char *) malloc(sizeof(char)*MAX);
    if (cmd == NULL)
    {
        perror("No se pudo reservar memoria\n");
        exit(EXIT_FAILURE);
    }
    do
    {
        getline(&cmd, &max, stdin);
        enviar(cmd, param->socket);
    } while (1);
    free(cmd);
    pthread_exit(NULL);
}
/*
    Programa main
*/
int main(int argc, char const *argv[]){

    int i = 1;
    char *host = NULL, *port = NULL, *nombre = NULL, *archivo = NULL;
    int sockfd, *socketP;
    struct sockaddr_in serveraddr;
    pthread_t hiloR, hiloE;
    Param *param;
  
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

    // Se crea tipo de datos a pasar al hilo de lectura de comandos.
    param = (Param *) malloc(sizeof(Param));
    if (param == NULL)
    {
        perror("No se pudo reservar memoria\n");
        exit(EXIT_FAILURE);
    }
    param->archivo = archivo;
    param->socket = &sockfd;
    socketP = &sockfd;

    // Creación de los hilos, uno para lectura de comandos por consola y archivo
    // y otro para lectura de mensajes del servidor.
    if (pthread_create(&hiloE, NULL, leerComando, (void *) param) != 0)
    {
        perror("Error creando hilo");
    }

    if (pthread_create(&hiloR, NULL, recibir, (void *) socketP) != 0)
    {
        perror("Error creando hilo");
    }

    pthread_join(hiloR, NULL);
    pthread_join(hiloE, NULL);
    close(sockfd);
    free(param);
    exit(EXIT_SUCCESS);
}
