#include <stdbool.h>
#include "secretaryController.h"
#include "../view/viewUtils.h"
#include "../database/databaseUtils.h"
#include "../database/connector.h"
#include "../view/secretaryView.h"
#include "../model/Client.h"
#include "../model/Contact.h"
#include "../database/secretaryDB.h"
#include "../model/Plant.h"
#include "../model/SalesOrder.h"
#include "../model/OrderRow.h"
#include "../view/choiceView.h"

void printSalesOrderTotal(int ordine){
    double totale;

    if(getTotalSalesOrderDB(ordine, &totale) == 1) {
        printError("Ritenta o cambia operazione da eseguire.\n");
        return;
    }

    char message[50];
    sprintf(message, "TOTALE ORDINE: %.2f €\n", totale);
    colorPrint(message, RED_TEXT);
}

void showSalesOrder() {
    Address deliveryAddress;
    int codiceCliente;
    Date data;
    char contatto[45];
    DatabaseResult* orderRows;
    int codiceOrdine;

    if(!getWhileIntegerInputView("Codice Ordine", &codiceOrdine)) {
        printError("Operazione annullata.\n");
        return;
    }
    int ret;
    if((ret = selectSalesOrderDB(codiceOrdine, &deliveryAddress, &data, contatto, &codiceCliente)) == 1) {
        printError("Ritenta o cambia operazione da eseguire.\n");
        return;
    }

    if(ret == 2) {
        printError("Ordine non trovato.\n");
        return;
    }

    printOrderInformation(codiceOrdine, deliveryAddress, data, contatto, codiceCliente);

    printSalesOrderTotal(codiceOrdine);

    orderRows = selectOrderRowsDB(codiceOrdine);

    if(orderRows == NULL) {
        printError("Ritenta o cambia operazione da eseguire.\n");
        return;
    }
    
    printOrderRowsInformation((OrderRow** ) orderRows->rowsSet, orderRows->numRows);
    
    freeDatabaseResult(orderRows);
    
    return;
}

bool selectAndShowContacts(int codiceContattabile) {
    DatabaseResult* contacts;
    contacts = selectContatsDB(codiceContattabile);
    if(contacts == NULL) {
        printError("Ritenta o cambia operazione da eseguire.\n");
        return false;
    }
    
    printContactInformation((Contact** ) contacts->rowsSet, contacts->numRows);
    freeDatabaseResult(contacts);
    return true;
}

void registerOrder() {
    SalesOrder order;
    char specie[10];
    int quantità;
    Contact preferiteContact;


    if(!getWhileIntegerInputView("Codice cliente", &(order.codiceCliente))) {
        printError("Operazione annullata.\n");
        return;
    }

    int ret;

    if((ret = getPreferiteContactDB(order.codiceCliente, &preferiteContact)) == 1) {
        printError("Ritenta o cambia operazione da eseguire.\n");
        return;
    }

    if(!registerAddressView(&order)) {
        printError("Operazione annullata.\n");
        return;
    }

    char temp[15];
    if(ret == 0) {
        getName(preferiteContact.type, temp);
        char message[100];
        sprintf(message,"Contatto preferito del cliente: %s, %s", temp, preferiteContact.value);
        colorPrint(message, GREEN_TEXT);
        if(choiceContact()){
            strcpy(order.contatto, preferiteContact.value);
            goto salta;
        }
    } else {
        colorPrint("Il cliente non ha un contatto preferito. Scegliere un contatto:\n", GREEN_TEXT);
    }

    if(!selectAndShowContacts(order.codiceCliente)) {
        return;
    }

    if(!getWhileInputView("Contatto", order.contatto, 46)) {
        printError("Operazione annullata.\n");
        return;
    }

salta:

    colorPrint("Inserire i codici specie e le relative quantità da inserire nell'ordine. Una volta finito inserire 'cancel'\n", GREEN_TEXT);
    if(!getOrderValues(specie, &quantità)) {
        printError("Operazione annullata. Bisogna inserire almeno una specie nell'ordine.\n");
        return;
    }

    if(!registerOrderDB(&order, specie, quantità)) {
        goto salta;
    }

    while(getOrderValues(specie, &quantità)) {
        if(!registerPlantInOrderDB(order.codiceOrdine, specie, quantità)) {
            printError("Errore nell'inserimento della specie nell'ordine.");
        }
    }

    if(!closeOrderDB(order.codiceOrdine)) {
        printError("Errore nella chiusura dell'ordine.");
    }

    printSalesOrderTotal(order.codiceOrdine);

    char message[150];
    sprintf(message, "Ordine completato! Il codice ordine è %d.\n", order.codiceOrdine);

    colorPrint(message, GREEN_TEXT);
}

