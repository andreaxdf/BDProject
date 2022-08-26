#include "secretaryDB.h"
#include "databaseUtils.h"
#include <stdbool.h>
#include "../model/Client.h"
#include "../model/Contact.h"
#include "../model/Plant.h"
#include "../model/SalesOrder.h"
#include "../model/OrderRow.h"

DatabaseResult* selectOrderRowsDB(int codiceOrdine) {
    MYSQL_STMT *selectOrderRowsProcedure;

    char procedureName[45] = "Selezione righe Ordine di Vendita";
    if (!setupPreparedStatement(&selectOrderRowsProcedure, "CALL mostra_specie_ordine_di_vendita(?) ", conn)) {
        char message[100];
        sprintf(message, "Impossibile Preparare Procedura %s", procedureName);
        printMysqlError(conn, message);
        return NULL;
    }

    MYSQL_BIND param[1];

    bindParam(&param[0], MYSQL_TYPE_LONG, &codiceOrdine, sizeof(int), false);
    
    if(!execute_stmt_and_store_result(selectOrderRowsProcedure, param, procedureName)) {
        return NULL;
    }

    char specie[CODE_MAX_SIZE];
    int  quantità;
    double prezzo;

    MYSQL_BIND returnParam[3];

    bindParam(&returnParam[0], MYSQL_TYPE_VAR_STRING, specie, CODE_MAX_SIZE, false);
    bindParam(&returnParam[1], MYSQL_TYPE_LONG, &quantità, sizeof(int), false);
    bindParam(&returnParam[2], MYSQL_TYPE_DOUBLE, &prezzo, sizeof(double), false);


    if (mysql_stmt_bind_result(selectOrderRowsProcedure, returnParam) != 0) {
        char message[100];
        sprintf(message, "Impossibile Recuperare Risultato %s", procedureName);
        printStatementError(selectOrderRowsProcedure, message) ;
        freeStatement(selectOrderRowsProcedure, false);
        return NULL;
    }

    DatabaseResult *result = myMalloc(sizeof(DatabaseResult));
    result->numRows = mysql_stmt_num_rows(selectOrderRowsProcedure);
    result->rowsSet = myMalloc(sizeof(OrderRow *) * result->numRows);
    

    int hasResult = mysql_stmt_fetch(selectOrderRowsProcedure);
    int i = 0;
    while (hasResult != 1 && hasResult != MYSQL_NO_DATA) {
        OrderRow* orderRow = myMalloc(sizeof(OrderRow));
        strcpy(orderRow->specie, specie);
        orderRow->prezzo = prezzo;
        orderRow->quantità = quantità;

        result->rowsSet[i] = orderRow;

        hasResult = mysql_stmt_fetch(selectOrderRowsProcedure);
        i++;
    }

    if (hasResult == 1) {
        char message[100];
        sprintf(message, "Fetch Risultato Impossibile per %s", procedureName);
        printStatementError(selectOrderRowsProcedure, message);
        freeStatement(selectOrderRowsProcedure, true);
        freeDatabaseResult(result);
        return NULL;
    }
    
    freeStatement(selectOrderRowsProcedure, true);

    return result;
}

int selectSalesOrderDB(int codiceOrdine, Address* address, Date* date, char* contatto, int* codiceCliente) {
    MYSQL_STMT *showSalesOrderProcedure;

    char procedureName[35] = "Mostra Ordine di Vendita";
    if (!setupPreparedStatement(&showSalesOrderProcedure, "CALL mostra_ordine_di_vendita(?) ", conn)) {
        char message[100];
        sprintf(message, "Impossibile Preparare Procedura %s", procedureName);
        printMysqlError(conn, message);
        return 1;
    }

    MYSQL_BIND param[1];

    bindParam(&param[0], MYSQL_TYPE_LONG, &codiceOrdine, sizeof(int), false);
    
    if(!execute_stmt_and_store_result(showSalesOrderProcedure, param, procedureName)) {
        return 1;
    }

    MYSQL_TIME orderDate;

    MYSQL_BIND returnParam[6];

    bindParam(&returnParam[0], MYSQL_TYPE_TIME, &orderDate, sizeof(MYSQL_TIME), false);
    bindParam(&returnParam[1], MYSQL_TYPE_VAR_STRING, contatto, CONTACT_VALUE_MAX_SIZE, false);
    bindParam(&returnParam[2], MYSQL_TYPE_VAR_STRING, address->via, VIA_MAX_SIZE, false);
    bindParam(&returnParam[3], MYSQL_TYPE_VAR_STRING, address->civico, CIVICO_MAX_SIZE, false);
    bindParam(&returnParam[4], MYSQL_TYPE_VAR_STRING, address->citta, CITTA_MAX_SIZE, false);
    bindParam(&returnParam[5], MYSQL_TYPE_LONG, codiceCliente, sizeof(int), false);

    int ret;

    if((ret = bind_and_fetch_one_result(showSalesOrderProcedure, returnParam, procedureName)) == 1) {
        return 1;
    }

    getDateParam(date, &orderDate);

    return ret;
}

