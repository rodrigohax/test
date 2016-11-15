/*
Cliente
Tarea 2 - SO
Karla Leal Salazar - Rodrigo Meneses Fierro
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>

#define MAXBUF  512

int main (int argc, char *argv[]) {
  int    sock, nBytes;
  struct sockaddr_in name;
  struct hostent *hp;
  char   buffer[MAXBUF];

  if ( argc < 4 ) {
    printf("Usar : %s <ip-servidor> <puerto> <RUT> \n", argv[0]);
    exit(0);
  }    // socket: Crea un punto de comunicación
    // La función socket retorna un descriptor de archivo (de tipo int)
  sock = socket(AF_INET, SOCK_STREAM, 0);
  if ( sock < 0 ) {
    perror("Error socket() \n");
    printf("socket() %s \n", strerror(errno));

    exit(-1);
  }

    // gethostbyname(argv[1]): retorna una estructura de tipo hostent para un nombre
    // de host dado. En este caso argv[1] es el nombre del host o su dirección IPv4.
  hp = gethostbyname(argv[1]);
  if ( hp == NULL ) {
    perror("\nError en gethostbyname(char *) \n");
    printf("gethostbyname() %s \n", strerror(errno));

    exit(-1);
  }

    // Llenado de la estructura name de tipo struct sockaddr_in
    name.sin_family = AF_INET; // Para protocolo de Internet IPv4.
    // bcopy: copia una secuencia de hp->h_length bytes desde la fuente hp->h_addr a
    // name.sin_addr
    bcopy ((char *)hp->h_addr, (char *)&name.sin_addr, hp->h_length);
    // Copiamos el número puerto dado por el server name.sin_port. Con la función
    // htons transformamos la representación entera del puerto a network-order
    // (big-endian).
    name.sin_port = htons(atoi(argv[2]));
    // connect: inicia una conexión usando el socket sock a la dirección contenida
    // en la estructura name.
    int con = connect(sock, (struct sockaddr *)&name, sizeof name);
    if ( con < 0 ) {
      perror("Error connect() \n");
      printf("connect() %s \n", strerror(errno));

      exit(-1);
    }
    int sent = send(sock,argv[3],sizeof(argv[3]),0);
    // Envia solicitud
    char solicitud[10];
    printf("Ingrese su solicitud\n");
    scanf("%s", solicitud);
    strcpy(buffer, solicitud);

    // Envía el contenido string (del tamaño del buffer)
    sent = send(sock, buffer, strlen(buffer), 0);
    if ( sent < 0 ) {
      perror("Error send() \n");
      printf("send() %s \n", strerror(errno));
      exit(-1);
    }
    
    exit(0);
  }

