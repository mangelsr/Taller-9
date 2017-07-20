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

#define BUFLEN 100
#define BUFRD 100

int main(int argc, char ** argv)
{
  int servidor;
  
  if(argc != 3)
  {
  	printf("Error!!\n");
    printf("Modo de Uso: ./servidor <ip> <numero de puerto>\n");
    return -1;
  }

  char* ip = argv[1];
  int puerto = atoi(argv[2]);

  //Direccion del servidor
  struct sockaddr_in direccion_servidor;
  //ponemos en 0 la estructura direccion_servidor
  memset(&direccion_servidor, 0, sizeof(direccion_servidor));

  //llenamos los campos
  //IPv4
  direccion_servidor.sin_family = AF_INET;
  //Convertimos el numero de puerto al endianness de la red
  direccion_servidor.sin_port = htons(puerto);
  //Nos vinculamos a la interface localhost o podemos usar INADDR_ANY para ligarnos A TODAS las interfaces
  direccion_servidor.sin_addr.s_addr = inet_addr(ip);

  //Abrimos un socket
  servidor = socket(((struct sockaddr *)&direccion_servidor)->sa_family, SOCK_STREAM, 0);
  if (servidor == -1)
  {
  	printf("Error al abrir el socket\n");
  	return -1;
  }
  //Para que no haya problemas debido a que el socket siga abierto
  int abierto = 1;
  setsockopt(servidor, SOL_SOCKET, SO_REUSEADDR, &abierto, sizeof(abierto));

  //Enlazamos el socket
  int enlace = bind(servidor, (struct sockaddr *)&direccion_servidor, sizeof(direccion_servidor));
  if(enlace != 0)
  {
  	printf("Error!!!\n");
  	printf("No se puede enlazar al puerto : dirección ya está en uso\n");
    return -1;
  }

  //Ponemos al socket en espera
  int escuchar = listen(servidor,100);
  if(escuchar == -1)
  {
  	printf("No es posible escuchar en ese puerto\n");
  	return -1;
  }
  printf("Enlazado al puerto.\n");

  struct sockaddr_in direccion_cliente;
  memset(&direccion_servidor, 0, sizeof(direccion_cliente));
  unsigned int tam = sizeof(direccion_cliente);
  
  while(1)
  {
    int cliente = accept(servidor,(struct sockaddr *)&direccion_cliente,&tam);
    char *ruta = (char *)malloc(BUFLEN*sizeof(char *));
    char *file = (char *)malloc(BUFRD*sizeof(char *));
  	recv(cliente, ruta, BUFLEN, 0);
    printf("\nUsuario conectado\n");

    printf("Buscanco archivo: %s\n", ruta);

    int fd = open(ruta, O_RDONLY,S_IROTH);
    if (fd < 0)
    {
      printf("Error al abrir el archivo\n");
      char * mensaje = "E";
      send(cliente, mensaje, strlen(mensaje) ,0);
      continue;
    }
      
    printf("Archivo abierto correctamente\n");
    int filesize;
      
    while((filesize = read(fd, file, BUFRD)) > 0)
    {
      send(cliente, file, filesize, 0);
      memset(file, 0, BUFRD);
    }
      
    printf("Archivo enviado correctamente\n");
    close(fd);
    free(file);
    close(cliente);
  }

  return 0;
}