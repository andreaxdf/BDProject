
#include "viewUtils.h"

void clearScreen() {
    printf("\033[2J\033[H");
}

void showHeader() {
	colorPrint("******************************************\n", GREEN_TEXT);
	colorPrint("*     Verde S.r.l. Management System     *\n", GREEN_TEXT);
	colorPrint("******************************************\n\n", GREEN_TEXT);
}

void showMenu(const char **menuOptionsArray, int optionsNumber) {
    for (int i = 0 ; i < optionsNumber ; i++) {
        printf("%d. %s\n", i, menuOptionsArray[i]);
    }
    printf("Scegliere Opzione [%d-%d] >>> ", 0, optionsNumber - 1);
}

int getUserOption(const char *menuOption[], int menuLen) {
    printf("\n");
    colorPrint("Cosa posso fare per te?", GREEN_HIGH);
    printf("\n");
    showMenu(menuOption, menuLen);

    int selectedOption ;
    if (!getIntegerFromUser(&selectedOption, "")) {
        selectedOption = -1;
    } else if (selectedOption < 0 || selectedOption > menuLen - 1) {
        char message[70];
        sprintf(message, "\nNumero inserito non valido. Inserire numeri tra %d e %d\n", 0, menuLen - 1);
        colorPrint(message, RED_TEXT);
    }
    return selectedOption;
}

/*
    Funzione che chiede all'utente le informazioni per inizializzare un indirizzo.
    - address: indirizzo della variabile in cui inserire i risultati.
*/
bool getAddressView(Address* address) {

    if(!getWhileInputView("Via", address->via, VIA_MAX_SIZE)) {
        return false;
    }

    if(!getWhileInputView("Numero civico", address->civico, CIVICO_MAX_SIZE)) {
        return false;
    }

    if(!getWhileInputView("Città", address->citta, CITTA_MAX_SIZE)) {
        return false;
    }

    return true;
}

/*
    Funzione che chiede all'utente le informazioni per inizializzare un contatto.
    - contact: indirizzo della variabile in cui inserire i risultati.
*/
bool getContactView(Contact* contact) {
    char message[] = "\nQual è il tipo di contatto da inserire?\n0. Telefono\n1. Cellulare\n2. Email\n"; 
type:
    if(!getIntegerFromUser((int *)&(contact->type), message)) {
        goto type;
    }

    char typeMessage[17];
    switch (contact->type) {
        case PHONE:
            strcpy(typeMessage, "Numero telefono");
            break;
        case CELL_PHONE:
            strcpy(typeMessage, "Numero cellulare");
            break;
        case EMAIL:
            strcpy(typeMessage, "Indirizzo email");
            break;
    }

    if(!getWhileInputView(typeMessage, contact->value, CONTACT_VALUE_MAX_SIZE)) {
        return false;
    }

    return true;
}

bool getReferentView(Referent* referent) {
    if(!getWhileInputView("Nome Referente", referent->nome, NAME_MAX_SIZE)) {
        return false;
    }

    if(!getWhileInputView("Cognome Referente", referent->cognome, SURNAME_MAX_SIZE)) {
        return false;
    }

    return true;
}

void printValue(char* value, int width) {
    char message[80];

    colorPrint("|", GREEN_TEXT);
    int len = strlen(value);
    sprintf(message, "%s", value);
    colorPrint(message, GREEN_TEXT);
    if(len < width) {
        for(int i = 0; i < width-len; i++) {
            printf(" ");
        }
    }
}