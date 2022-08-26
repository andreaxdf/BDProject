#include "departmentDB.h"
#include "../view/viewUtils.h"
#include "connector.h"
#include "../model/Plant.h"
#include "../model/PurchaseOrder.h"

DatabaseResult* selectPlantInOrderDB(int ordine) {
    MYSQL_STMT *stmt;

    char procedureName[35] = "Selezione Specie in un Ordine";
    if (!setupPreparedStatement(&stmt, "CALL mostra_ordine_di_acquisto(?) ", conn)) {
        char message[100];
        sprintf(message, "Impossibile Preparare Procedura %s", procedureName);
        printMysqlError(conn, message);
        return NULL;
    }

    MYSQL_BIND param[1];

    bindParam(&param[0], MYSQL_TYPE_LONG, &ordine, sizeof(int), false);

    if(!execute_stmt(stmt, param, procedureName)) {
        freeStatement(stmt, true);
        return false;
    }

    if (mysql_stmt_store_result(stmt) != 0) {
        char message[100];
        sprintf(message, "Store Risultato Impossibile per %s", procedureName);
        printStatementError(stmt, message);
        freeStatement(stmt, true);
        return NULL;
    }

    char codiceSpecie[CODE_MAX_SIZE];
    char nomeLatino[NAME_MAX_SIZE];
    char nomeComune[NAME_MAX_SIZE];
    int quantità;
    double prezzo;


    MYSQL_BIND returnParam[5];


    bindParam(&returnParam[0], MYSQL_TYPE_VAR_STRING, codiceSpecie, CODE_MAX_SIZE, false);
    bindParam(&returnParam[1], MYSQL_TYPE_VAR_STRING, nomeLatino, NAME_MAX_SIZE, false);
    bindParam(&returnParam[2], MYSQL_TYPE_VAR_STRING, nomeComune, NAME_MAX_SIZE, false);
    bindParam(&returnParam[3], MYSQL_TYPE_LONG, &quantità, sizeof(int), false);
    bindParam(&returnParam[4], MYSQL_TYPE_DOUBLE, &prezzo, sizeof(double), false);


    if (mysql_stmt_bind_result(stmt, returnParam) != 0) {
        char message[100];
        sprintf(message, "Impossibile Recuperare Risultato %s", procedureName);
        printStatementError(stmt, message) ;
        freeStatement(stmt, false);
        return NULL;
    }

    DatabaseResult *result = myMalloc(sizeof(DatabaseResult));
    result->numRows = mysql_stmt_num_rows(stmt);
    result->rowsSet = myMalloc(sizeof(Plant *) * result->numRows);
    

    int hasResult = mysql_stmt_fetch(stmt);
    int i = 0;
    while (hasResult != 1 && hasResult != MYSQL_NO_DATA) {
        Plant* plant = myMalloc(sizeof(Plant));
        
        strcpy(plant->codiceSpecie, codiceSpecie); 
        strcpy(plant->nomeLatino, nomeLatino); 
        strcpy(plant->nomeComune, nomeComune);
        plant->giacenza = quantità;
        plant->prezzo = prezzo;

        result->rowsSet[i] = plant;

        hasResult = mysql_stmt_fetch(stmt);
        i++;
    }

    if (hasResult == 1) {
        char message[100];
        sprintf(message, "Fetch Risultato Impossibile per %s", procedureName);
        printStatementError(stmt, message);
        freeStatement(stmt, true);
        freeDatabaseResult(result);
        return NULL;
    }
    
    freeStatement(stmt, true);

    return result;
}