DatabaseResult* selectContatsDB(int codiceContattabile) {
    MYSQL_STMT *selectContactsProcedure;

    char procedureName[35] = "Selezione Contatti";
    if (!setupPreparedStatement(&selectContactsProcedure, "CALL seleziona_contatti(?) ", conn)) {
        char message[100];
        sprintf(message, "Impossibile Preparare Procedura %s", procedureName);
        printMysqlError(conn, message);
        return NULL;
    }

    MYSQL_BIND param[1];

    bindParam(&param[0], MYSQL_TYPE_LONG, &codiceContattabile, sizeof(int), false);
    
    if(!execute_stmt_and_store_result(selectContactsProcedure, param, procedureName)) {
        return NULL;
    }

    char valore[CONTACT_VALUE_MAX_SIZE];
    char type[10];

    MYSQL_BIND returnParam[2];

    bindParam(&returnParam[0], MYSQL_TYPE_VAR_STRING, type, 10, false);
    bindParam(&returnParam[1], MYSQL_TYPE_VAR_STRING, valore, CONTACT_VALUE_MAX_SIZE, false);

    if (mysql_stmt_bind_result(selectContactsProcedure, returnParam) != 0) {
        char message[100];
        sprintf(message, "Impossibile Recuperare Risultato %s", procedureName);
        printStatementError(selectContactsProcedure, message) ;
        freeStatement(selectContactsProcedure, false);
        return NULL;
    }

    DatabaseResult *result = myMalloc(sizeof(DatabaseResult));
    result->numRows = mysql_stmt_num_rows(selectContactsProcedure);
    result->rowsSet = myMalloc(sizeof(Contact *) * result->numRows);
    

    int hasResult = mysql_stmt_fetch(selectContactsProcedure);
    int i = 0;
    while (hasResult != 1 && hasResult != MYSQL_NO_DATA) {
        Contact* contact = myMalloc(sizeof(Contact));
        strcpy(contact->value, valore);
        contact->type = getType(type);

        result->rowsSet[i] = contact;

        hasResult = mysql_stmt_fetch(selectContactsProcedure);
        i++;
    }

    if (hasResult == 1) {
        char message[100];
        sprintf(message, "Fetch Risultato Impossibile per %s", procedureName);
        printStatementError(selectContactsProcedure, message);
        freeStatement(selectContactsProcedure, true);
        freeDatabaseResult(result);
        return NULL;
    }
    
    freeStatement(selectContactsProcedure, true);

    return result;
}


