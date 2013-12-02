// Proyecto 1 - Listas
// Francisco Gamundi
// Franco Nori

#include "lista.h"
 
listaClientes agregarCliente(listaClientes l, int sockfd, char *nombre){
    listaClientes nueva_lista, itera;
    itera = l;
    nueva_lista = (listaClientes) malloc(sizeof(listaClientes));
    //No crea la lista
    if (nueva_lista == NULL) {
        printf("\nActualmente no hay suficiente memoria para esta operacion\n ");
    } else {
        nueva_lista->nombre = (char *) malloc(sizeof(char)*25);
        //No crea el nombre de la sala en la lista
        if (nueva_lista->nombre == NULL){
            printf("\nActualmente no hay suficiente memoria para esta operacion\n ");
        }
        strcpy(nueva_lista->nombre, nombre);
        nueva_lista->sockfd = sockfd;
        nueva_lista->prox = NULL;
        //Caso lista vacia inicialmente
        if (l == NULL)
        {
            return nueva_lista;
        //Caso lista de salas no vacia
        } else {
            while (itera->prox != NULL)
            {
                if(strcmp(itera->nombre, nombre)==0){
                    perror("ERROR; Ya existe un usuario con este nombre!\n");
                    return l;
                }else{
                    itera = itera->prox;
                }
            }
            if(strcmp(itera->nombre, nombre)==0){
                perror("ERROR; Ya existe un usuario con este nombre!\n");
                return l;
            }
            itera->prox = nueva_lista;
        }
    }
    return l;
}
 
listaClientes eliminarCliente(listaClientes l, int sockfd){
    listaClientes itera, anterior;
    anterior = l;
    itera = l;
    /* Caso que recibe lista vacia y caso de que el que hay eliminar es el 
    primero.*/
    if (itera == NULL)
    {
        return(NULL);
    } else {
        /*Caso en que el cliente eliminar esta de primero en la lista*/
        if(itera->sockfd == sockfd)
        {
            itera = anterior->prox;
            free(anterior->nombre);
            free(anterior);
            return(itera);
        }else{
            /* Se busca cada cliente en la lista hasta encontrar el correcto y se procede
            a eliminarlo. Cuando llega al ultimo elemento de la lista, se sale.*/
            while(itera->prox != NULL)
            {
                if (itera->sockfd == sockfd)
                {
                    anterior->prox = itera->prox;
                    free(itera->nombre);
                    free(itera);
                    return(l);
                }
                anterior = itera;
                itera = itera->prox;
            }
            /*Caso en que el cliente esta de ultimo*/
            if (itera->sockfd == sockfd)
                {
                    anterior->prox = itera->prox;
                    free(itera->nombre);
                    free(itera);
                    return(l);
            }else{
                perror("ERROR; Dicho usuario no existe. No se pudo eliminar\n");
            }
        }
    }
    return l;
}

listaSalas agregarSala(listaSalas l, char *nombre){
    listaSalas nueva_lista, itera;
    itera = l;
    nueva_lista = (listaSalas) malloc(sizeof(listaSalas));
    //No crea la lista
    if (nueva_lista == NULL) {
        printf("\nActualmente no hay suficiente memoria para esta operacion\n ");
    } else {
        nueva_lista->nombreSala = (char *) malloc(sizeof(char)*25);
        //No crea el nombre de la sala en la lista
        if (nueva_lista->nombreSala == NULL){
            printf("\nActualmente no hay suficiente memoria para esta operacion\n ");
        }
        strcpy(nueva_lista->nombreSala, nombre);
        nueva_lista->clientes = NULL;
        nueva_lista->prox = NULL;
        //Caso lista vacia inicialmente
        if (l == NULL)
        {
            return nueva_lista;
        //Caso lista de salas no vacia
        } else {
            while (itera->prox != NULL)
            {
                if(strcmp(itera->nombreSala, nombre)==0){
                    perror("ERROR; La sala ya existe con este nombre!\n");
                    return l;
                }else{
                    itera = itera->prox;
                }
            }
            if(strcmp(itera->nombreSala, nombre)==0){
                perror("ERROR; La sala ya existe con este nombre!\n");
                return l;
            }
            itera->prox = nueva_lista;
        }
    }
    return l;
}
 
listaSalas eliminarSala(listaSalas l, char* nombreSala){
    listaSalas itera, anterior;
    anterior = l;
    itera = l;
    // Caso que recibe lista vacia y caso de que el que hay eliminar es el 
    // primero.
    if (itera == NULL)
    {
        return(NULL);
    } else {
        //Caso en que la sala a eliminar esta de primera en la lista
        if(strcmp(itera->nombreSala, nombreSala)==0)
        {
            itera = anterior->prox;
            free(anterior->nombreSala);
            free(anterior);
            return(itera);
        }else{
            // Se busca cada sala en la lista hasta encontrar la correcta y se procede
            // a eliminarla. Cuando llega al ultimo elemento de la lista, se sale.
            while(itera->prox != NULL)
            {
                if (strcmp(itera->nombreSala, nombreSala)==0)
                {
                    anterior->prox = itera->prox;
                    free(itera->nombreSala);
                    free(itera);
                    return(l);
                }
                anterior = itera;
                itera = itera -> prox;
            }
            //Caso en que la sala esta de ultima
            if (strcmp(itera->nombreSala, nombreSala)==0)
                {
                    anterior->prox = itera->prox;
                    free(itera->nombreSala);
                    free(itera);
                    return(l);
            }else{
                perror("ERROR; Dicha sala no existe. No se pudo eliminar\n");
            }
        }
    }
    return l;
}

void iteraSala(listaSalas salas){
    listaSalas itera;
    do
    {
        printf("%s\n", itera->nombreSala);
        itera = itera->prox;
    } while (itera != NULL);
}

/*
 * Encuentra la sala
 */
listaSalas buscarSala(listaSalas l, char *nombreSala){
    listaSalas itera;
    itera = l;
    while(itera != NULL){
        if (strcmp(itera->nombreSala, nombreSala)==0){
            return itera;
        }
        itera = itera->prox;
    }
    return NULL;
}