void addContact() {
    Contact contact;
    int codiceContattabile;
    if(getCode("Inserire il codice del cliente/referente a cui aggiungere il contatto:\n",&codiceContattabile)
        & getContactView(&contact)) {
        if(!addContactDB(codiceContattabile, &contact)) {
            printError("Ritenta o cambia operazione da eseguire.\n");
            return;
        }
        colorPrint("\nContatto aggiunto!\n", GREEN_TEXT);
    }
}

void searchPrivateClient() {
    char codiceFiscale[17];
    Private_client client;
    int ret;
    if(searchPrivateClientView(codiceFiscale)) {
        if((ret = searchPrivateClientDB(&client, codiceFiscale)) == 1) {
            printError("Ritenta o cambia operazione da eseguire.\n");
            return;
        } else if(ret == 2) {
            printError("Cliente non trovato. Potrebbe esserci stato un errore oppure il cliente non è ancora registrato.");
            return;
        }
        
        printClientInformation(client);
    }

    
}

void addCompany() {
    Company company;
    Contact contactCompany;
    Contact contactReferent;
    if(addCompanyView(&company, &contactCompany, &contactReferent)) {
        if(!addCompanyDB(&company, &contactCompany, &contactReferent)) {
            printError("Ritenta o cambia operazione da eseguire.\n");
            return;
        }
        colorPrint("\nRivendita inserita!\n", GREEN_TEXT);
    }
}

void addPrivateClient() {
    Private_client client;
    Contact contact;
    if(addPrivateClientView(&client, &contact)) {
        if(!addPrivateClientDB(&client, &contact)) {
            printError("Ritenta o cambia operazione da eseguire.\n");
            return;
        }
        colorPrint("\nCliente inserito!\n", GREEN_TEXT);
    }
    
}

void searchCompany() {
    Company company;
    char partitaIVA[12];
    int ret;
    if(searchCompanyView(partitaIVA)) {
        if((ret = searchCompanyDB(&company, partitaIVA)) == 1) {
            printError("Ritenta o cambia operazione da eseguire.\n");
            return;
        } else if(ret == 2) {
            printError("Rivendita non trovata. Potrebbe esserci stato un errore oppure la rivendita non è ancora registrata.");
            return;
        }
        
        printCompanyInformation(company);
    }
}

void selectAvailablePlant() {
    DatabaseResult* plants = selectAvailablePlantDB();

    if(plants == NULL) return;
    
    printAllPlant((Plant **) plants->rowsSet, plants->numRows);
    freeDatabaseResult(plants);
}

void selectContacts() {
    int codiceContattabile;
    if(getCode("Inserire codice contattabile:\n", &codiceContattabile)) {
        selectAndShowContacts(codiceContattabile);
    }
}

void changePreferiteContact() {
    int codiceContattabile;
    if(!getCode("Inserire codice contattabile:\n", &codiceContattabile)) {
        return;
    }
    colorPrint("\nI suoi contatti sono: \n", GREEN_TEXT);
    if(!selectAndShowContacts(codiceContattabile)) {
        printError("Errore durante la selezione dei contatti");
        return;
    }
    char preferiteContact[46];
    if(!getWhileInputView("\nValore Contatto Preferito", preferiteContact, 46)) {
           printError("Operazione annullata\n");
           return;
    }

    if(!changePreferiteContactDB(codiceContattabile, preferiteContact)) {
        printError("Ritenta o cambia operazione da eseguire.\n");
        return;
    }

    colorPrint("Modifica contatto preferito eseguita!\n", GREEN_TEXT);
 
}

void secretaryController() {
    clearScreen();
    secretaryViewHeader();
    enum SecretaryControllerOptions option;
    do {
        option = getSecretaryOption();

        switch (option) {
            case (REGISTER_ORDER): 
                registerOrder();
                break;
            case (AVAILABLE_PLANT): 
                selectAvailablePlant();
                break;
            case (SEARCH_PRIVATE_CLIENT): 
                searchPrivateClient();
                break;
            case (SEARCH_COMPANY): 
                searchCompany();
                break;
            case (ADD_PRIVATE_CLIENT):
                addPrivateClient();
                break;
            case (ADD_COMPANY): 
                addCompany();
                break;
            case (ADD_CONTACT):
                addContact();
                break;
            case (SELECT_CONTACT): 
                selectContacts();
                break;
            case (CHANGE_PREFERITE_CONTACT): 
                changePreferiteContact();
                break;
            case (SHOW_SALES_ORDER): 
                showSalesOrder();
                break;
            case (END_OPTION):
                goto exit;
        }

    } while(true);
exit:
}