DatabaseResult* selectAvailablePlantDB() {
    MYSQL_STMT *selectAvailablePlantProcedure;

    char procedureName[35] = "Selezione piante disponibili";
    if (!setupPreparedStatement(&selectAvailablePlantProcedure, "CALL seleziona_piante_disponibili() ", conn)) {
        char message[100];
        sprintf(message, "Impossibile Preparare Procedura %s", procedureName);
        printMysqlError(conn, message);
        return NULL;
    }

    //Esecuzione della procedura
    if (mysql_stmt_execute(selectAvailablePlantProcedure) != 0) {
        char message[100];
        sprintf(message, "Impossibile Eseguire Procedeura %s", procedureName);
        printStatementError(selectAvailablePlantProcedure, message);
        return NULL;
    }

    if (mysql_stmt_store_result(selectAvailablePlantProcedure) != 0) {
        char message[100];
        sprintf(message, "Store Risultato Impossibile per %s", procedureName);
        printStatementError(selectAvailablePlantProcedure, message);
        freeStatement(selectAvailablePlantProcedure, true);
        return NULL;
    }

    Plant temp;
    MYSQL_BIND returnParam[7];

    bindParam(&returnParam[0], MYSQL_TYPE_VAR_STRING, temp.codiceSpecie, CODE_MAX_SIZE, false);
    bindParam(&returnParam[1], MYSQL_TYPE_VAR_STRING, temp.nomeLatino, NAME_MAX_SIZE, false);
    bindParam(&returnParam[2], MYSQL_TYPE_VAR_STRING, temp.nomeComune, NAME_MAX_SIZE, false);
    bindParam(&returnParam[3], MYSQL_TYPE_LONG, &(temp.giacenza), sizeof(int), false);
    bindParam(&returnParam[4], MYSQL_TYPE_VAR_STRING, &(temp.esotica), 2, false);
    bindParam(&returnParam[5], MYSQL_TYPE_VAR_STRING, &(temp.interno), 2, false);
    bindParam(&returnParam[6], MYSQL_TYPE_DOUBLE, &(temp.prezzo), sizeof(double), false);  

    if (mysql_stmt_bind_result(selectAvailablePlantProcedure, returnParam) != 0) {
        char message[100];
        sprintf(message, "Impossibile Recuperare Risultato %s", procedureName);
        printStatementError(selectAvailablePlantProcedure, message) ;
        freeStatement(selectAvailablePlantProcedure, false);
        return NULL;
    }

    DatabaseResult *result = myMalloc(sizeof(DatabaseResult));
    result->numRows = mysql_stmt_num_rows(selectAvailablePlantProcedure);
    result->rowsSet = myMalloc(sizeof(Plant *) * result->numRows);
    

    int hasResult = mysql_stmt_fetch(selectAvailablePlantProcedure);
    int i = 0;
    while (hasResult != 1 && hasResult != MYSQL_NO_DATA) {
        Plant* plant = myMalloc(sizeof(Plant));
        strcpy(plant->codiceSpecie, temp.codiceSpecie);
        strcpy(plant->nomeLatino, temp.nomeLatino);
        strcpy(plant->nomeComune, temp.nomeComune);
        plant->giacenza = temp.giacenza;
        plant->esotica = temp.esotica;
        plant->interno = temp.interno;
        plant->prezzo = temp.prezzo;

        result->rowsSet[i] = plant;

        hasResult = mysql_stmt_fetch(selectAvailablePlantProcedure);
        i++;
    }

    if (hasResult == 1) {
        char message[100];
        sprintf(message, "Fetch Risultato Impossibile per %s", procedureName);
        printStatementError(selectAvailablePlantProcedure, message);
        freeStatement(selectAvailablePlantProcedure, true);
        freeDatabaseResult(result);
        return NULL;
    }
    
    freeStatement(selectAvailablePlantProcedure, true);

    return result;
}

