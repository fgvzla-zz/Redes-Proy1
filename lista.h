// Proyecto 1 - Listas
// Francisco Gamundi
// Franco Nori

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#define MAX 255

struct listaC{
    int sockfd;
    char *nombre;
    struct listaC *prox;
};
typedef struct listaC *listaClientes;

struct listaS{
    char *nombreSala;
	listaClientes clientes;    
    struct listaS *prox;
};
typedef struct listaS *listaSalas;


extern listaClientes agregarCliente(listaClientes, int, char *);
extern listaClientes eliminarCliente(listaClientes, int);
extern void iteraClientes(listaClientes);
extern void limpiarClientes(listaClientes);

extern listaSalas agregarSala(listaSalas, char *);
extern listaSalas eliminarSala(listaSalas, char *);
extern int buscarSala(listaSalas, char *);
extern void iteraSalas(listaSalas);
extern void limpiarSalas(listaSalas);