DatabaseResult* selectOpenedOrderDB() {
    MYSQL_STMT *stmt;

    char procedureName[35] = "Selezione Ordini Ancora Aperti";
    if (!setupPreparedStatement(&stmt, "CALL seleziona_ordini_di_acquisto_aperti() ", conn)) {
        char message[100];
        sprintf(message, "Impossibile Preparare Procedura %s", procedureName);
        printMysqlError(conn, message);
        return NULL;
    }

    //Esecuzione della procedura
    if (mysql_stmt_execute(stmt) != 0) {
        char message[100];
        sprintf(message, "Impossibile Eseguire Procedeura %s", procedureName);
        printStatementError(stmt, message);
        freeStatement(stmt, true);
        return NULL;
    }

    if (mysql_stmt_store_result(stmt) != 0) {
        char message[100];
        sprintf(message, "Store Risultato Impossibile per %s", procedureName);
        printStatementError(stmt, message);
        freeStatement(stmt, true);
        return NULL;
    }

    int codiceOrdine;
    MYSQL_TIME orderDate;
    int codiceFornitore;
    char codiceFiscale[CF_MAX_SIZE];
    char nomeFornitore[NAME_MAX_SIZE];


    MYSQL_BIND returnParam[5];


    bindParam(&returnParam[0], MYSQL_TYPE_LONG, &codiceOrdine, sizeof(int), false);
    bindParam(&returnParam[1], MYSQL_TYPE_TIME, &orderDate, sizeof(MYSQL_TIME), false);
    bindParam(&returnParam[2], MYSQL_TYPE_LONG, &codiceFornitore, sizeof(int), false);
    bindParam(&returnParam[3], MYSQL_TYPE_VAR_STRING, codiceFiscale, CF_MAX_SIZE, false);
    bindParam(&returnParam[4], MYSQL_TYPE_VAR_STRING, nomeFornitore, NAME_MAX_SIZE, false);


    if (mysql_stmt_bind_result(stmt, returnParam) != 0) {
        char message[100];
        sprintf(message, "Impossibile Recuperare Risultato %s", procedureName);
        printStatementError(stmt, message) ;
        freeStatement(stmt, false);
        return NULL;
    }

    DatabaseResult *result = myMalloc(sizeof(DatabaseResult));
    result->numRows = mysql_stmt_num_rows(stmt);
    result->rowsSet = myMalloc(sizeof(PurchaseOrder *) * result->numRows);
    

    int hasResult = mysql_stmt_fetch(stmt);
    int i = 0;
    while (hasResult != 1 && hasResult != MYSQL_NO_DATA) {
        PurchaseOrder* order = myMalloc(sizeof(PurchaseOrder));
        order->codiceOrdine = codiceOrdine;
        getDateParam(&(order->data), &orderDate);
        strcpy(order->codiceFiscale, codiceFiscale);
        strcpy(order->nome, nomeFornitore);
        order->codiceFornitore = codiceFornitore;

        result->rowsSet[i] = order;

        hasResult = mysql_stmt_fetch(stmt);
        i++;
    }

    if (hasResult == 1) {
        char message[100];
        sprintf(message, "Fetch Risultato Impossibile per %s", procedureName);
        printStatementError(stmt, message);
        freeStatement(stmt, true);
        freeDatabaseResult(result);
        return NULL;
    }
    
    freeStatement(stmt, true);

    return result;
}

DatabaseResult* searchSupplierDB(char* codiceSpecie) {
    MYSQL_STMT *stmt;

    char procedureName[35] = "Cerca Fornitori per Specie";
    if (!setupPreparedStatement(&stmt, "CALL carica_fornitore_per_specie(?) ", conn)) {
        char message[100];
        sprintf(message, "Impossibile Preparare Procedura %s", procedureName);
        printMysqlError(conn, message);
        return NULL;
    }

    MYSQL_BIND param[1];

    bindParam(&param[0], MYSQL_TYPE_VAR_STRING, codiceSpecie, strlen(codiceSpecie), false);

  
    if(!execute_stmt(stmt, param, procedureName)) {
        freeStatement(stmt, true);
        return false;
    }

    if (mysql_stmt_store_result(stmt) != 0) {
        char message[100];
        sprintf(message, "Store Risultato Impossibile per %s", procedureName);
        printStatementError(stmt, message);
        freeStatement(stmt, true);
        return NULL;
    }

    int codiceFornitore;
    char codiceFiscale[CF_MAX_SIZE];
    char nomeFornitore[NAME_MAX_SIZE];


    MYSQL_BIND returnParam[3];

    bindParam(&returnParam[0], MYSQL_TYPE_LONG, &codiceFornitore, sizeof(int), false);
    bindParam(&returnParam[1], MYSQL_TYPE_VAR_STRING, codiceFiscale, CF_MAX_SIZE, false);
    bindParam(&returnParam[2], MYSQL_TYPE_VAR_STRING, nomeFornitore, NAME_MAX_SIZE, false);


    if (mysql_stmt_bind_result(stmt, returnParam) != 0) {
        char message[100];
        sprintf(message, "Impossibile Recuperare Risultato %s", procedureName);
        printStatementError(stmt, message) ;
        freeStatement(stmt, false);
        return NULL;
    }

    DatabaseResult *result = myMalloc(sizeof(DatabaseResult));
    result->numRows = mysql_stmt_num_rows(stmt);
    result->rowsSet = myMalloc(sizeof(Supplier *) * result->numRows);
    

    int hasResult = mysql_stmt_fetch(stmt);
    int i = 0;
    while (hasResult != 1 && hasResult != MYSQL_NO_DATA) {
        Supplier* supplier = myMalloc(sizeof(Supplier));
        strcpy(supplier->codiceFiscale, codiceFiscale);
        strcpy(supplier->nome, nomeFornitore);
        supplier->codiceFornitore = codiceFornitore;

        result->rowsSet[i] = supplier;

        hasResult = mysql_stmt_fetch(stmt);
        i++;
    }

    if (hasResult == 1) {
        char message[100];
        sprintf(message, "Fetch Risultato Impossibile per %s", procedureName);
        printStatementError(stmt, message);
        freeStatement(stmt, true);
        freeDatabaseResult(result);
        return NULL;
    }
    
    freeStatement(stmt, true);

    return result;
}