int searchCompanyDB(Company* company, char* partitaIVA) {
    MYSQL_STMT *searchCompanyProcedure;

    char procedureName[25] = "Cerca Rivendita";
    if (!setupPreparedStatement(&searchCompanyProcedure, "CALL cerca_rivendita(?) ", conn)) {
        char message[100];
        sprintf(message, "Impossibile Preparare Procedura %s", procedureName);
        printMysqlError(conn, message);
        return 1;
    }

    MYSQL_BIND param[1];

    bindParam(param, MYSQL_TYPE_VAR_STRING, partitaIVA, strlen(partitaIVA), false);

    if(!execute_stmt_and_store_result(searchCompanyProcedure, param, procedureName)) {
        return 1;
    }

    char type;
    int indirizzoFatturazione;
    int indirizzoResidenza;
    char cognome[SURNAME_MAX_SIZE];
    char cf[CF_MAX_SIZE];
    MYSQL_BIND returnParam[19];

    bindParam(&returnParam[0], MYSQL_TYPE_LONG, &(company->codiceCliente), sizeof(int), false);
    bindParam(&returnParam[1], MYSQL_TYPE_VAR_STRING, &type, 2, false);
    bindParam(&returnParam[2], MYSQL_TYPE_LONG, &indirizzoFatturazione, sizeof(int), false);
    bindParam(&returnParam[3], MYSQL_TYPE_LONG, &indirizzoResidenza, sizeof(int), false);
    bindParam(&returnParam[4], MYSQL_TYPE_VAR_STRING, company->nome, NAME_MAX_SIZE, false);
    bindParam(&returnParam[5], MYSQL_TYPE_VAR_STRING, cf, CF_MAX_SIZE, false);
    bindParam(&returnParam[6], MYSQL_TYPE_VAR_STRING, cognome, SURNAME_MAX_SIZE, false);
    bindParam(&returnParam[7], MYSQL_TYPE_VAR_STRING, company->partitaIVA, PARTITAIVA_MAX_SIZE, false);
    bindParam(&returnParam[8], MYSQL_TYPE_LONG, &(company->referente.codiceContattabile), sizeof(int), false);
    bindParam(&returnParam[9], MYSQL_TYPE_VAR_STRING, company->referente.nome, NAME_MAX_SIZE, false);
    bindParam(&returnParam[10], MYSQL_TYPE_VAR_STRING, company->referente.cognome, SURNAME_MAX_SIZE, false);
    bindParam(&returnParam[11], MYSQL_TYPE_LONG, &indirizzoResidenza, sizeof(int), false);
    bindParam(&returnParam[12], MYSQL_TYPE_VAR_STRING, company->indirizzoResidenza.via, VIA_MAX_SIZE, false);
    bindParam(&returnParam[13], MYSQL_TYPE_VAR_STRING, company->indirizzoResidenza.civico, CIVICO_MAX_SIZE, false);
    bindParam(&returnParam[14], MYSQL_TYPE_VAR_STRING, company->indirizzoResidenza.citta, CITTA_MAX_SIZE, false);
    bindParam(&returnParam[15], MYSQL_TYPE_LONG, &indirizzoFatturazione, sizeof(int), false);
    bindParam(&returnParam[16], MYSQL_TYPE_VAR_STRING, company->indirizzoFatturazione.via, VIA_MAX_SIZE, false);
    bindParam(&returnParam[17], MYSQL_TYPE_VAR_STRING, company->indirizzoFatturazione.civico, CIVICO_MAX_SIZE, false);
    bindParam(&returnParam[18], MYSQL_TYPE_VAR_STRING, company->indirizzoFatturazione.citta, CITTA_MAX_SIZE, false);
    

    return bind_and_fetch_one_result(searchCompanyProcedure, returnParam, procedureName);
}

