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
    int *sockfd, size;
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
        size = read(*sockfd, inbuffer, MAX-1);
        if (size == -1)
            perror("No se puede leer del socket");
        if (size > 1)
            if (!strcmp(inbuffer, "fue"))
            {
                close(*sockfd);
                size = -1;
            } else {
                printf("%s\n", inbuffer);
            }
    } while(size >= 0);
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
    int size;
    char *token;
    token = (char *) malloc(sizeof(char)*MAX);
    if (token == NULL)
    {
        perror("No se pudo reservar memoria\n");
        exit(EXIT_FAILURE);
    }

    size = strlen(cmd);
    if (size < 4)
    {
        printf("%s\n", "Comando no válido");
    } else {
        token = strncpy(token, cmd, 3);
        token[3] = '\0';
        if (size == 4)
        {
            if (!strcmp(token, "sal") || !strcmp(token, "des") || !strcmp(token, "usu") || !strcmp(token, "fue"))
            {
                if (write(*sockfd, cmd, strlen(cmd)-1) == -1) {
                    perror("No se puede escribir al socket");
                }
                if (!strcmp(token, "fue"))
                {
                    printf("Terminó la ejecucíón del programa\n");
                    pthread_exit(NULL);
                }
            } else {
                printf("%s\n", "Comando no válido");
            }
        } else if (size > 5) {
            if (!strcmp(token, "men") || !strcmp(token, "sus") || !strcmp(token, "cre") || !strcmp(token, "eli"))
            {
                if (write(*sockfd, cmd, strlen(cmd)-1) == -1)
                {
                    perror("No se puede escribir al socket");
                }
            } else {
                printf("%s\n", "Comando no válido");
            }
        } else {
            printf("%s\n", "Comando no válido");
        }
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
        usleep(100000);
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
    int max, size;

    max = 255;
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

    // See lee la entrada estandar y se envía cada linea leida a la función
    // enviar.
    do
    {
        getline(&cmd, (size_t *)&max, stdin);
        enviar(cmd, param->socket);
    } while (1);
    free(cmd);
    pthread_exit(NULL);
}
/*
    Programa main
*/
int main(int argc, char *argv[]){

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

    // Se envía el nombre del cliente
    if (write(sockfd, nombre, strlen(nombre)) == -1)
    {
        perror("No se puede escribir al socket");
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
    if (pthread_create(&hiloE, NULL, (void *)leerComando, (void *)param) != 0)
    {
        perror("Error creando hilo");
    }

    if (pthread_create(&hiloR, NULL, (void *)recibir, (void *) socketP) != 0)
    {
        perror("Error creando hilo");
    }
    pthread_join(hiloR, NULL);
    pthread_join(hiloE, NULL);
    free(param);
    exit(EXIT_SUCCESS);
}
