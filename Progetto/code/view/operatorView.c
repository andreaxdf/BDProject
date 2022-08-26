#include "operatorView.h"
#include "choiceView.h"
#include "../utils/IOUtils.h"
#include "viewUtils.h"

const char* operatorMenuOptions[] = {
    "Aggiungi Pacco",
    "Mostra piante da inviare",
    "Mostra ordini da evadere",
    "Mostra dettagli ordine di vendita",
    "Esci"
    };
const int operatorMenuLenght = 5;

int getOperatorOption() {
    return getUserOption(operatorMenuOptions, operatorMenuLenght);
}

void operatorViewHeader() {
    colorPrint("******************************************\n", GREEN_TEXT);
	colorPrint("*                Operatore               *\n", GREEN_TEXT);
	colorPrint("******************************************\n\n", GREEN_TEXT);
}

void printCodeAndQuantity(Plant** plants, int numPlants) {
    printf("\n|  Codice  |  Quantità  |\n");
    printf("‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾\n");

    for(int i = 0; i < numPlants; i++) {
        char temp[10];
        
        printValue(plants[i]->codiceSpecie, 10);
        sprintf(temp, "%d", plants[i]->giacenza);
        printValue(temp, 12);
        colorPrint("|\n", GREEN_TEXT);
    }
}

void printOrderCodes(int** orders, int numOrders) {
    printf("\n|  Codice  |\n");
    printf("‾‾‾‾‾‾‾‾‾‾‾‾\n");

    for(int i = 0; i < numOrders; i++) {
        char temp[10];
        
        sprintf(temp, "%d", *(orders[i]));
        printValue(temp, 10);
        colorPrint("|\n", GREEN_TEXT);
    }
}