#include "IOUtils.h"


void printError(char *errorMessage) {
    colorPrint(errorMessage, RED_TEXT) ;
    printf("\n") ;
}


bool getUserInput(char *requestString, char *resultBuffer, int bufferSize) {
    /*
        Funzione per prendere l'input utente da stdinput e restituirlo con resultBuffer.
        bufferSize comprende nel conto lo \0
    */

    printf("%s", requestString) ;

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


bool getDateFromUser(Date *datePtr, char *requestString) {
    char dateString[strlen("yyyy-mm-dd") + 1] ;
    if (!(getUserInput(requestString, dateString, strlen("yyyy-mm-dd") + 1))) {
        printError("Errore Inserimento Data") ;
        return false ;
    }

    if (!verifyAndParseDate(datePtr, dateString)) {
        printError("Formato della Data Inserita non Valido") ;
        return false ;
    }

    return true ;
}

bool getTimeFromUser(Time *timePtr, char *requestString) {

    char timeString[strlen("hh:mm") + 1] ;
    if (!(getUserInput(requestString, timeString, strlen("hh:mm") + 1))) {
        printError("Errore Inserimento Orario") ;
        return false ;
    }

    if (!verifyAndParseTime(timePtr, timeString)) {
        printError("Formato Orario Inserito non Valido") ;
        return false ;
    }

    return true ;
}


bool getIntegerFromUser(int *integerPtr, char *resultMessage) {
    char integerStringBuff[11 + 1] ;
    if (!getUserInput(resultMessage, integerStringBuff, 11 + 1)) {
        printError("Errore Inserimento Codice Numerico") ;
        return false ;
    }

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