DatabaseResult* selectStocksDB() {
    MYSQL_STMT *selectStocksProcedure;

    char procedureName[35] = "Selezione Giacenze";
    if (!setupPreparedStatement(&selectStocksProcedure, "CALL selezione_giacenza_specie() ", conn)) {
        char message[100];
        sprintf(message, "Impossibile Preparare Procedura %s", procedureName);
        printMysqlError(conn, message);
        return NULL;
    }

    //Esecuzione della procedura
    if (mysql_stmt_execute(selectStocksProcedure) != 0) {
        char message[100];
        sprintf(message, "Impossibile Eseguire Procedeura %s", procedureName);
        printStatementError(selectStocksProcedure, message);
        freeStatement(selectStocksProcedure, true);
        return NULL;
    }

    if (mysql_stmt_store_result(selectStocksProcedure) != 0) {
        char message[100];
        sprintf(message, "Store Risultato Impossibile per %s", procedureName);
        printStatementError(selectStocksProcedure, message);
        freeStatement(selectStocksProcedure, true);
        return NULL;
    }

    char codiceSpecie[CODE_MAX_SIZE];
    char nomeLatino[NAME_MAX_SIZE];
    char nomeComune[NAME_MAX_SIZE];
    int giacenza;

    MYSQL_BIND returnParam[4];

    bindParam(&returnParam[0], MYSQL_TYPE_VAR_STRING, codiceSpecie, CODE_MAX_SIZE, false);
    bindParam(&returnParam[1], MYSQL_TYPE_VAR_STRING, nomeLatino, NAME_MAX_SIZE, false);
    bindParam(&returnParam[2], MYSQL_TYPE_VAR_STRING, nomeComune, NAME_MAX_SIZE, false);
    bindParam(&returnParam[3], MYSQL_TYPE_LONG, &giacenza, sizeof(int), false);


    if (mysql_stmt_bind_result(selectStocksProcedure, returnParam) != 0) {
        char message[100];
        sprintf(message, "Impossibile Recuperare Risultato %s", procedureName);
        printStatementError(selectStocksProcedure, message) ;
        freeStatement(selectStocksProcedure, false);
        return NULL;
    }

    DatabaseResult *result = myMalloc(sizeof(DatabaseResult));
    result->numRows = mysql_stmt_num_rows(selectStocksProcedure);
    result->rowsSet = myMalloc(sizeof(Plant *) * result->numRows);
    

    int hasResult = mysql_stmt_fetch(selectStocksProcedure);
    int i = 0;
    while (hasResult != 1 && hasResult != MYSQL_NO_DATA) {
        Plant* plant = myMalloc(sizeof(Plant));
        strcpy(plant->codiceSpecie, codiceSpecie);
        strcpy(plant->nomeLatino, nomeLatino);
        strcpy(plant->nomeComune, nomeComune);
        plant->giacenza = giacenza;

        result->rowsSet[i] = plant;

        hasResult = mysql_stmt_fetch(selectStocksProcedure);
        i++;
    }

    if (hasResult == 1) {
        char message[100];
        sprintf(message, "Fetch Risultato Impossibile per %s", procedureName);
        printStatementError(selectStocksProcedure, message);
        freeStatement(selectStocksProcedure, true);
        freeDatabaseResult(result);
        return NULL;
    }
    
    freeStatement(selectStocksProcedure, true);

    return result;
}

