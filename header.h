#ifndef _HEADER_H_
#define _HEADER_H_

#include <stdbool.h>
#include <unistd.h>

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

typedef struct{
    int n_plance_intere;
    int n_posti_over;
}somma_check;

typedef struct{
    int id_plancia;
    int numero_plance;
    int posti_occupati;
    int posti_vuoti;
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

extern tavolo lista_tavoli[MAX_NUMBER_PLANCE];
extern prenotati lista_prenotati[SIZE_LISTA_PRENOTATI];
extern int number_prenotazioni;
extern int number_combined;
extern int number_tavoli;
extern int number_plance;

void txt_read(void);
void divisione_piazza(void);
void aggregazione_vicino(void);
void composizione_tavoli(void);

#endif 