int searchPrivateClientDB(Private_client* clientResult, char* codiceFiscale) {
    MYSQL_STMT *searchPrivateClientProcedure;

    char procedureName[25] = "Cerca Cliente Privato";
    if (!setupPreparedStatement(&searchPrivateClientProcedure, "CALL cerca_cliente_privato(?) ", conn)) {
        char message[100];
        sprintf(message, "Impossibile Preparare Procedura %s", procedureName);
        printMysqlError(conn, message);
        return 1;
    }

    MYSQL_BIND param[1];

    bindParam(param, MYSQL_TYPE_VAR_STRING, codiceFiscale, strlen(codiceFiscale), false);

    if(!execute_stmt_and_store_result(searchPrivateClientProcedure, param, procedureName)) {
        return 1;
    }

    char type;
    int indirizzoFatturazione;
    int indirizzoResidenza;
    MYSQL_BIND returnParam[15];

    bindParam(&returnParam[0], MYSQL_TYPE_LONG, &(clientResult->codiceCliente), sizeof(int), false);
    bindParam(&returnParam[1], MYSQL_TYPE_VAR_STRING, &type, 2, false);
    bindParam(&returnParam[2], MYSQL_TYPE_LONG, &indirizzoFatturazione, sizeof(int), false);
    bindParam(&returnParam[3], MYSQL_TYPE_LONG, &indirizzoResidenza, sizeof(int), false);
    bindParam(&returnParam[4], MYSQL_TYPE_VAR_STRING, clientResult->nome, NAME_MAX_SIZE, false);
    bindParam(&returnParam[5], MYSQL_TYPE_VAR_STRING, clientResult->codiceFiscale, CF_MAX_SIZE, false);
    bindParam(&returnParam[6], MYSQL_TYPE_VAR_STRING, clientResult->cognome, SURNAME_MAX_SIZE, false);
    bindParam(&returnParam[7], MYSQL_TYPE_LONG, &indirizzoResidenza, sizeof(int), false);
    bindParam(&returnParam[8], MYSQL_TYPE_VAR_STRING, clientResult->indirizzoResidenza.via, VIA_MAX_SIZE, false);
    bindParam(&returnParam[9], MYSQL_TYPE_VAR_STRING, clientResult->indirizzoResidenza.civico, CIVICO_MAX_SIZE, false);
    bindParam(&returnParam[10], MYSQL_TYPE_VAR_STRING, clientResult->indirizzoResidenza.citta, CITTA_MAX_SIZE, false);
    bindParam(&returnParam[11], MYSQL_TYPE_LONG, &indirizzoFatturazione, sizeof(int), false);
    bindParam(&returnParam[12], MYSQL_TYPE_VAR_STRING, clientResult->indirizzoFatturazione.via, VIA_MAX_SIZE, false);
    bindParam(&returnParam[13], MYSQL_TYPE_VAR_STRING, clientResult->indirizzoFatturazione.civico, CIVICO_MAX_SIZE, false);
    bindParam(&returnParam[14], MYSQL_TYPE_VAR_STRING, clientResult->indirizzoFatturazione.citta, CITTA_MAX_SIZE, false);

    return bind_and_fetch_one_result(searchPrivateClientProcedure, returnParam, procedureName);
}

int getPreferiteContactDB(int codiceCliente, Contact* preferiteContact) {
    MYSQL_STMT *getPreferiteContactProcedure;
    char procedureName[45] = "Seleziona contatto preferito";
    if (!setupPreparedStatement(&getPreferiteContactProcedure, "CALL seleziona_contatto_preferito(?)", conn)) {
        char message[100];
        sprintf(message, "Impossibile Preparare Procedura %s", procedureName);
        printMysqlError(conn, message);
        return 1;
    }

    MYSQL_BIND param[1];

    bindParam(&param[0], MYSQL_TYPE_LONG, &codiceCliente, sizeof(int), false);
    

    if(!execute_stmt(getPreferiteContactProcedure, param, procedureName)) {
        return 1;
    }

    char temp[10];
    MYSQL_BIND returnParam[2];

    bindParam(&returnParam[0], MYSQL_TYPE_VAR_STRING, preferiteContact->value, CONTACT_VALUE_MAX_SIZE, false);
    bindParam(&returnParam[1], MYSQL_TYPE_VAR_STRING, temp, 10, false);

    

    int ret = bind_and_fetch_one_result(getPreferiteContactProcedure, returnParam, procedureName);

    if(ret == 1) return 1;

    if(ret == 2) return 2;

    preferiteContact->type = getType(temp);

    return ret;
}

bool addContactDB(int codiceContattabile, Contact* contact) {
    MYSQL_STMT *addContactProcedure;
    char procedureName[25] = "Aggiungi Contatto";
    if (!setupPreparedStatement(&addContactProcedure, "CALL aggiungi_contatto(?,?,?)", conn)) {
        char message[100];
        sprintf(message, "Impossibile Preparare Procedura %s", procedureName);
        printMysqlError(conn, message);
        return false;
    }

    MYSQL_BIND param[3];
    char type[10];
    getName(contact->type, type);

    bindParam(&param[0], MYSQL_TYPE_LONG, &codiceContattabile, sizeof(int), false);
    bindParam(&param[1], MYSQL_TYPE_VAR_STRING, contact->value, strlen(contact->value), false);
    bindParam(&param[2], MYSQL_TYPE_VAR_STRING, type, strlen(type), false);
    
    if(!execute_stmt(addContactProcedure, param, procedureName)) {
        return false;
    }

    freeStatement(addContactProcedure, false);

    return true;
}

