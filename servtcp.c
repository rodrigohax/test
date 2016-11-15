/*Servidor Concurrente*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netdb.h>
#include <sys/un.h>
#include <signal.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#define MAXBUF 512
#define MAXNAME 10
#define K 5

int  count=0, fd[2], candidatos[K] = {0,0,0,0,0};
char tarea[8];
int sock,s,n,lenght,i,nbytes;
char buffer[MAXBUF];


void servidor_esclavo(int sock);

int main () {
    void ctrlc();
    pid_t pidhijo;
    struct sockaddr_in name;
    //Se crea pipe para comunicación interprocesos
    pipe(fd);

    // Programa la rutina o función de atención de interrupciones.
    signal (SIGINT,ctrlc);
    // socket: Crea un punto de comunicación
    // La función socket retorna un descriptor de archivos (de tipo int)
    // Este socket se usa sólo para aceptar conexiones (listener)
    sock = socket(AF_INET,SOCK_STREAM,0);
    if(sock == -1){
        perror("Error en socket(int, int, int) \n");
        printf("socket() %s \n", strerror(errno));
        exit(-1);
    }

    // Llenado de la estructura name de tipo struct sockaddr_in
    name.sin_family = AF_INET; // Para protocolo de Internet IPv4.
    // El socket listener es enlazado usando la constante INADDR_ANY
    // a cualquier dirección IP del host o máquina donde se ejecuta el server. 
    // Esto sirve cuando el host tiene más de una interface o dirección IP asignada.
    name.sin_addr.s_addr = INADDR_ANY;
    // El puerto se inicializa en 0 para que el SO le asigne uno.
    name.sin_port = 0;
    // Enlaza el nombre o estructura 'name' al socket 'sock' (Asignando nombre y puerto al socket)
    int b = bind(sock,(struct sockaddr *)&name, sizeof name);
    if( b == -1 ) {
        perror("Error en bind() \n");
        printf("bind() %s \n", strerror(errno));
        exit(-1);
    }
    
    // Calcula tamaño en bytes de la estructura name.
    lenght = sizeof name;
    // getsockname: retorna la dirección a la cual el socket 'sock' está
    // ligado. Este dirección queda en la estructura 'name'. También asigna 
    // el puerto real de coomunicación.
    int sock_name = getsockname(sock, (struct sockaddr *)&name, &lenght);
    if ( sock_name == -1 ) {
        perror("Error en getsockname() \n");
        printf("getsockname() %s \n", strerror(errno));
        exit(-1);
    }
    
    // ntohs: convierte representación de red a representación del host local.
    printf ("El puerto asociada al socket es %d\n", ntohs(name.sin_port));
    printf ("El servidor esta  OK, Para bajarlo  Presione Control C\n");
    // Define el tamaño máximo de la cola de espera para mensajes de entrada
    int l = listen (sock, 5);
    if( l < 0 ) {
        perror("Error en listen() \n");
        printf("listen() %s \n", strerror(errno));
        exit(-1);
    }
    
    while(1) {
        // accept: espera por conexiones remotas de clientes. Retorna un
        // descriptor de socket con la cual un proceso esclavo se va a
        // comunicar con un cliente.
        s = accept(sock,0,0);
        if ( s < 0 ) {
            perror("Error en accept() \n");
            printf("accept() %s \n", strerror(errno));
            exit(-1);   
        }

        pidhijo = fork();       
        if ( pidhijo < 0 ) {
            perror("Error en fork() \n");
            printf("fork() %s \n", strerror(errno));
            exit(-1);
        }

        if ( pidhijo == 0 ){
            servidor_esclavo(s);
            exit(0);
        }

        else{
            /* Lee un string del pipe */
            read(fd[0], buffer, sizeof(buffer));
            int opcion = *buffer - '0';

            switch(opcion){
                case 1:
                printf("vote por charlatan 1\n");
                candidatos[0]++;
                break;
                case 2:
                printf("vote x dario\n");
                candidatos[1]++;
                break;
                case 3:
                printf("vote x flakito\n");
                candidatos[2]++;
                break;
                case 4:
                printf("vote x peppa\n");
                candidatos[3]++;
                break;
                default : 
                printf("elije una weaxdddddd\n");
                break;
            }

            printf("**************************************\n");
            printf("************ RESULTADOS **************\n");
            printf("**************************************\n");
            printf("Charlatan: %d\n",candidatos[0]);
            printf("Dario: %d\n",candidatos[1]);
            printf("Flakito: %d\n", candidatos[2]);
            printf("Peppa: %d\n", candidatos[3]);

            wait(NULL);
        }
    }

    exit(0);
}

void servidor_esclavo(int sock) {
    int nbytes;
    
    nbytes = recv(sock,buffer,sizeof(buffer),0);
    if ( nbytes <=0 ) {
        close (sock);
        printf ("Fin de la Conexion\n");        
    }

    //printf("Proceso hijo: %d, recibio RUT: %s\n", getpid(), buffer);

    recv(sock,buffer,sizeof(buffer),0);

        /* Proceso hijo cierra la entrada del pipe */
    /* Envia el string a traves del pipe */
    write(fd[1], buffer, sizeof(buffer));
    close(fd[1]);
}


void ctrlc() {
    printf("\nServidor is OUT\n");
    exit(0);
}

