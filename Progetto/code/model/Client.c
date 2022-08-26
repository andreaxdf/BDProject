#include "Client.h"
#include <stdio.h>
#include "../utils/IOUtils.h"
#include "../view/viewUtils.h"

bool checkCodiceFiscale(char* codiceFiscale) {
    if(codiceFiscale == NULL) {
        return false;
    }
    if(strlen(codiceFiscale) != 16) {
        return false;
    }
    //TODO aggiungere controlli
    return true;
}

void printClientInformation(Private_client client) {
    colorPrint("\nInformazioni cliente:\n\n", GREEN_TEXT);

    char message[300];
    sprintf(message, "Codice Cliente: %d\nNome: %s\nCognome: %s\nCodice Fiscale: %s\n",
        client.codiceCliente,
        client.nome,
        client.cognome,
        client.codiceFiscale);
    colorPrint(message, GREEN_TEXT);
    printAddressInformation(client.indirizzoResidenza, "Indirizzo Residenza: ");
    printAddressInformation(client.indirizzoFatturazione, "Indirizzo Fatturazione: ");
}

void printCompanyInformation(Company company) {
    colorPrint("\nInformazioni cliente:\n\n", GREEN_TEXT);

    char message[300];
    sprintf(message, "Codice Cliente: %d\nNome: %s\nPartita IVA: %s\n",
        company.codiceCliente,
        company.nome,
        company.partitaIVA);
    colorPrint(message, GREEN_TEXT);
    sprintf(message, "Codice Referente: %d\nNome Referente: %s\nCognome Referente: %s\n", 
        company.referente.codiceContattabile,
        company.referente.nome, 
        company.referente.cognome);
    colorPrint(message, GREEN_TEXT);
    printAddressInformation(company.indirizzoResidenza, "Indirizzo Residenza: ");
    printAddressInformation(company.indirizzoFatturazione, "Indirizzo Fatturazione: ");
}