bool addPrivateClientDB(Private_client* client, Contact* contact) {
    //TODO cosa succede se il cliente è già registrato? Errore. Va bene così?
    MYSQL_STMT *addPrivateClientProcedure;
    char procedureName[25] = "Aggiungi Cliente Privato";
    if (!setupPreparedStatement(&addPrivateClientProcedure, "CALL aggiungi_cliente_privato(?,?,?,?,?,?,?,?,?,?,?)", conn)) {
        char message[100];
        sprintf(message, "Impossibile Preparare Procedura %s", procedureName);
        printMysqlError(conn, message);
        return false;
    }

    MYSQL_BIND param[11];
    char type[10];
    getName(contact->type, type);

    bindParam(&param[0], MYSQL_TYPE_VAR_STRING, client->nome, strlen(client->nome), false);
    bindParam(&param[1], MYSQL_TYPE_VAR_STRING, client->codiceFiscale, strlen(client->codiceFiscale), false);
    bindParam(&param[2], MYSQL_TYPE_VAR_STRING, client->cognome, strlen(client->cognome), false);
    bindParam(&param[3], MYSQL_TYPE_VAR_STRING, contact->value, strlen(contact->value), false);
    bindParam(&param[4], MYSQL_TYPE_VAR_STRING, type, strlen(type), false);
    bindParam(&param[5], MYSQL_TYPE_VAR_STRING, client->indirizzoFatturazione.via, strlen(client->indirizzoFatturazione.via), false);
    bindParam(&param[6], MYSQL_TYPE_VAR_STRING, client->indirizzoFatturazione.citta, strlen(client->indirizzoFatturazione.citta), false);
    bindParam(&param[7], MYSQL_TYPE_VAR_STRING, client->indirizzoFatturazione.civico, strlen(client->indirizzoFatturazione.civico), false);
    bindParam(&param[8], MYSQL_TYPE_VAR_STRING, client->indirizzoResidenza.via, strlen(client->indirizzoResidenza.via), false);
    bindParam(&param[9], MYSQL_TYPE_VAR_STRING, client->indirizzoResidenza.citta, strlen(client->indirizzoResidenza.citta), false);
    bindParam(&param[10], MYSQL_TYPE_VAR_STRING, client->indirizzoResidenza.civico, strlen(client->indirizzoResidenza.civico), false);
    
    if(!execute_stmt(addPrivateClientProcedure, param, procedureName)) {
        return false;
    }

    freeStatement(addPrivateClientProcedure, false);

    return true;
}

