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

void send_all_tables(int localnewfd)
{
    for(int i = 0; i < MAX_NUMBER_PLANCE; i++){
        build_packet_to_send(i);
        send(localnewfd, &packet_to_send[0], SIZE_PACKET_TO_SEND, 0);
    }
}

void build_packet_to_send(int index)
{

    memset(packet_to_send, 0xFF, SIZE_PACKET_TO_SEND);
    packet_to_send[0] = PACKET_NEW_TABLE;
    packet_to_send[1] = lista_tavoli[index].id_plancia;
    packet_to_send[2] = lista_tavoli[index].numero_plance;
    packet_to_send[3] = lista_tavoli[index].numero_plance * 8;
    packet_to_send[4] = lista_tavoli[index].posti_vuoti;
    packet_to_send[5] = lista_tavoli[index].posti_occupati;
    memcpy((packet_to_send + 6), &(lista_tavoli[0].nome_1), sizeof(lista_tavoli[0].nome_1));
    return;
}

void build_packet_to_send_fake()
{
    packet_to_send[0] = PACKET_TO_STOP;
    return;
}
