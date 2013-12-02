// Proyecto 1 - Servidor
// Francisco Gamundi
// Franco Nori
#include "lista.h"

/**
 * Distribuye el mensaje a todos en la lista de clientes recibida.
 * 
 * @param mensaje   Mensaje a enviar.
 * @param clientes  Lista de clientes a enviar el mensaje.
 */
void distMensaje(char* mensaje, listaClientes clientes){
    listaClientes itera;
    itera = clientes;
    do
    {
        if (write(itera->sockfd, mensaje, strlen(mensaje)) == -1){
            perror("No se puede escribir en socket");
        }
        itera = itera->prox;
    } while (itera != NULL);
}

/**
 * Migra todos los clientes de una sala a otra.
 * 
 * @param sala        Sala que se va a cerrar.
 * @param salaInicio  Sala a donde van a migrar los usuarios.
 */
void migrarClientes(listaSalas sala, listaSalas salaInicio){
    listaClientes cliente = sala->clientes;
    while(cliente != NULL) {
        salaInicio->clientes = agregarCliente(salaInicio->clientes, cliente->sockfd, cliente->nombre);
        sala->clientes = eliminarCliente(sala->clientes, cliente->sockfd);
        cliente = cliente->prox;
    }
}

/**
 * Revisa constantemente lo que se esta escribiendo en el socket
 * 
 * @param pEntrada tipo de datos con los datos necesarios para la comunicación
 */
int escuchar(void *pEntrada) {
    Param *param;
    char *c, *token, *mensaje;
    int status;
    int i;
    param = (Param *) pEntrada;
    listaClientes lClientes;
    listaSalas sala;    
    c = (char *) malloc(sizeof(char)*MAX);

    if (c == NULL)
    {
        perror("No se pudo reservar memoria\n");
        exit(EXIT_FAILURE);
    }
    token = (char *) malloc(sizeof(char)*MAX);
    if (token == NULL)
    {
        perror("No se pudo reservar memoria\n");
        exit(EXIT_FAILURE);
    }
    mensaje = (char *) malloc(sizeof(char)*MAX);
    if (mensaje == NULL)
    {
        perror("No se pudo reservar memoria\n");
        exit(EXIT_FAILURE);
    }
    while (read(param->sockfd, c, MAX-1) != -1)
    {
        /* Corta el string en 2 partes y retorna la 1ra al token,
           mientras que la segunda se queda en el c */
        token = strsep(&c, " ");
        token = strtok(token, " ");
        sala = param->salaActual;
        lClientes = sala->clientes;
        
        // Revisa que encontro en la primera frase del comando
        if (strcmp(token,"men")==0){
            /* Si se recibió un mensaje, se guarda en un string junto con el
             * nombre de la persona que lo envio y se distribuye con la funcion
             * dist Mensaje.
             */
            strcpy(mensaje, param->nombre);
            strcat(mensaje, ": ");
            strcat(mensaje, c);
            distMensaje(mensaje, lClientes);
            memset(mensaje, 0, MAX);
            memset(c, 0, MAX);
        } else if (strcmp(token,"sus")==0){
            // Se elimina el cliente de la sala actual y se agrega a la lista
            // de clientes de la nueva sala
            sala->clientes = eliminarCliente(sala->clientes, param->sockfd);
            param->salaActual = buscarSala(param->lSalas, c);
            sala = param->salaActual;
            sala->clientes = agregarCliente(sala->clientes, param->sockfd, param->nombre);
            mensaje = strcat(mensaje, "\nAhora está en la sala ");
            mensaje = strcat(mensaje, c);
            mensaje = strcat(mensaje, "\n");
            if (write(param->sockfd, mensaje, strlen(mensaje)) == -1){
                perror("No se puede escribir en socket");
            }
            memset(mensaje, 0, MAX);
            memset(c, 0, MAX);
        } else if (strcmp(token,"des")==0){
            // Se elimina el cliente de la sala actual y se agrega a la lista
            // de clientes de la sala
            sala->clientes = eliminarCliente(sala->clientes, param->sockfd);
            param->salaActual = param->lSalas;
            sala = param->salaActual;
            sala->clientes = agregarCliente(sala->clientes, param->sockfd, param->nombre);
            mensaje = strcat(mensaje, "\nSe eliminó la sala y ahora está en la sala ");
            mensaje = strcat(mensaje, sala->nombreSala);
            mensaje = strcat(mensaje, "\n");
            if (write(param->sockfd, mensaje, strlen(mensaje)) == -1){
                perror("No se puede escribir en socket");
            }
            memset(mensaje, 0, MAX);
            memset(c, 0, MAX);
        } else if (strcmp(token,"cre")==0){
            /*El usuario crea la sala en el servidor*/
            sala = param->lSalas;
            sala = agregarSala(sala, c);
            printf("%s%s\n", "Se creó la sala", c);
            // iteraSala(sala);
            mensaje = strcat(mensaje, "\nSe creó la sala ");
            mensaje = strcat(mensaje, c);
            mensaje = strcat(mensaje, "\n");
            
            if (write(param->sockfd, mensaje, strlen(mensaje)) == -1){
                perror("No se puede escribir en socket");
            }
            memset(mensaje, 0, MAX);
            memset(c, 0, MAX);
        } else if (strcmp(token,"eli")==0){
            // Se migran los clientes de la sala a eliminar a la sala defecto y
            // se elimina la sala.
            sala = buscarSala(param->lSalas, c);
            migrarClientes(sala, param->lSalas);
            param->lSalas = eliminarSala(param->lSalas, c);
            mensaje = strcat(mensaje, "\nSe eliminó la sala ");
            mensaje = strcat(mensaje, c);
            mensaje = strcat(mensaje, "\n");
            if (write(param->sockfd, mensaje, strlen(mensaje)) == -1){
                perror("No se puede escribir en socket");
            }
            memset(mensaje, 0, MAX);
            memset(c, 0, MAX);
        } else if (strcmp(token,"fue")==0){
            // Se elimina el cliente de la sala, se cierra el socket y se
            // termina el hilo.
            sala->clientes = eliminarCliente(sala->clientes, param->sockfd);
            close(param->sockfd);
            pthread_exit(0);
        } else {
            mensaje = strcat(mensaje, "\nInstrucción no válida\n");
            if (write(param->sockfd, mensaje, strlen(mensaje)) == -1){
                perror("No se puede escribir en socket");
            }
            memset(mensaje, 0, MAX);
        }
    }
    sala->clientes = eliminarCliente(sala->clientes, param->sockfd);
    close(param->sockfd);
    pthread_exit(0);
}

