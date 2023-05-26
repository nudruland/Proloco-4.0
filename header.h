#ifndef _HEADER_H_
#define _HEADER_H_

#include <stdbool.h>
#include <unistd.h>
#include <stdint.h>

#define SIZE_NAME                       30
#define SIZE_COMMENTO                   100
#define SIZE_LISTA_PRENOTATI            100

#define MAX_NUMBER_PLANCE               90
#define MAX_TAVOLI_5_PLANCE             10
#define MAX_TAVOLI_4_PLANCE             2
#define MAX_TAVOLI_3_PLANCE             3
#define MIN_TAVOLI_1_PLANCIA            3


#define MAX_N_PERSON_X_PLANCIA          8
#define MAX_N_PERSON_SINGLE_TAVOLO      6  
#define MIN_NUMBER_X_PLANCIA            5
#define MAX_NUMBER_X_PLANCIA            8 //questo serve a quando facciamo le coppie, qua accoppiati al massimo    

#define VICINO_A                        " vicino a"

#define SIZE_PACKET_TO_SEND             (8*sizeof(uint8_t) + (2*SIZE_NAME)) 
#define NUMBER_PACKET_TO_SEND_TOTAL     10
#define PACKET_NEW_TABLE                0xFF
#define PACKET_TO_STOP                  0x02

typedef struct{
    int n_plance_intere;
    int n_posti_over;
}somma_check;

typedef struct{
    uint8_t id_plancia;
    uint8_t numero_plance;
    uint8_t posti_occupati;
    uint8_t posti_vuoti;
    char nome_1[SIZE_NAME];
    char nome_2[SIZE_NAME];
    char nome_3[SIZE_NAME];
    char nome_4[SIZE_NAME];
    char commento[SIZE_COMMENTO];
}tavolo;

typedef struct{
    char nome[SIZE_NAME];
    int numero;
    char commento[SIZE_COMMENTO];
    int n_plance_intere;
    int n_posti_over;
    int ID_plancia;
    bool composta;
    bool placed;
} prenotati;

extern uint8_t packet_to_send[SIZE_PACKET_TO_SEND];
extern uint8_t packet_to_send_total[SIZE_PACKET_TO_SEND * NUMBER_PACKET_TO_SEND_TOTAL];

extern tavolo lista_tavoli[MAX_NUMBER_PLANCE];
extern prenotati lista_prenotati[SIZE_LISTA_PRENOTATI];
extern int number_prenotazioni;
extern int number_combined;
extern uint8_t number_tavoli;
extern uint8_t number_plance;

void txt_read(void);
void divisione_piazza(void);
void aggregazione_vicino(void);
void composizione_tavoli(void);
void printChar_my(uint8_t *world, int end);


#endif 