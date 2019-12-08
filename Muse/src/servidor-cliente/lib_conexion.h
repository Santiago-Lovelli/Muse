#ifndef LIB_CONEXION_H_
#define LIB_CONEXION_H_
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <sys/socket.h> //For Sockets
#include <stdlib.h>
#include <netinet/in.h> //For the AF_INET (Address Family)
#include <string.h>

#define MAXCONN 15

/*
Funciones para conectar MUSE con los procesos que usan
la libreria libmuse.h
*/

//Estructuras

typedef struct
{
  uint32_t resp;
}Paquete_respuesta;

typedef struct
{
  uint32_t op;
  uint32_t size_alloc;

}Paquete_muse_alloc;

typedef struct
{
  uint32_t op;
  uint32_t direccion;
}Paquete_muse_free;


typedef struct
{
  uint32_t op;
  uint32_t p_muse_read; //posicion de muse a leer
  uint32_t read_size; //deberia ser size_t ? Si es asi, hay que ver cuanto ocupa en 32bit para enviarlo
}Paquete_muse_get;


typedef struct
{
  uint32_t op;
  uint32_t muse_pos;
  void * data;
  size_t size_send;
}Paquete_muse_cpy;


typedef struct
{
  uint32_t op;

}Paquete_muse_map;


typedef struct
{
  uint32_t op;
}Paquete_muse_sync;


typedef struct
{
  uint32_t op;
}Paquete_muse_unmap;


typedef struct
{
  uint32_t op;
  uint32_t id;
}Paquete_muse_close;

typedef struct
{
  uint32_t size_resp;
  void * respuesta;
}Paquete_respuesta_general;

//Variables globales

struct sockaddr_in serv; //This is our main socket variable.
uint32_t fd; //This is the socket file descriptor that will be used to identify the socket
uint32_t conn; //This is the connection file descriptor that will be used to distinguish client connections.
char message[100] = ""; //This array will store the messages that are sent by the server


//Funciones


uint32_t conectarse_a_servidor(char *ip,uint32_t puerto);
uint32_t iniciar_servidor(uint32_t puerto);
uint32_t esperar_cliente(uint32_t cliente);
void recibir_paquete(uint32_t destinatario);

Paquete_respuesta_general * recibir_muse_alloc(uint32_t destinatario);
void enviar_muse_alloc(uint32_t destino,Paquete_muse_alloc *paquete);

uint32_t recibir_muse_free(uint32_t destinatario);
void enviar_muse_free(uint32_t destino,Paquete_muse_free *paquete);

Paquete_respuesta_general * recibir_muse_get(uint32_t destinatario);
void enviar_muse_get(uint32_t destino,Paquete_muse_get *paquete);

uint32_t recibir_muse_cpy(uint32_t destinatario);
void enviar_muse_cpy(uint32_t destino,Paquete_muse_cpy *paquete);

uint32_t recibir_muse_close(uint32_t destinatario);
void enviar_muse_close(uint32_t destino,Paquete_muse_close *paquete);

//---------------------------------------------

void enviar_respuesta_general(uint32_t destino,Paquete_respuesta_general *paquete);
Paquete_respuesta_general * recibir_respuesta_general(uint32_t destinatario);

uint32_t tratar_muse_alloc(uint32_t tam,uint32_t id_proceso);


//falta serializar mus_map ,muse_unmap y muse_sync



/*+++++Copy de comun+++++*/

int iniciar_servidor_copiado(char* ip, char* puerto){
	int socket_servidor;

	sleep(60);

    struct addrinfo hints, *servinfo, *p;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    getaddrinfo(NULL, puerto, &hints, &servinfo);
    for (p=servinfo; p != NULL; p = p->ai_next)
    {
        if ((socket_servidor = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
            continue;

        if (bind(socket_servidor, p->ai_addr, p->ai_addrlen) == -1) {
            close(socket_servidor);
            continue;
        }
        break;
    }

	listen(socket_servidor, SOMAXCONN);

    freeaddrinfo(servinfo);


    return socket_servidor;
}

int esperar_cliente_con_accept(int socket_servidor)
{
	struct sockaddr_in dir_cliente;
	int tam_direccion = sizeof(struct sockaddr_in);

	int socket_cliente = accept(socket_servidor, &dir_cliente, &tam_direccion);

	return socket_cliente;
}





#endif
