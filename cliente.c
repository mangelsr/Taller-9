#include <sys/types.h>          /* some systems still require this */
#include <sys/stat.h>
#include <stdio.h>              /* for convenience */
#include <stdlib.h>             /* for convenience */
#include <stddef.h>             /* for offsetof */
#include <string.h>             /* for convenience */
#include <unistd.h>             /* for convenience */
#include <signal.h>             /* for SIG_ERR */ 
#include <netdb.h> 
#include <errno.h> 
#include <syslog.h> 
#include <sys/socket.h> 
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/resource.h>
#include <fcntl.h>

#define BUFLEN 10485760 //10 Mb

int main(int argc, char ** argv)
{
  int cliente;

  if(argc != 5)
  {
  	printf("Error!!\n");
    printf("Modo de Uso: ./cliente <ip> <puerto> <ruta del archivo> <nombre del archivo>\n");
    return -1;
  }

  char *ip = argv[1];
  int puerto = atoi(argv[2]);
  char *ruta = argv[3];
  char *nombre = argv[4];
  char solicitud[] ="get ";
  strcat(solicitud, ruta);
  char name[] ="./";	
  strcat(name, nombre);
  printf("tomando parametros...\n");

  //Direccion del cliente
  struct sockaddr_in direccion_cliente;
  //ponemos en 0 la estructura direccion_cliente
  memset(&direccion_cliente, 0, sizeof(direccion_cliente));
  

  //llenamos los campos
  //IPv4
  direccion_cliente.sin_family = AF_INET;		
  //Convertimos el numero de puerto al endianness de la red
  direccion_cliente.sin_port = htons(puerto);	
  //Nos tratamos de conectar a esta direccion
  direccion_cliente.sin_addr.s_addr = inet_addr(ip);

  //Abrimos un socket
  cliente = socket(((struct sockaddr *)&direccion_cliente)->sa_family, SOCK_STREAM, 0);
  if (cliente == -1)
  {
  	printf("Error al abrir el socket\n");
  	return -1;
  }
  printf("Abierto el socket para el cliente...\n");

  //Conectamos
  int conectar = connect(cliente, (struct sockaddr *)&direccion_cliente, sizeof(direccion_cliente));
  if (conectar != 0)
  {
  	printf("Error : No es posible conectar\n");
  	return 1;
  }
  printf("conectado...\n");
  
  //send(cliente, ruta, strlen(ruta), 0);
  
  /*while(1)
  {
  	//char mensaje[BUFLEN];
  	//scanf("%s",mensaje);
  	//send(cliente, mensaje,strlen(mensaje),0);
  }*/

  return 0;
}