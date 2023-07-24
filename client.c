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

void build_packet_to_send(tavolo mytavolo)
{
    //diocane
    int a;
    packet_to_send[0] = PACKET_NEW_TABLE;
    packet_to_send[1] = mytavolo.id_plancia;
    packet_to_send[2] = mytavolo.numero_plance;
    packet_to_send[3] = mytavolo.numero_plance * 8;
    packet_to_send[4] = mytavolo.posti_vuoti;
    packet_to_send[5] = mytavolo.posti_occupati;
    memcpy((packet_to_send + 6), &(mytavolo.nome_1), sizeof(mytavolo.nome_1));
    return;
}

void build_packet_to_send_fake()
{
    packet_to_send[0] = PACKET_TO_STOP;
    return;
}