bool closeOrderAndAddStockDB(int codiceOrdine) {
    MYSQL_STMT *addStockProcedure;
    char procedureName[35] = "Chiudi ordine e aggiorna giacenza";
    if (!setupPreparedStatement(&addStockProcedure, "CALL chiudi_ordine_e_aumenta_giacenza(?)", conn)) {
        char message[100];
        sprintf(message, "Impossibile Preparare Procedura %s", procedureName);
        printMysqlError(conn, message);
        return false;
    }

    MYSQL_BIND param[1];

    bindParam(&param[0], MYSQL_TYPE_LONG, &codiceOrdine, sizeof(int), false);
    
    if(!execute_stmt(addStockProcedure, param, procedureName)) {
        freeStatement(addStockProcedure, false);
        return false;
    }

    freeStatement(addStockProcedure, false);

    return true;
}

bool registerPurchaseOrderDB(int codiceFornitore, char* specie, int quantità, double prezzo, int* codiceOrdine) {
    MYSQL_STMT *registerPurchaseOrderProcedure;
    char procedureName[45] = "Registra Ordine di Acquisto";
    if (!setupPreparedStatement(&registerPurchaseOrderProcedure, "CALL aggiungi_ordine_di_acquisto(?,?,?,?,?)", conn)) {
        char message[100];
        sprintf(message, "Impossibile Preparare Procedura %s", procedureName);
        printMysqlError(conn, message);
        return false;
    }

    MYSQL_BIND param[5];

    bindParam(&param[0], MYSQL_TYPE_LONG, &codiceFornitore, sizeof(int), false);
    bindParam(&param[1], MYSQL_TYPE_VAR_STRING, specie, strlen(specie), false);
    bindParam(&param[2], MYSQL_TYPE_LONG, &quantità, sizeof(int), false);
    bindParam(&param[3], MYSQL_TYPE_DOUBLE, &prezzo, sizeof(double), false);
    bindParam(&param[4], MYSQL_TYPE_LONG, codiceOrdine, sizeof(int), false);
  
    if(!execute_stmt(registerPurchaseOrderProcedure, param, procedureName)) {
        freeStatement(registerPurchaseOrderProcedure, false);
        return false;
    }



    //Set Parametro di OUT
    bindParam(&param[4], MYSQL_TYPE_LONG, codiceOrdine, sizeof(int), false);
  
    //Bind del parametro di OUT
    if (mysql_stmt_bind_result(registerPurchaseOrderProcedure, &param[4]) != 0) {
        char message[100];
        sprintf(message, "Impossibile Fare il Bind del risultato di %s", procedureName);
        printStatementError(registerPurchaseOrderProcedure, message) ;
        freeStatement(registerPurchaseOrderProcedure, true) ;
        return false;
    }

    //Presa del parametro di OUT

    if (mysql_stmt_fetch(registerPurchaseOrderProcedure) != 0) {
        printStatementError(registerPurchaseOrderProcedure, "Impossibile recuperare il codice ordine\n") ;
        return false;
    }

    // Consume the possibly-returned table for the output parameter
	while(mysql_stmt_fetch(registerPurchaseOrderProcedure) == 0) {}

    freeStatement(registerPurchaseOrderProcedure, true);

    return true;
}

bool registerPlantInPurchaseOrderDB(int codiceOrdine, char* specie, int quantità, double prezzo) {
    MYSQL_STMT *registerPlantInOrderProcedure;
    char procedureName[45] = "Aggiungi pianta a ordine di acquisto";
    if (!setupPreparedStatement(&registerPlantInOrderProcedure, "CALL aggiungi_specie_a_ordine_di_acquisto(?,?,?,?)", conn)) {
        char message[100];
        sprintf(message, "Impossibile Preparare Procedura %s", procedureName);
        printMysqlError(conn, message);
        return false;
    }


    MYSQL_BIND param[4];

    bindParam(&param[0], MYSQL_TYPE_LONG, &codiceOrdine, sizeof(int), false);
    bindParam(&param[1], MYSQL_TYPE_VAR_STRING, specie, strlen(specie), false);
    bindParam(&param[2], MYSQL_TYPE_LONG, &quantità, sizeof(int), false);
    bindParam(&param[3], MYSQL_TYPE_DOUBLE, &prezzo, sizeof(double), false);

    if(!execute_stmt(registerPlantInOrderProcedure, param, procedureName)) {
        freeStatement(registerPlantInOrderProcedure, false);
        return false;
    }

    freeStatement(registerPlantInOrderProcedure, false);

    return true;
}

