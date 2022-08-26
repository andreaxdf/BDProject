#include "operatorController.h"
#include <stdbool.h>
#include "../view/viewUtils.h"
#include "../view/operatorView.h"
#include "../view/secretaryView.h"
#include "../database/operatorDB.h"
#include "secretaryController.h"

void showOrderDetails() {
    showSalesOrder();
}

void addPackage() {
    char specie[CODE_MAX_SIZE];
    int quantità;
    int ordine;
    int pacco;

    if(!getWhileIntegerInputView("Codice Ordine", &ordine)) {
        printError("Operazione annullata.");
        return;
    }

    colorPrint("Inserire i codici specie e le relative quantità da inserire nel pacco. Una volta finito inserire 'cancel'\n", GREEN_TEXT);
retry:
    if(!getOrderValues(specie, &quantità)) {
        printError("Operazione annullata. Bisogna inserire almeno una specie nell'ordine.\n");
        return;
    }

    if(!addPackageDB(ordine, specie, quantità, &pacco)) {
        goto retry;
    }

    while(getOrderValues(specie, &quantità)) {
        if(!addPlantToPackageDB(ordine, pacco, specie, quantità)) {
            printError("Errore nell'inserimento della specie nell'ordine.");
        }
    }

    if(!closePackageDB(pacco, ordine)) {
        printError("Errore nella chiusura dell'ordine.");
    }

    colorPrint("Pacco completato!\n", GREEN_TEXT);
}

void selectSalesOrderToSend() {

    DatabaseResult* orders = selectSalesOrderToSendDB();

    if(orders == NULL) return;
    
    printOrderCodes((int **) orders->rowsSet, orders->numRows);
    freeDatabaseResult(orders);
}

void selectPlantToSend() {
    int ordine;

    if(!getWhileIntegerInputView("Codice Ordine", &ordine)) {
        printError("Operazione annullata.");
        return;
    }

    DatabaseResult* plants = selectPlantToSendDB(ordine);

    if(plants == NULL) return;
    
    printCodeAndQuantity((Plant **) plants->rowsSet, plants->numRows);
    freeDatabaseResult(plants);
}

void operatorController() {
    clearScreen();
    operatorViewHeader();
    enum OperatorControllerOptions option;
    do {
        option = getOperatorOption();

        switch (option) {
            case ADD_PACKAGE: 
                addPackage();
                break;
            case SELECT_PLANT_TO_SEND: 
                selectPlantToSend();
                break;
            case SELECT_ORDER_TO_SEND: 
                selectSalesOrderToSend();
                break;
            case SHOW_ORDER: 
                showOrderDetails();
                break;
            case END_OPTION_OPERATOR:
                goto exit;
        }

    } while(true);
exit:
}