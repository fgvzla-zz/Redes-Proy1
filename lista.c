// Proyecto 1 - Listas
// Francisco Gamundi
// Franco Nori

#include "lista.h"
 
listaClientes agregarCliente(listaClientes l, int sockfd, char *nombre){
    listaClientes nueva_lista, itera;
    itera = l;
    nueva_lista = (listaClientes) malloc(sizeof(listaClientes));
    if (nueva_lista == NULL) {
        printf("\nActualmente no hay suficiente memoria para esta operacion\n ");
    } else {
        nueva_lista->nombre = (char *) malloc(sizeof(char)*25);
        if (nueva_lista->nombre == NULL) {
            printf("\nActualmente no hay suficiente memoria para esta operacion\n ");
        }
        strcpy(nueva_lista->nombre, nombre);
        nueva_lista->sockfd = sockfd;
        nueva_lista->prox = NULL;
        if (l == NULL)
        {
            return nueva_lista;
        } else {
            while (itera->prox != NULL)
            {
                itera = itera->prox;
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
    // Caso que recibe lista vacia y caso de que el que hay eliminar es el 
    // primero.
    if (anterior == NULL)
    {
        return(NULL);
    } else {
        if (anterior->sockfd == sockfd)
        {
            itera = anterior -> prox;
            free(anterior->nombre);
            free(anterior);
            return(itera);
        }
    }

    // Se busca cada cliente en la lista hasta encontrar la correcta y se procede
    // a eliminarla.
    while(itera->prox != NULL)
    {
        anterior = itera;
        itera = itera -> prox;
        if (itera->sockfd == sockfd)
        {
            anterior->prox = itera->prox;
            free(itera->nombre);
            free(itera);
            return(l);
        }
    }
    return l;
}

listaSalas agregarSala(listaSalas l, char *nombre){
    listaSalas nueva_lista, itera;
    itera = l;
    nueva_lista = (listaSalas) malloc(sizeof(listaSalas));
    if (nueva_lista == NULL) {
        printf("\nActualmente no hay suficiente memoria para esta operacion\n ");
    } else {
        nueva_lista->nombreSala = (char *) malloc(sizeof(char)*25);
        if (nueva_lista->nombreSala == NULL) {
            printf("\nActualmente no hay suficiente memoria para esta operacion\n ");
        }
        strcpy(nueva_lista->nombreSala, nombre);
        nueva_lista->clientes = NULL;
        nueva_lista->prox = NULL;
        if (l == NULL)
        {
            return nueva_lista;
        } else {
            while (itera->prox != NULL)
            {
                itera = itera->prox;
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
    if (anterior == NULL)
    {
        return(NULL);
    } else {
        if (strcmp(anterior->nombreSala, nombreSala))
        {
            itera = anterior -> prox;
            free(anterior->nombreSala);
            free(anterior);
            return(itera);
        }
    }

    // Se busca cada sala en la lista hasta encontrar la correcta y se procede
    // a eliminarla.
    while(itera->prox != NULL)
    {
        anterior = itera;
        itera = itera -> prox;
        if (strcmp(itera->nombreSala, nombreSala))
        {
            anterior->prox = itera->prox;
            free(itera->nombreSala);
            free(itera);
            return(l);
        }
    }
    return l;
}