bool addCompanyDB(Company* company, Contact* contactCompany, Contact* contactReferent) {
    //TODO cosa succede se il cliente è già registrato? Errore. Va bene così?
    MYSQL_STMT *addPrivateClientProcedure;
    char procedureName[25] = "Aggiungi Rivendita";
    if (!setupPreparedStatement(&addPrivateClientProcedure, "CALL aggiungi_rivendita(?,?,?,?,?,?,?,?,?,?,?,?,?,?)", conn)) {
        char message[100];
        sprintf(message, "Impossibile Preparare Procedura %s", procedureName);
        printMysqlError(conn, message);
        return false;
    }

    MYSQL_BIND param[14];
    char typeCompany[10];
    char typeReferent[10];
    getName(contactCompany->type, typeCompany);
    getName(contactReferent->type, typeReferent);

    bindParam(&param[0], MYSQL_TYPE_VAR_STRING, company->nome, strlen(company->nome), false);
    bindParam(&param[1], MYSQL_TYPE_VAR_STRING, company->partitaIVA, strlen(company->partitaIVA), false);
    bindParam(&param[2], MYSQL_TYPE_VAR_STRING, company->referente.nome, strlen(company->referente.nome), false);
    bindParam(&param[3], MYSQL_TYPE_VAR_STRING, company->referente.cognome, strlen(company->referente.cognome), false);
    bindParam(&param[4], MYSQL_TYPE_VAR_STRING, contactCompany->value, strlen(contactCompany->value), false);
    bindParam(&param[5], MYSQL_TYPE_VAR_STRING, typeCompany, strlen(typeCompany), false);
    bindParam(&param[6], MYSQL_TYPE_VAR_STRING, contactReferent->value, strlen(contactReferent->value), false);
    bindParam(&param[7], MYSQL_TYPE_VAR_STRING, typeReferent, strlen(typeReferent), false);
    bindParam(&param[8], MYSQL_TYPE_VAR_STRING, company->indirizzoFatturazione.via, strlen(company->indirizzoFatturazione.via), false);
    bindParam(&param[9], MYSQL_TYPE_VAR_STRING, company->indirizzoFatturazione.citta, strlen(company->indirizzoFatturazione.citta), false);
    bindParam(&param[10], MYSQL_TYPE_VAR_STRING, company->indirizzoFatturazione.civico, strlen(company->indirizzoFatturazione.civico), false);
    bindParam(&param[11], MYSQL_TYPE_VAR_STRING, company->indirizzoResidenza.via, strlen(company->indirizzoResidenza.via), false);
    bindParam(&param[12], MYSQL_TYPE_VAR_STRING, company->indirizzoResidenza.citta, strlen(company->indirizzoResidenza.citta), false);
    bindParam(&param[13], MYSQL_TYPE_VAR_STRING, company->indirizzoResidenza.civico, strlen(company->indirizzoResidenza.civico), false);
    
    if(!execute_stmt(addPrivateClientProcedure, param, procedureName)) {
        return false;
    }

    freeStatement(addPrivateClientProcedure, false);

    return true;
}

bool changePreferiteContactDB(int codiceContattabile, char* preferiteContact) {
    MYSQL_STMT *changePreferiteContactProcedure;
    char procedureName[35] = "Modifica Contatto Preferito";
    if (!setupPreparedStatement(&changePreferiteContactProcedure, "CALL modifica_contatto_preferito(?,?)", conn)) {
        char message[100];
        sprintf(message, "Impossibile Preparare Procedura %s", procedureName);
        printMysqlError(conn, message);
        return false;
    }

    MYSQL_BIND param[2];

    bindParam(&param[0], MYSQL_TYPE_LONG, &codiceContattabile, sizeof(int), false);
    bindParam(&param[1], MYSQL_TYPE_VAR_STRING, preferiteContact, strlen(preferiteContact), false);
    
    if(!execute_stmt(changePreferiteContactProcedure, param, procedureName)) {
        return false;
    }

    freeStatement(changePreferiteContactProcedure, false);

    return true;
}

bool registerOrderDB(SalesOrder* order, char* specie, int quantità) {
    MYSQL_STMT *registerOrderProcedure;
    char procedureName[25] = "Registra Ordine";
    if (!setupPreparedStatement(&registerOrderProcedure, "CALL aggiungi_ordine_di_vendita(?,?,?,?,?,?,?,?)", conn)) {
        char message[100];
        sprintf(message, "Impossibile Preparare Procedura %s", procedureName);
        printMysqlError(conn, message);
        return false;
    }

    MYSQL_BIND param[8];

    bindParam(&param[0], MYSQL_TYPE_VAR_STRING, order->indirizzoSpedizione.via, strlen(order->indirizzoSpedizione.via), false);
    bindParam(&param[1], MYSQL_TYPE_VAR_STRING, order->indirizzoSpedizione.citta, strlen(order->indirizzoSpedizione.citta), false);
    bindParam(&param[2], MYSQL_TYPE_VAR_STRING, order->indirizzoSpedizione.civico, strlen(order->indirizzoSpedizione.civico), false);
    bindParam(&param[3], MYSQL_TYPE_LONG, &(order->codiceCliente), sizeof(int), false);
    bindParam(&param[4], MYSQL_TYPE_VAR_STRING, order->contatto, strlen(order->contatto), false);
    bindParam(&param[5], MYSQL_TYPE_VAR_STRING, specie, strlen(specie), false);
    bindParam(&param[6], MYSQL_TYPE_LONG, &quantità, sizeof(int), false);
    bindParam(&param[7], MYSQL_TYPE_VAR_STRING, &(order->codiceOrdine), sizeof(int), false);
    
    if(!execute_stmt(registerOrderProcedure, param, procedureName)) {
        mysql_stmt_free_result(registerOrderProcedure);
        mysql_stmt_reset(registerOrderProcedure);
        return false;
    }

    //Set Parametro di OUT
    bindParam(&param[7], MYSQL_TYPE_LONG, &(order->codiceOrdine), sizeof(int), false);

    //Bind del parametro di OUT
    if (mysql_stmt_bind_result(registerOrderProcedure, &param[7]) != 0) {
        char message[100];
        sprintf(message, "Impossibile Fare il Bind del risultato di %s", procedureName);
        printStatementError(registerOrderProcedure, message);
        freeStatement(registerOrderProcedure, true);
        return false;
    }

    //Presa del parametro di OUT
    if (mysql_stmt_fetch(registerOrderProcedure) != 0) {
        printStatementError(registerOrderProcedure, "Impossibile recuperare il codice ordine\n") ;
        return false;
    }

    // Consume the possibly-returned table for the output parameter
	while(mysql_stmt_fetch(registerOrderProcedure) == 0) {}

    freeStatement(registerOrderProcedure, true);

    return true;
}

