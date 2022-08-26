#include "../controller/managerController.h"
#include "viewUtils.h"
#include "../model/Client.h"
#include "choiceView.h"
#include "../utils/IOUtils.h"
#include "../model/Plant.h"
#include "../model/User.h"
#include "../model/Price.h"
#include <stdlib.h>

const char *managerMenuOptions[] = {
    "Mostra piante",
    "Aggiungi una nuova specie",
    "Aggiungi una colorazione ad una specie",
    "Cambia il prezzo di una specie",
    "Mostra il listino prezzi di una specie",
    "Aggiungi un nuovo utente",
    "Esci"
    };
const int managerMenuLenght = 7;

void managerViewHeader() {
    colorPrint("******************************************\n", GREEN_TEXT);
	colorPrint("*                 Manager                *\n", GREEN_TEXT);
	colorPrint("******************************************\n\n", GREEN_TEXT);
}

int getManagerOption() {
    return getUserOption(managerMenuOptions, managerMenuLenght);
}

bool getWhileRole(Role* role) {
    int temp;
loop:
    while(!getIntegerFromUser(&temp, "Inserire il ruolo del nuovo utente:\n0. SEGRETERIA\n1. MANAGER\n2. OPERATORE\n3. GESTORE MAGAZINO\n")) {
        char errorMessage[150];
        sprintf(errorMessage, "Errore Lettura Ruolo. Inserirlo nuovamente. \nPer annullare l'opearazione di inserimento, inserire '-1'.\n");
        printError(errorMessage);
    }
    if(temp == -1) {
        return false;
    }
    switch (temp) {
        case SEGRETERIA:
            *role = SEGRETERIA;
            break;
        case MANAGER:
            *role = MANAGER;
            break;
        case OPERATORE:
            *role = OPERATORE;
            break;
        case GESTORE_MAGAZINO:
            *role = GESTORE_MAGAZINO;
            break;
        default:
            printError("Ruolo inserito non valido. Inserirlo di nuovo.\n");
            goto loop;
    }
    return true;
}

bool getUserInformationView(User* user, Role* role) {

    if(!getWhileInputView("Username", user->username, USERNAME_MAX_SIZE)) {
        printError("Operazione annullata.");
        return false;
    }

    if(!getWhileInputView("Password", user->password, PASSWORD_MAX_SIZE)) {
        printError("Operazione annullata.");
        return false;
    }

    if(!getWhileRole(role)) {
        printError("Operazione annullata.");
        return false;
    }

    return true;
}

bool getPlantInformationView(Plant* plant) {
    
    if(!getWhileInputView("Codice Specie", plant->codiceSpecie, CODE_MAX_SIZE)) {
        printError("Operazione annullata.");
        return false;
    }

    if(!getWhileInputView("Nome Latino", plant->nomeLatino, NAME_MAX_SIZE)) {
        printError("Operazione annullata.");
        return false;
    }

    if(!getWhileInputView("Nome Comune", plant->nomeComune, NAME_MAX_SIZE)) {
        printError("Operazione annullata.");
        return false;
    }

    if(!getWhileIntegerInputView("Giacenza", &(plant->giacenza))) {
        printError("Operazione annullata.");
        return false;
    }

    if(getWhileCharInputView("Esotica (S/n)", 's', 'n')) {
        plant->esotica = 'e'; 
    } else {
        plant->esotica = 'n';
    }

    if(getWhileCharInputView("Da interno(i) o esterno(e)? (i/e)", 'i', 'e')) {
        plant->interno = 'i'; 
    } else {
        plant->interno = 'e';
    }

    char price[20];
    bool condition;
    do {
        if(!getWhileInputView("Prezzo iniziale", price, 20)) {
            printError("Operazione annullata.");
            return false;
        }
        plant->prezzo = atof(price);
        if(plant->prezzo == 0.0) {
            condition = false;
            printError("Prezzo inserito non valido.");
        } else {
            condition = true;
        }
    } while(!condition);

    return true;
}

bool getColorView(char* codiceSpecie, char* colore) {

    if(!getWhileInputView("Codice Specie", codiceSpecie, CODE_MAX_SIZE)) {
        printError("Operazione annullata.");
        return false;
    }

    if(!getWhileInputView("Colore", colore, 30)) {
        printError("Operazione annullata.");
        return false;
    }

    return true;
}
    
bool getPriceView(char* codiceSpecie, double* price) {

    if(!getWhileInputView("Codice Specie", codiceSpecie, CODE_MAX_SIZE)) {
        printError("Operazione annullata.");
        return false;
    }

    if(!getWhileDoubleInputView("Prezzo", price)) {
        printError("Operazione annullata.");
        return false;
    }

    return true;
}

void printAllPrices(Price** prices, int numPrices) {
    printf("\n|  Prezzo  |     Data Inizio     |     Data Fine     |\n");
    printf("‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾\n");

    for(int i = 0; i < numPrices; i++) {
        char initialDate[15];
        getDateString(prices[i]->initialDate, initialDate);
        char finalDate[15];
        getDateString(prices[i]->finalDate, finalDate);
        char price[10];

        sprintf(price, "%.2f", prices[i]->price);

        printValue(price, 10);
        printValue(initialDate, 21);
        printValue(finalDate, 19);
        colorPrint("|\n", GREEN_TEXT);
    }
}