bool addPlantToSupplierDB(int codiceFornitore, char* specie) {
    MYSQL_STMT *stmt;
    char procedureName[45] = "Aggiungi specie a fornitore";
    if (!setupPreparedStatement(&stmt, "CALL aggiungi_specie_a_fornitore(?,?)", conn)) {
        char message[100];
        sprintf(message, "Impossibile Preparare Procedura %s", procedureName);
        printMysqlError(conn, message);
        return false;
    }


    MYSQL_BIND param[2];

    bindParam(&param[0], MYSQL_TYPE_LONG, &codiceFornitore, sizeof(int), false);
    bindParam(&param[1], MYSQL_TYPE_VAR_STRING, specie, strlen(specie), false);

    if(!execute_stmt(stmt, param, procedureName)) {
        freeStatement(stmt, false);
        return false;
    }

    freeStatement(stmt, false);

    return true;
}

bool addSupplierDB(Supplier* supplier, Address* address, char* specie) {
    MYSQL_STMT *addSupplierProcedure;
    char procedureName[45] = "Aggiungi Fornitore";
    if (!setupPreparedStatement(&addSupplierProcedure, "CALL aggiungi_fornitore(?,?,?,?,?,?,?)", conn)) {
        char message[100];
        sprintf(message, "Impossibile Preparare Procedura %s", procedureName);
        printMysqlError(conn, message);
        return false;
    }


    MYSQL_BIND param[7];

    bindParam(&param[0], MYSQL_TYPE_VAR_STRING, supplier->nome, strlen(supplier->nome), false);
    bindParam(&param[1], MYSQL_TYPE_VAR_STRING, supplier->codiceFiscale, strlen(supplier->codiceFiscale), false);
    bindParam(&param[2], MYSQL_TYPE_VAR_STRING, address->via, strlen(address->via), false);
    bindParam(&param[3], MYSQL_TYPE_VAR_STRING, address->citta, strlen(address->citta), false);
    bindParam(&param[4], MYSQL_TYPE_VAR_STRING, address->civico, strlen(address->civico), false);
    bindParam(&param[5], MYSQL_TYPE_VAR_STRING, specie, strlen(specie), false);
    bindParam(&param[6], MYSQL_TYPE_LONG, &(supplier->codiceFornitore), sizeof(int), false);
    

    if(!execute_stmt(addSupplierProcedure, param, procedureName)) {
        return false;
    }
    
    //Set Parametro di OUT
    bindParam(&param[6], MYSQL_TYPE_LONG, &(supplier->codiceFornitore), sizeof(int), false);
  
    //Bind del parametro di OUT
    if (mysql_stmt_bind_result(addSupplierProcedure, &param[6]) != 0) {
        char message[100];
        sprintf(message, "Impossibile Fare il Bind del risultato di %s", procedureName);
        printStatementError(addSupplierProcedure, message) ;
        freeStatement(addSupplierProcedure, true) ;
        return false;
    }

    //Presa del parametro di OUT

    if (mysql_stmt_fetch(addSupplierProcedure) != 0) {
        printStatementError(addSupplierProcedure, "Impossibile recuperare il codice ordine\n") ;
        return false;
    }

    // Consume the possibly-returned table for the output parameter
	while(mysql_stmt_fetch(addSupplierProcedure) == 0) {}

    freeStatement(addSupplierProcedure, true);

    return true;
}

bool addAddressToSupplierDB(int codiceFornitore, Address* address) {
    MYSQL_STMT *addSupplierProcedure;
    char procedureName[45] = "Aggiungi Indirizzo a Fornitore";
    if (!setupPreparedStatement(&addSupplierProcedure, "CALL aggiungi_indirizzo_a_fornitore(?,?,?,?)", conn)) {
        char message[100];
        sprintf(message, "Impossibile Preparare Procedura %s", procedureName);
        printMysqlError(conn, message);
        return false;
    }


    MYSQL_BIND param[4];

    bindParam(&param[0], MYSQL_TYPE_VAR_STRING, address->via, strlen(address->via), false);
    bindParam(&param[1], MYSQL_TYPE_VAR_STRING, address->citta, strlen(address->citta), false);
    bindParam(&param[2], MYSQL_TYPE_VAR_STRING, address->civico, strlen(address->civico), false);
    bindParam(&param[3], MYSQL_TYPE_LONG, &codiceFornitore, sizeof(int), false);
    

    if(!execute_stmt(addSupplierProcedure, param, procedureName)) {
        return false;
    }

    freeStatement(addSupplierProcedure, false);

    return true;
}

int getTotalPurchaseOrderDB(int ordine, double* totale) {
    MYSQL_STMT *stmt;

    char procedureName[25] = "Calcola totale";
    if (!setupPreparedStatement(&stmt, "CALL totale_ordine_di_acquisto(?) ", conn)) {
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