bool registerPlantInOrderDB(int codiceOrdine, char* specie, int quantità) {
    MYSQL_STMT *registerPlantInOrderProcedure;
    char procedureName[35] = "Aggiungi pianta a ordine";
    if (!setupPreparedStatement(&registerPlantInOrderProcedure, "CALL aggiungi_pianta_a_ordine(?,?,?)", conn)) {
        char message[100];
        sprintf(message, "Impossibile Preparare Procedura %s", procedureName);
        printMysqlError(conn, message);
        return false;
    }

    MYSQL_BIND param[3];

    bindParam(&param[0], MYSQL_TYPE_LONG, &codiceOrdine, sizeof(int), false);
    bindParam(&param[1], MYSQL_TYPE_VAR_STRING, specie, strlen(specie), false);
    bindParam(&param[2], MYSQL_TYPE_LONG, &quantità, sizeof(int), false);

    if(!execute_stmt(registerPlantInOrderProcedure, param, procedureName)) {
        mysql_stmt_free_result(registerPlantInOrderProcedure);
        mysql_stmt_reset(registerPlantInOrderProcedure);
        return false;
    }

    freeStatement(registerPlantInOrderProcedure, false);

    return true;
}

bool closeOrderDB(int codiceOrdine) {
    MYSQL_STMT *closeOrderProcedure;
    char procedureName[35] = "Chiusura ordine";
    if (!setupPreparedStatement(&closeOrderProcedure, "CALL chiudi_ordine(?)", conn)) {
        char message[100];
        sprintf(message, "Impossibile Preparare Procedura %s", procedureName);
        printMysqlError(conn, message);
        return false;
    }

    MYSQL_BIND param[1];

    bindParam(&param[0], MYSQL_TYPE_LONG, &codiceOrdine, sizeof(int), false);
   
    if(!execute_stmt(closeOrderProcedure, param, procedureName)) {
        return false;
    }

    freeStatement(closeOrderProcedure, false);

    return true;
}

int getTotalSalesOrderDB(int ordine, double* totale) {
    MYSQL_STMT *stmt;

    char procedureName[25] = "Calcola totale";
    if (!setupPreparedStatement(&stmt, "CALL totale_ordine_di_vendita(?) ", conn)) {
        char message[100];
        sprintf(message, "Impossibile Preparare Procedura %s", procedureName);
        printMysqlError(conn, message);
        return 1;
    }

    MYSQL_BIND param[1];

    bindParam(param, MYSQL_TYPE_LONG, &ordine, sizeof(int), false);

    if(!execute_stmt_and_store_result(stmt, param, procedureName)) {
        return 1;
    }

    double temp;
    MYSQL_BIND returnParam[1];

    bindParam(&returnParam[0], MYSQL_TYPE_DOUBLE, &temp, sizeof(int), false);

    int ret = bind_and_fetch_one_result(stmt, returnParam, procedureName);

    if(ret == 1) return 1;

    *totale = temp;

    return 0;
}