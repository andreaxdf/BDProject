#include "secretaryView.h"
#include "../controller/secretaryController.h"
#include "viewUtils.h"
#include "../model/Client.h"
#include "../model/Contact.h"
#include "choiceView.h"
#include "../utils/IOUtils.h"
#include "../model/Plant.h"
#include "../model/SalesOrder.h"

//Lista comandi da mostrare per i client
const char *secretaryMenuOptions[] = {
    "Registra un ordine",
    "Mostra delle piante disponibili",
    "Cerca cliente privato",
    "Cerca rivendita",
    "Aggiungi nuovo cliente privato",
    "Aggiungi nuova rivendita",
    "Aggiungi contatto",
    "Mostra contatti di un cliente/referente",
    "Modifica contatto preferito di un cliente/referente",
    "Mostra dettagli ordine di vendita",
    "Esci"
    };
const int secretaryMenuLenght = 11;

void secretaryViewHeader() {
    colorPrint("******************************************\n", GREEN_TEXT);
	colorPrint("*               Segreteria               *\n", GREEN_TEXT);
	colorPrint("******************************************\n\n", GREEN_TEXT);
}

int getSecretaryOption() {
    return getUserOption(secretaryMenuOptions, secretaryMenuLenght);
}


/*
    Funzione che chiede all'utente le informazioni per inizializzare 
    un cliente e un contatto per l'aggiunta di un cliente privato al 
    database.
    - client: indirizzo della variabile Private_client da inizilizzare.
    - contact: indirizzo della variabile Contact da inizilizzare.
*/
bool addPrivateClientView(Private_client* client, Contact* contact) {
    colorPrint("\nInserire informazioni cliente\n\n", GREEN_TEXT);

    if(!getWhileInputView("Nome", client->nome, NAME_MAX_SIZE)) {
        goto exit;
    }

    if(!getWhileInputView("Cognome", client->cognome, SURNAME_MAX_SIZE)) {
        goto exit;
    }

codiceFiscale:
    if(!getWhileInputView("Codice Fiscale", client->codiceFiscale, CF_MAX_SIZE)) {
        goto exit;
    } else if(!checkCodiceFiscale(client->codiceFiscale)) {
        printError("Il codice fiscale non è valido");
        goto codiceFiscale;
    }
    
    colorPrint("\nInserire indirizzo di residenza:\n", GREEN_TEXT);
    if(!getAddressView(&(client->indirizzoResidenza))) {
        goto exit;
    }
    
    if(choiceFattAddress()) {
        colorPrint("\nInserire indirizzo di fatturazione:\n", GREEN_TEXT);
        if(!getAddressView(&(client->indirizzoFatturazione))) {
            goto exit;
        }
    } else {
        strcpy(client->indirizzoFatturazione.citta, client->indirizzoResidenza.citta);
        strcpy(client->indirizzoFatturazione.via, client->indirizzoResidenza.via);
        strcpy(client->indirizzoFatturazione.civico, client->indirizzoResidenza.civico);
    }
  
    if(!getContactView(contact)) {
        goto exit;
    }

    return true;
exit:
    printError("Procedura per l'inserimento del cliente annullata\n");
    return false;
}

bool addCompanyView(Company* company, Contact* contactCompany, Contact* contactReferent) {
    colorPrint("\nInserire informazioni cliente\n\n", GREEN_TEXT);

    if(!getWhileInputView("Nome Rivendita", company->nome, NAME_MAX_SIZE)) {
        goto exit;
    }

    if(!getWhileInputView("Partita IVA", company->partitaIVA, SURNAME_MAX_SIZE)) {
        goto exit;
    }
    
    colorPrint("\nInserire indirizzo di residenza:\n", GREEN_TEXT);
    if(!getAddressView(&(company->indirizzoResidenza))) {
        goto exit;
    }
    
    if(choiceFattAddress()) {
        colorPrint("\nInserire indirizzo di fatturazione:\n", GREEN_TEXT);
        if(!getAddressView(&(company->indirizzoFatturazione))) {
            goto exit;
        }
    } else {
        strcpy(company->indirizzoFatturazione.citta, company->indirizzoResidenza.citta);
        strcpy(company->indirizzoFatturazione.via, company->indirizzoResidenza.via);
        strcpy(company->indirizzoFatturazione.civico, company->indirizzoResidenza.civico);
    }
  
    if(!getContactView(contactCompany)) {
        goto exit;
    }

    if(!getReferentView(&(company->referente))) {
        goto exit;
    }

    if(!getContactView(contactReferent)) {
        goto exit;
    }

    return true;
exit:
    printError("Procedura per l'inserimento della rivendita annullata\n");
    return false;
}


