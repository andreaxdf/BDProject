#include "departmentController.h"
#include "../view/departmentView.h"
#include <stdbool.h>
#include "../view/viewUtils.h"
#include "../database/departmentDB.h"
#include "../database/databaseUtils.h"
#include "../model/Plant.h"
#include "../model/Supplier.h"
#include "../model/PurchaseOrder.h"

void printTotalOrder(int ordine) {
    double totale;

    if(getTotalPurchaseOrderDB(ordine, &totale) == 1) {
        printError("Ritenta o cambia operazione da eseguire.\n");
        return;
    }
    
    char message[50];
    sprintf(message, "TOTALE ORDINE: %.2f €\n", totale);
    colorPrint(message, RED_TEXT);
}

void showPlantInOrder() {

    int ordine;
    if(!getWhileIntegerInputView("Codice Ordine", &ordine)) {
        printError("Operazione annullata.\n");
        return;
    }

    DatabaseResult* plants = selectPlantInOrderDB(ordine);

    if(plants == NULL) return;

    printAllPlantName((Plant** ) plants->rowsSet, plants->numRows);

    freeDatabaseResult(plants);

    printTotalOrder(ordine);

    return;
}

void showOpenedOrder() {
    DatabaseResult* orders = selectOpenedOrderDB();

    if(orders == NULL) return;

    printAllPurchaseOrder((PurchaseOrder** ) orders->rowsSet, orders->numRows);

    freeDatabaseResult(orders);

    return;
}

void searchSupplier() {
    
    char codiceSpecie[CODE_MAX_SIZE];
    if(!getWhileInputView("Codice Specie da ordinare",codiceSpecie, CODE_MAX_SIZE)) 
        return;

    DatabaseResult* suppliers = searchSupplierDB(codiceSpecie);

    if(suppliers == NULL) return;
        
    printSupplierInformation((Supplier** ) suppliers->rowsSet, suppliers->numRows);

    freeDatabaseResult(suppliers);

    return;
}


void selectStock() {
    DatabaseResult* plants= selectStocksDB();

    if(plants == NULL) return;

    printAllStocks((Plant** ) plants->rowsSet, plants->numRows);

    freeDatabaseResult(plants);

    return;
}

void closeOrderAndAddStock() {
    int codiceOrdine;

    if(getWhileIntegerInputView("Codice Ordine", &codiceOrdine)) {
        if(!closeOrderAndAddStockDB(codiceOrdine)) {
            printError("Ritenta o cambia operazione da eseguire.\n");
            return;
        }

        colorPrint("Le giacenze aggiornate sono:\n", GREEN_TEXT);

        selectStock();
        return;
    }
}

void addPurchaseOrder() {
    int codiceFornitore;
    char specie[10];
    int quantità;
    double prezzo;
    int codiceOrdine;

    if(!getWhileIntegerInputView("Codice Fornitore", &codiceFornitore)) {
        printError("Operazione annullata.\n");
        return;
    }

    colorPrint("Inserire il codice, la quantità e il prezzo per ogni specie da inserire nell'ordine. Una volta finito inserire 'cancel'\n", GREEN_TEXT);
    if(!getPurchaseOrderValues(specie, &quantità, &prezzo)) {
        printError("Operazione annullata. Bisogna inserire almeno una specie nell'ordine.\n");
        return;
    }

    if(!registerPurchaseOrderDB(codiceFornitore, specie, quantità, prezzo, &codiceOrdine)) {
        printError("Ritenta o cambia operazione da eseguire.\n");
        return;
    }

    while(getPurchaseOrderValues(specie, &quantità, &prezzo)) {
        if(!registerPlantInPurchaseOrderDB(codiceOrdine, specie, quantità, prezzo)) {
            printError("Errore nell'inserimento della specie nell'ordine.");
        }
    }

    printTotalOrder(codiceOrdine);

    char message[150];
    sprintf(message, "Ordine completato! Il codice ordine è %d.\n", codiceOrdine);

    colorPrint(message, GREEN_TEXT);
}

void addSupplier() {
    Supplier supplier;
    Address address;
    char codiceSpecie[CODE_MAX_SIZE];

    if(!getSupplierView(&supplier)) {
        printError("Operazione annullata.\n");
        return;
    }

    colorPrint("Inserire un indirizzo:  \n", GREEN_TEXT);
    if(!getAddressView(&address)) {
        printError("Operazione annullata.\n");
        return;
    }

    colorPrint("Inserire il codice delle specie di cui dispone il fornitore\n", GREEN_TEXT);

    if(!getWhileInputView("Codice Specie", codiceSpecie, CODE_MAX_SIZE)) {
        printError("Operazione annullata.\n");
        return;
    }

    if(!addSupplierDB(&supplier, &address, codiceSpecie)) {
        printError("Ritenta o cambia operazione da eseguire.\n");
        return;
    }

    while(getWhileInputView("Codice Specie", codiceSpecie, CODE_MAX_SIZE)) {
        if(!addPlantToSupplierDB(supplier.codiceFornitore, codiceSpecie)) {
            printError("Errore nell'inserimento della specie nell'ordine.");
        }
    }

    char message[100];
    sprintf(message, "\nFornitore inserito!");
    colorPrint(message, GREEN_TEXT);
}

void addSupplierAddress() {
    int codiceFornitore;
    Address address;

    if(!getWhileIntegerInputView("Codice Fornitore", &codiceFornitore)) {
        printError("Operazione annullata.\n");
        return;
    }

    colorPrint("Inserire un indirizzo:  \n", GREEN_TEXT);
    if(!getAddressView(&address)) {
        printError("Operazione annullata.\n");
        return;
    }

    if(!addAddressToSupplierDB(codiceFornitore, &address)) {
        printError("Ritenta o cambia operazione da eseguire.\n");
        return;
    }
    colorPrint("Indirizzo aggiunto!\n", GREEN_TEXT);
}

void departmentController() {
    clearScreen();
    departmentViewHeader();
    enum DepartmentControllerOptions option;
    do {
        option = getDepartmentOption();

        switch (option) {
            case ADD_STOCK: 
                closeOrderAndAddStock();
                break;
            case SELECT_STOCK: 
                selectStock();
                break;
            case ADD_PURCHASE_ORDER: 
                addPurchaseOrder();
                break;
            case SHOW_PURCHASE_ORDER_OPEN: 
                showOpenedOrder();
                break;
            case SHOW_PLANT_IN_ORDER: 
                showPlantInOrder();
                break;
            case ADD_SUPPLIER:
                addSupplier();
                break;
            case ADD_SUPPLIER_ADDRESS: 
                addSupplierAddress();
                break;
            case SEARCH_SUPPLIER: 
                searchSupplier();
                break;
            
            case END_OPTION_DEPARTMENT:
                goto exit;
        }

    } while(true);
exit:
}