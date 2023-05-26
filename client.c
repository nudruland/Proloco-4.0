#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>

#include "header.h"

#define PORT "8080" // the port users will be connecting to

#define BACKLOG 10 // how many pending connections queue will hold

uint8_t packet_to_send[SIZE_PACKET_TO_SEND];
uint8_t packet_to_send_total[SIZE_PACKET_TO_SEND * NUMBER_PACKET_TO_SEND_TOTAL];

void printChar_my(uint8_t *world, int end)
{
    for(int i = 0; i < end; i++)
    {
        printf("%c", (char)(*(world + i)));
    }
    printf("\n");
}

void sigchld_handler(int s)
{
 // waitpid() might overwrite errno, so we save and restore it:
 int saved_errno = errno;

 while(waitpid(-1, NULL, WNOHANG) > 0);

 errno = saved_errno;
}


 // get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

void build_packet_to_send(tavolo mytavolo, int i)
{
    int offset = 0;
    memset(packet_to_send, 0x00, sizeof(packet_to_send));
    packet_to_send[0] = PACKET_NEW_TABLE;
    packet_to_send[1] = mytavolo.id_plancia;
    packet_to_send[2] = mytavolo.numero_plance;
    packet_to_send[3] = mytavolo.numero_plance * 8;
    packet_to_send[4] = mytavolo.posti_vuoti;
    packet_to_send[5] = mytavolo.posti_occupati;
    packet_to_send[6] = (uint8_t)strlen(mytavolo.nome_1);
    memcpy((packet_to_send + 7), &(mytavolo.nome_1), packet_to_send[6]);

    /*packet_to_send[8 + SIZE_NAME] = (uint8_t)strlen(mytavolo.nome_2);
    memcpy((packet_to_send + 9 + SIZE_NAME), &(mytavolo.nome_2), packet_to_send[8 + SIZE_NAME]);*/

    offset = i * SIZE_PACKET_TO_SEND;
    printf("OFFSET: %d i: %d\n", offset, i);
    memcpy(packet_to_send_total + offset, packet_to_send, sizeof(packet_to_send));
    
    return;
}

void build_packet_to_send_fake()
{
    packet_to_send[0] = PACKET_TO_STOP;
    return;
}

int main(void)
{
    int sockfd, new_fd; // listen on sock_fd, new connection on new_fd
    struct addrinfo hints, *servinfo, *p;
    struct sockaddr_storage their_addr; // connector's address information
    socklen_t sin_size;
    struct sigaction sa;
    int yes=1;
    char s[INET6_ADDRSTRLEN];
    int rv;
    int c = 0;
    int ca = 0;
    int length_effectively = 0;

    txt_read();
    aggregazione_vicino();
    composizione_tavoli();
    divisione_piazza();

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET; //this is to tell if IP è 4,6 o non specificato
    hints.ai_socktype = SOCK_STREAM; //tcp/ip 
    hints.ai_flags = AI_PASSIVE; // use my IP

    /*the following function is needed to associate the host (here NULL), PORT,
    and the hints to servinfo.
    */
    if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

 // loop through all the results and bind to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {

        if ((sockfd = socket(p->ai_family, p->ai_socktype,
        p->ai_protocol)) == -1) {
        perror("server: socket");
        continue;
        }

        //used to be sure that the port is free
        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
        sizeof(int)) == -1) {
        perror("setsockopt");
        exit(1);
        }

        //used to associate the socket to a specific local port. Use to listen
        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
        close(sockfd);
        perror("server: bind");
        continue;
        }

        break;
    }

    freeaddrinfo(servinfo); // all done with this structure

    if (p == NULL) {
        fprintf(stderr, "server: failed to bind\n");
        exit(1);
    }

    //this is used in order to listen which client want to connect to me.
    //BACKLOG is the max number of client that i want to listen
    if (listen(sockfd, BACKLOG) == -1) {
        perror("listen");
        exit(1);
    }

    sa.sa_handler = sigchld_handler; // reap all dead processes
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }   
    
    printf("server: waiting for connections...\n"); 
    while(1) { // main accept() loop
        
        if(!ca)
        {
            sin_size = sizeof their_addr;
            new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
            close(sockfd); // child doesn't need the listener
            if (new_fd == -1) {
                perror("accept");
                continue;
            }else{
                ca ++;
            }   

            inet_ntop(their_addr.ss_family,
            get_in_addr((struct sockaddr *)&their_addr),
            s, sizeof s);
            printf("server: got connection from %s\n", s);
        }  

        if(ca == 1)
        {
            /*build_packet_to_send(lista_tavoli[1]);
            if (length_effectively = send(new_fd, packet_to_send, sizeof(packet_to_send), 0) == -1)
            //if (send(new_fd, "Hello, World!", 13, 0) == -1)
            {
                perror("send");
            }else{
                printf("lunghezza pacchetto: %d \t la size define é: %d", length_effectively, SIZE_PACKET_TO_SEND);
            }*/
            memset(packet_to_send_total, 0x00, sizeof(packet_to_send_total));
            for(int i = 0; i < 10/*number_tavoli*/; i++)
            {
                build_packet_to_send(lista_tavoli[i], i);
            }

            if (length_effectively = send(new_fd, packet_to_send_total, sizeof(packet_to_send_total), 0) == -1)
            //if (send(new_fd, "Hello, World!", 13, 0) == -1)
            {
                perror("send");
            }
            ca++;
        }
        /*
        if(ca == 2)
        {
            build_packet_to_send_fake();
            if (length_effectively = send(new_fd, packet_to_send, sizeof(packet_to_send), 0) == -1)
            //if (send(new_fd, "Hello, World!", 13, 0) == -1)
            {
                perror("send");
            }else{
                printf("lunghezza pacchetto: %d \t la size define é: %d\n", length_effectively, SIZE_PACKET_TO_SEND);
            }
            ca++;
        }*/

    }  
    return 0;
}