/*
    Funzione che richiede il codice fiscale in input e 
    lo salva nel resultBuffer passato come argomento.
*/
bool searchPrivateClientView(char* codiceFiscale) {
    colorPrint("Inserire il codice fiscale del cliente da cercare:\n", GREEN_TEXT);
    if(!getWhileInputView("Codice Fiscale", codiceFiscale, CF_MAX_SIZE)) {
        return false;
    }

    return true;
}

bool searchCompanyView(char* partitaIVA) {
    colorPrint("Inserire la partita IVA della rivendita da cercare:\n", GREEN_TEXT);
    if(!getWhileInputView("Partita IVA", partitaIVA, PARTITAIVA_MAX_SIZE)) {
        return false;
    }
    return true;
}

void printAllPlant(Plant** plants, int numPlants) {
    printf("\n|  Codice  |     Nome Latino     |     Nome Comune     |  Esotica  |  Interno/Esterno  |  Giacenza  |  Prezzo  |\n");
    printf("‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾\n");

    for(int i = 0; i < numPlants; i++) {
        char temp[10];
        
        printValue(plants[i]->codiceSpecie, 10);
        printValue(plants[i]->nomeLatino, 21);
        printValue(plants[i]->nomeComune, 21);
        if(plants[i]->esotica == 'e') {
            printValue("Esotica", 11);
        } else {
            printValue("Non Esotica", 11);
        }
        if(plants[i]->interno == 'i') {
            printValue("Da interno", 19);
        } else {
            printValue("Da esterno", 19);
        }
        sprintf(temp, "%d", plants[i]->giacenza);
        printValue(temp, 12);
        sprintf(temp, "%.2f", plants[i]->prezzo);
        printValue(temp, 10);
        colorPrint("|\n", GREEN_TEXT);
    }
}

void printContactInformation(Contact** contacts, int numContacts) {
    printf("\n|  Tipo Contatto  |            Valore            |\n");
    printf("‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾\n");

    for(int i = 0; i < numContacts; i++) {
        char temp[10];
        getName(contacts[i]->type, temp);
        printValue(temp, 17);
        printValue(contacts[i]->value, 30);
        colorPrint("|\n", GREEN_TEXT);
    }
}

bool registerAddressView(SalesOrder* order) {
    colorPrint("Inserire l'indirizzo di spedizione:\n", GREEN_TEXT);
    if(!getAddressView(&(order->indirizzoSpedizione))) {
        printError("Operazione annullata.");
        return false;
    }
    
    return true;
}

bool getOrderValues(char* specie, int* quantità) {
    if(!getWhileInputView("Codice Specie", specie, CODE_MAX_SIZE)) {
        return false;
    }

    if(!getWhileIntegerInputView("Quantità", quantità)) {
        return false;
    }

    return true;
}

void printOrderInformation(int codiceOrdine, Address address, Date date, char* contatto, int codiceCliente) {
    char message[200];

    sprintf(message, "Codice Ordine: %d\n", codiceOrdine);
    colorPrint(message, GREEN_TEXT);

    sprintf(message, "Codice Cliente: %d\n", codiceCliente);
    colorPrint(message, GREEN_TEXT);

    sprintf(message, "Indirizzo di Spedizione: %s %s, %s\n",
        address.via,
        address.civico,
        address.citta);
    colorPrint(message, GREEN_TEXT);

    sprintf(message, "Contatto: %s\n", contatto);
    colorPrint(message, GREEN_TEXT);

    sprintf(message, "Data Ordine: %d-%d-%d\n", date.day, date.month, date.year);
    colorPrint(message, GREEN_TEXT);

}

void printOrderRowsInformation(OrderRow** rows, int numRows) {
    printf("\n|  Codice Specie  |  Quantità  |  Prezzo  |\n");
    printf("‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾\n");

    for(int i = 0; i < numRows; i++) {
        char temp[10];
        printValue(rows[i]->specie, 17);
        sprintf(temp, "%d", rows[i]->quantità);
        printValue(temp, 12);
        sprintf(temp, "%.2f", rows[i]->prezzo);
        printValue(temp, 10);
        colorPrint("|\n", GREEN_TEXT);
    }
}