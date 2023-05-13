#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "header.h"

prenotati lista_prenotati[SIZE_LISTA_PRENOTATI];
tavolo lista_tavoli[MAX_NUMBER_PLANCE];
FILE *fp;

char name_file[] = "to_ignore/prenotati.txt";
int number_prenotazioni = 0;
int number_combined = 0;
int number_tavoli = 0;
int number_plance = 0;

void bubbleSort(prenotati arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j].n_posti_over > arr[j + 1].n_posti_over) {
                // Scambia le strutture
                prenotati temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

void txt_read(void)
{
    char ch[130];
    int i;
    int position_first_stop = 0;
    int position_second_stop = 0;
    
    int start_num = 0;
    bool find_num = false;
    char *num;
    int num_int = 0;
    int size_raw_num = 0;

    fp = fopen(name_file, "r");
    
    if(fp == NULL)
    {
        printf("UNABLE TO OPEN FILE\n");
    }

    while (fgets(ch, sizeof(ch), fp) != NULL) 
    {
        i = 0;
        position_first_stop = 0;
        position_second_stop = 0;
        size_raw_num = 0;
        start_num = 0;
        find_num = false;

        while(ch[i] != ',' && i < strlen(ch))
        {
            i++;
        }

        memcpy(lista_prenotati[number_prenotazioni].nome, ch, i);
        position_first_stop = i;
        i++;

        while(ch[i] != ',' && i < strlen(ch))
        {
            i++;
        }

        position_second_stop = i;
        size_raw_num = position_second_stop - position_first_stop - 1;
        
        num = (char *)malloc(size_raw_num*sizeof(char));
        memset(num, 0x00,size_raw_num*sizeof(char));
        memcpy(num, ch + position_first_stop + 1, position_second_stop - position_first_stop - 1);

        for(int k = 0; k < size_raw_num; k++)
        {
            if( ((int)(*(num + k)) > 47 && (int)(*(num + k)) < 58) && !find_num )
            {
                find_num = true;
                start_num = k;
            }

            if(find_num)
            {
                num_int = (int)(*(num + k)) - 48;
                lista_prenotati[number_prenotazioni].numero += (int)pow(10, size_raw_num - k - 1) * num_int;
            } 
        }

        //qua devo inserire il numero di plance intere che riempie e i posti extra. 
        lista_prenotati[number_prenotazioni].n_plance_intere = lista_prenotati[number_prenotazioni].numero / MAX_N_PERSON_X_PLANCIA;
        lista_prenotati[number_prenotazioni].n_posti_over = lista_prenotati[number_prenotazioni].numero % MAX_N_PERSON_X_PLANCIA;
        lista_prenotati[number_prenotazioni].placed = false;
        lista_prenotati[number_prenotazioni].composta = false;

        memcpy(lista_prenotati[number_prenotazioni].commento, ch + position_second_stop + 1, strlen(ch) - 2 - i);
        
        free(num);
        number_prenotazioni++;
    }
    number_prenotazioni--;

    return;
}

void aggregazione_vicino(void)
{
    int k = 0;
    bool find = false;
    char string_to_search[] = VICINO_A;
    char string_to_add[] = " + ";
    number_combined = number_prenotazioni;
 
    for(int i = 0; i < number_combined; i++) //devo ricordarmi che una volta che lo trovo, lo elimino e copio al suo posto l'ultimo della lista
    {   
        find = false;
        k = 0;
        if(!strncmp(lista_prenotati[i].commento, string_to_search, sizeof(string_to_search) - 1))
        {
            while(!find && k <= number_combined)
            {
                if(strstr(lista_prenotati[i].commento, lista_prenotati[k].nome)!=NULL)
                {
                    find = true;

                    strcat(lista_prenotati[i].nome, string_to_add);
                    strcat(lista_prenotati[i].nome, lista_prenotati[k].nome);
                    memset(lista_prenotati[i].commento, 0x00, strlen(lista_prenotati[i].commento));
                    strcat(lista_prenotati[i].commento, lista_prenotati[k].commento);

                    lista_prenotati[k].placed = false;
                    lista_prenotati[i].numero += lista_prenotati[k].numero;
                    lista_prenotati[i].n_plance_intere = lista_prenotati[i].numero / MAX_N_PERSON_X_PLANCIA;
                    lista_prenotati[i].n_posti_over = lista_prenotati[i].numero % MAX_N_PERSON_X_PLANCIA;

                    if(k != number_combined)
                    {
                        lista_prenotati[k] = lista_prenotati[number_combined - 1 ];
                    }else{
                        lista_prenotati[k] = lista_prenotati[number_combined - 2];
                    }
                    number_combined--;
                
                }
                k++;
            }

            if(k == number_combined + 1 && !find){
                
                printf("%s STATTI SOLO MERDA\n", lista_prenotati[i].nome);
            
            }
        }

    }
}

void composizione_tavoli(void)
{
    int i = 0;
    int k = 0;
    somma_check sum;
    int local_check;

    for(i = 0; i < number_combined; i++)
    {
        for(k = i; k < number_combined; k++)
        {
            if(i != k && !lista_prenotati[k].composta && !lista_prenotati[i].composta)
            {
                local_check = lista_prenotati[i].n_posti_over + lista_prenotati[k].n_posti_over;
                sum.n_posti_over = local_check % MAX_N_PERSON_X_PLANCIA;
                sum.n_plance_intere = local_check / MAX_N_PERSON_X_PLANCIA;
                local_check = sum.n_plance_intere + lista_prenotati[i].n_plance_intere + lista_prenotati[k].n_plance_intere;
                if(sum.n_posti_over >= MIN_NUMBER_X_PLANCIA && sum.n_posti_over <= MAX_NUMBER_X_PLANCIA && (local_check < 10) /*&& !strcmp(lista_prenotati[i].commento, lista_prenotati[k].commento)*/)
                {
                    number_plance += local_check + 1;

                    lista_prenotati[i].composta = true;
                    lista_prenotati[k].composta = true;
                    
                    lista_tavoli[number_tavoli].numero_plance = local_check + 1;
                    lista_tavoli[number_tavoli].posti_occupati = lista_prenotati[i].numero + lista_prenotati[k].numero;
                    lista_tavoli[number_tavoli].posti_vuoti = (lista_tavoli[number_tavoli].numero_plance) * MAX_N_PERSON_X_PLANCIA - lista_tavoli[number_tavoli].posti_occupati;
                    
                    strcpy(lista_tavoli[number_tavoli].nome_1, lista_prenotati[i].nome);
                    strcpy(lista_tavoli[number_tavoli].nome_2, lista_prenotati[k].nome);
                    strcpy(lista_tavoli[number_tavoli].commento, lista_prenotati[i].commento);

                    number_tavoli++;
                }
            }
        }

        if(!lista_prenotati[k].composta)
        {
            

            lista_prenotati[k].composta = true;

            lista_tavoli[number_tavoli].numero_plance = lista_prenotati[i].n_plance_intere + (lista_prenotati[i].n_posti_over > 0)*1;
            lista_tavoli[number_tavoli].posti_occupati = lista_prenotati[i].numero;
            lista_tavoli[number_tavoli].posti_vuoti = (lista_tavoli[number_tavoli].numero_plance) * MAX_N_PERSON_X_PLANCIA - lista_tavoli[number_tavoli].posti_occupati;
            strcpy(lista_tavoli[number_tavoli].nome_1, lista_prenotati[i].nome);
            strcpy(lista_tavoli[number_tavoli].commento, lista_prenotati[i].commento);
            number_tavoli++;
            number_plance += lista_tavoli[number_tavoli].numero_plance; 
        }
    }

    for(int i = 0; i < number_tavoli; i++)
    {
        printf("TAVLO %d: \tNUMERO PLANCE: %d\tPOSTI OCCUPATI: %d\tPOSTI VUOTI: %d\n\t\tPRIMO_NOME: %s\tSECONDO_NOME: %s", i + 1, lista_tavoli[i].numero_plance, lista_tavoli[i].posti_occupati, lista_tavoli[i].posti_vuoti, lista_tavoli[i].nome_1, lista_tavoli[i].nome_2);
        printf("\n\n");
        
    }
    printf("PLANCE TOTALI: %d\n", number_plance);

}

void divisione_piazza(void)
{
    
    //ordinarle
    for(int i = 0; i < number_combined; i++)
    {
        printf("%s %d %d\n", lista_prenotati[i].nome, lista_prenotati[i].n_posti_over, lista_prenotati[i].numero);
    }

    printf("\n");

    /*bubbleSort(lista_prenotati, number_combined);

    for(int i = 0; i < number_combined; i++)
    {
        printf("%s %d\n", lista_prenotati[i].nome, lista_prenotati[i].n_posti_over);
    }*/


}