/**
    Programa main
*/
int main(int argc, char *argv[]){

    int i = 1;
    char *port = NULL, *sala = NULL;
    Param *param;
    int sockfd;
    struct sockaddr_in clientaddr, serveraddr;
    int clientaddrlength;
    listaSalas salas;
  
    // Verifica que hayan los argumentos suficientes y necesarios
    if (argc < 3){
        printf("ERROR; No hay argumentos suficientes!\n");
        return(-1);
    } else if (argc > 5){
        printf("ERROR; Demasiados argumentos.\n");
        return(-1);
    }

    // Se leen los argumentos
    while (i < argc)
    {
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
    if (port == NULL)
    {
        printf("ERROR; No se especificó PUERTO (-p).\n");
        return(-1);
    }

    // Se crea la lista de salas y se agrega la primera
    salas = NULL;
    if (sala == NULL)
    {
        sala = "actual";  
    }
    salas = agregarSala(salas, sala);

    // Se abre el socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("No se pudo abrir el socket");
        exit(EXIT_FAILURE);

    }

    bzero(&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons(atoi(port));
    if (bind(sockfd, (struct sockaddr *) &serveraddr, sizeof(serveraddr)) != 0)
    {
        perror("No se puede afiliar la dirección al socket");
        exit(EXIT_FAILURE);

    }
    if (listen(sockfd, 5) < 0)
    {
        perror("No se pueden recibir conexiones");
        exit(EXIT_FAILURE);

    }

    while (1) {
        param = (Param *) malloc(sizeof(Param));
        param->nombre = (char *) malloc(sizeof(char)*25);

        // Se espera por una conexión
        clientaddrlength = sizeof(clientaddr);
        param->sockfd = accept(sockfd, (struct sockaddr *) &clientaddr, &clientaddrlength);
        if (param->sockfd < 0)
        {
            perror("Error al aceptar la conexión");
            exit(0);
        }

        // Se lee el nombre del cliente y se agrega a la lista de clientes de la
        // sala por defecto
        read(param->sockfd, param->nombre, 25);
        salas->clientes = agregarCliente(salas->clientes, param->sockfd, param->nombre);
        param->lSalas = salas;
        param->salaActual = salas;

        if (pthread_create(&(param->tId), NULL, (void *)escuchar, (void *)param) != 0)
        {
            perror("Error creando hilo");
        }
    }
}