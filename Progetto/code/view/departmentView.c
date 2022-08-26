#include "departmentView.h"
#include "viewUtils.h"
#include "../utils/IOUtils.h"
#include "../model/Plant.h"
#include "../model/Supplier.h"

const char* departmentMenuOptions[] = {
    "Chiudi ordine e aggiorna giacenza",
    "Mostra le giacenze delle specie",
    "Aggiungi ordine di acquisto",
    "Mostra ordini di acquisto ancora non arrivati",
    "Mostra specie di un ordine di acquisto",
    "Aggiungi fornitore",
    "Aggiungi indirizzo a fornitore",
    "Cerca fornitore per specie",
    "Esci"
    };
const int departmentMenuLenght = 9;

void departmentViewHeader() {
    colorPrint("******************************************\n", GREEN_TEXT);
	colorPrint("*           GESTORE  MAGAZZINO           *\n", GREEN_TEXT);
	colorPrint("******************************************\n\n", GREEN_TEXT);
}

int getDepartmentOption() {
    return getUserOption(departmentMenuOptions, departmentMenuLenght);
}

void printAllStocks(Plant** plants, int numPlants) {
    printf("\n|  Codice  |     Nome Latino     |     Nome Comune     |  Giacenza  |\n");
    printf("‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾\n");

    for(int i = 0; i < numPlants; i++) {
        char temp[10];

        sprintf(temp, "%d", plants[i]->giacenza);

        printValue(plants[i]->codiceSpecie, 10);
        printValue(plants[i]->nomeLatino, 21);
        printValue(plants[i]->nomeComune, 21);
        printValue(temp, 12);
        colorPrint("|\n", GREEN_TEXT);
    }
}

bool getPurchaseOrderValues(char* specie, int* quantità, double* prezzo) {
    if(!getWhileInputView("Codice Specie", specie, CODE_MAX_SIZE)) {
        return false;
    }

    if(!getWhileIntegerInputView("Quantità", quantità)) {
        return false;
    }

   if(!getWhileDoubleInputView("Prezzo", prezzo)) {
        return false;
    }

    return true;
}

bool getSupplierView(Supplier* supplier) {

    if(!getWhileInputView("Nome Fornitore", supplier->nome, NAME_MAX_SIZE)) {
        return false;
    }

   if(!getWhileInputView("Codice Fiscale", supplier->codiceFiscale, CF_MAX_SIZE)) {
        return false;
    }

    return true;
}

void printSupplierInformation(Supplier** supplier, int num) {
    printf("\n|  Codice Fornitore  |  Codice Fiscale  |     Nome Fornitore     |\n");
    printf("‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾\n");

    for(int i = 0; i < num; i++) {
        char temp[10];

        sprintf(temp, "%d", supplier[i]->codiceFornitore);
        printValue(temp, 20);
        printValue(supplier[i]->codiceFiscale, 18);
        printValue(supplier[i]->nome, 24);
        colorPrint("|\n", GREEN_TEXT);
    }
}

void printAllPurchaseOrder(PurchaseOrder** orders, int numOrders) {
    printf("\n|  Codice  |  Data Ordine  |  Codice Fornitore  |  Codice Fiscale  |     Nome Fornitore     |\n");
    printf("‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾\n");

    for(int i = 0; i < numOrders; i++) {
        char temp[10];
        char data[15];

        getDateString(orders[i]->data, data);
        sprintf(temp, "%d", orders[i]->codiceOrdine);
        printValue(temp, 10);

        printValue(data, 15);

        sprintf(temp, "%d", orders[i]->codiceFornitore);
        printValue(temp, 20);
        printValue(orders[i]->codiceFiscale, 18);
        printValue(orders[i]->nome, 24);
        colorPrint("|\n", GREEN_TEXT);
    }
}



void printAllPlantName(Plant** plants, int numPlants) {
    printf("\n|  Codice  |     Nome Latino     |     Nome Comune     |  Quantità  |  Prezzo  |\n");
    printf("‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾\n");

    for(int i = 0; i < numPlants; i++) {
        char temp[10];
        
        printValue(plants[i]->codiceSpecie, 10);
        printValue(plants[i]->nomeLatino, 21);
        printValue(plants[i]->nomeComune, 21);
        sprintf(temp, "%d", plants[i]->giacenza);
        printValue(temp, 12);
        sprintf(temp, "%.2f", plants[i]->prezzo);
        printValue(temp, 10);
        colorPrint("|\n", GREEN_TEXT);
    }
}