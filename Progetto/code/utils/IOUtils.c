#include "IOUtils.h"
#include "../model/Price.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <errno.h>
#include <ctype.h>
#include <limits.h>
#include "systemUtils.h"




void getDateString(Date date, char* result) {
    sprintf(result, "%d-%d-%d", date.day, date.month, date.year);
}

void printError(char *errorMessage) {
    colorPrint(errorMessage, RED_TEXT) ;
    printf("\n") ;
}


bool getUserInput(char *requestString, char *resultBuffer, int bufferSize) {
    /*
        Funzione per prendere l'input utente da stdinput e restituirlo con resultBuffer.
        bufferSize comprende nel conto lo \0
    */

    //printf("%s", requestString) ;
    colorPrint(requestString, GREEN_TEXT);

    //Alloco un buffer di dimensione pari alla massima dimensione valida per l'input più uno per lo \n
    char inputBuffer[bufferSize + 1] ;

    //Lettura al massimo di inputMaxSize - 1 caratteri incluso, se lo trova, il \n
    if (fgets(inputBuffer, bufferSize + 1, stdin) == NULL) {
        printError("Errore Scansione Input") ;
        return false ;
    }

    //Rimozione eventuale \n letto da fgets
    for (int i = 0 ; i < (int) strlen(inputBuffer) ; i++) if (inputBuffer[i] == '\n') inputBuffer[i] = '\0' ;

    /*
        Se ho una lunghezza di ciò che ho letto pari al massimo leggibile, 
        significa che sul canale di input è rimasto ALMENO lo \n,
        quindi devo rimuovere il resto dell'input per non leggerlo alla lettura successiva.
        Significa inoltre che ho letto almeno un carattere in più della dimensione massima prevista e quindi l'input
        non è valido
    */
    if ((int) strlen(inputBuffer) == bufferSize) {
        while(getchar() != '\n') ;
        printError("Input Inserito Troppo Lungo") ;
        return false ;
    }

    strcpy(resultBuffer, inputBuffer) ;

    /*
        Aggiunta controllo input non vuoto.
        Vantaggi:
            - evito di ricontrollare ogni volta che la funzione viene chiamata
            - Se viene chiesto un input all'utente ci si aspetta che esso venga inserito. 
              Se non lo fa c'è un errore e posso ritornare false 
    */
    if (strlen(resultBuffer) == 0) return false ;
    
    return true ;
}


void colorPrint(char *printText , TextColorEnum colorEnum) {
    int color = 0 ;
    switch (colorEnum) {
        case (RED_TEXT) :
            color = RED_TEXT_MARK ;
            break;
        case (GREEN_TEXT) :
            color = GREEN_TEXT_MARK ;
            break ;
        case (RED_HIGH) :
            color = RED_HIGH_TEXT_MARK ;
            break ;
        case (GREEN_HIGH) :
            color = GREEN_HIGH_TEXT_MARK ;
            break;
    }

    printf("\033[%dm%s\033[m", color, printText) ;
} 

bool getIntegerFromUser(int *integerPtr, char *resultMessage) {
    char integerStringBuff[11 + 1] ;
    if (!getUserInput(resultMessage, integerStringBuff, 11 + 1)) {
        printError("Errore Inserimento Codice Numerico") ;
        return false ;
    }
    errno = 0;
    char *checkString = "\0" ;
    long longInput = strtol(integerStringBuff, &checkString, 10) ;
    if (errno != 0) {
        printError("Numero Non Valido") ;
        errno = 0 ;
        return false ;
    }
    if (*checkString != '\0') {
        printError("Numero Non Valido") ;
        errno = 0 ;
        return false ;
    } ;
    if (longInput > INT_MAX || longInput < INT_MIN) return false ;
    *integerPtr = (int) longInput ;
    
    return true ;
}

bool getCode(char* message,int* codice) {
    colorPrint(message, GREEN_TEXT);
    if(!getIntegerFromUser(codice, "Codice >>> ")) {
        return false;
    }

    return true;
}

/*
    Funzione che chiede l'input fino a quando non si richiede di uscire.
    - request: stringa da mostrare all'utente per la richesta.
    - resultBuffer: buffer in cui inserire il risultato.
    - bufferSize: dimensione buffer.
*/
bool getWhileInputView(char* request, char* resultBuffer, int bufferSize) {
    char message[150];
    sprintf(message, "%s >>> ", request);
    while(!getUserInput(message, resultBuffer, bufferSize)) {
        char errorMessage[150];
        sprintf(errorMessage, "Errore Lettura %s. Inserirlo nuovamente. \nPer annullare l'opearazione di inserimento, inserire 'cancel'.\n", request);
        printError(errorMessage);
    }
    if(strcmp(resultBuffer, "cancel") == 0) {
        return false;
    }
    return true;
}

bool getWhileIntegerInputView(char* request, int* result) {
    char message[150];
    sprintf(message, "%s >>> ", request);
    while(!getIntegerFromUser(result, message)) {
        char errorMessage[150];
        sprintf(errorMessage, "Errore Lettura %s. Inserirlo nuovamente. \nPer annullare l'opearazione di inserimento, inserire '-1'.\n", request);
        printError(errorMessage);
    }
    if(*result == -1) {
        return false;
    }
    return true;
}

bool getWhileCharInputView(char* inputString, char charTrue, char charFalse) {
    char response;
    char message[100];
loop:
    sprintf(message, "%s >>> ", inputString);
    getUserInput(message, &response, 2);
    if(tolower(response) == charTrue) {
        return true;
    } else if(tolower(response) == charFalse) {
        return false;
    } else {
        printError("Errore nell'acquisizione dell'input");
        goto loop;
    }
}

bool getWhileDoubleInputView(char* inputString, double* result) {
    char temp[20];
    bool condition;
    do {
        if(!getWhileInputView(inputString, temp, 20)) {
            printError("Operazione annullata.");
            return false;
        }
        *result = atof(temp);
        if(*result == 0.0) {
            condition = false;
            printError("Prezzo inserito non valido.");
        } else {
            condition = true;
        }
    } while(!condition);

    return true;
}