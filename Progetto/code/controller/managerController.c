#include "managerController.h"
#include <stdbool.h>
#include "../view/managerView.h"
#include "../model/User.h"
#include "../view/viewUtils.h"
#include "../database/databaseUtils.h"
#include "../database/connector.h"
#include "../database/managerDB.h"
#include "departmentController.h"
#include "../model/Plant.h"

void showPlants() {
    selectStock();
}


void addPlant() {
    Plant plant;

    if(getPlantInformationView(&plant)) {
        if(!addPlantDB(&plant)) {
            printError("Ritenta o cambia operazione da eseguire.\n");
            return;
        }

        colorPrint("Specie inserita!\n", GREEN_TEXT);
        return;
    }
}

void addColorToPlant() {
    char codiceSpecie[11];
    char colore[30];

    if(getColorView(codiceSpecie, colore)) {
        if(!addColorToPlantDB(codiceSpecie, colore)) {
            printError("Ritenta o cambia operazione da eseguire.\n");
            return;
        }

        colorPrint("Colorazione aggiunta!\n", GREEN_TEXT);
        return;
    }
}

void changePriceToAPlant() {
    char codiceSpecie[11];
    double price;

    if(getPriceView(codiceSpecie, &price)) {
        if(!changePriceDB(codiceSpecie, price)) {
            printError("Ritenta o cambia operazione da eseguire.\n");
            return;
        }

        colorPrint("Prezzo modificato!\n", GREEN_TEXT);
        return;
    }
}

void selectPlantPrices() {
    DatabaseResult* prices;
    char codiceSpecie[CODE_MAX_SIZE];

    if(!getWhileInputView("Codice Specie", codiceSpecie, CODE_MAX_SIZE)) {
        printError("Operazione annullata.");
        return;
    }

    prices = selectPlantPricesDB(codiceSpecie);

    if(prices == NULL) return;

    printAllPrices((Price** ) prices->rowsSet, prices->numRows);

    return;
}

void addUser() {
    User user;
    Role role;

    if(getUserInformationView(&user, &role)) {
        if(!registerUserDB(&user, &role)) {
            printError("Ritenta o cambia operazione da eseguire.\n");
            return;
        }

        colorPrint("Utente inserito!\n", GREEN_TEXT);
        return;
    }
}

void managerController() {
    clearScreen();
    managerViewHeader();
    enum ManagerControllerOptions option;
    do {
        option = getManagerOption();

        switch (option) {
            case SHOW_PLANTS: 
                showPlants();
                break;
            case ADD_PLANT: 
                addPlant();
                break;
            case ADD_COLOR: 
                addColorToPlant();
                break;
            case CHANGE_PRICE: 
                changePriceToAPlant();
                break;
            case SELECT_PRICES: 
                selectPlantPrices();
                break;
            case ADD_USER:
                addUser();
                break;
            case END_OPTION_MANAGER:
                goto exit;
        }

    } while(true);
exit:
}