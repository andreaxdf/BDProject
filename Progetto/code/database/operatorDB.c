#include "operatorDB.h"
#include "../model/Plant.h"

DatabaseResult* selectSalesOrderToSendDB() {
    MYSQL_STMT *stmt;

    char procedureName[35] = "Selezione Ordini ancora da evadere";
    if (!setupPreparedStatement(&stmt, "CALL seleziona_ordini_di_vendita_non_evasi() ", conn)) {
        char message[100];
        sprintf(message, "Impossibile Preparare Procedura %s", procedureName);
        printMysqlError(conn, message);
        return NULL;
    }

    if (mysql_stmt_execute(stmt) != 0) {
        char message[100];
        sprintf(message, "Impossibile Eseguire Procedeura %s", procedureName);
        printStatementError(stmt, message);
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

    int ordine;

    MYSQL_BIND returnParam[1];

    bindParam(&returnParam[0], MYSQL_TYPE_LONG, &ordine, sizeof(int), false);

    if (mysql_stmt_bind_result(stmt, returnParam) != 0) {
        char message[100];
        sprintf(message, "Impossibile Recuperare Risultato %s", procedureName);
        printStatementError(stmt, message) ;
        freeStatement(stmt, false);
        return NULL;
    }

    DatabaseResult *result = myMalloc(sizeof(DatabaseResult));
    result->numRows = mysql_stmt_num_rows(stmt);
    result->rowsSet = myMalloc(sizeof(int *) * result->numRows);
    

    int hasResult = mysql_stmt_fetch(stmt);
    int i = 0;
    while (hasResult != 1 && hasResult != MYSQL_NO_DATA) {
        int* codice = myMalloc(sizeof(int));
        
        *codice = ordine;

        result->rowsSet[i] = codice;

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

DatabaseResult* selectPlantToSendDB(int ordine) {
    MYSQL_STMT *stmt;

    char procedureName[35] = "Selezione piante da inviare";
    if (!setupPreparedStatement(&stmt, "CALL selezione_piante_da_inviare(?) ", conn)) {
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
    int quantità;


    MYSQL_BIND returnParam[2];


    bindParam(&returnParam[0], MYSQL_TYPE_VAR_STRING, codiceSpecie, CODE_MAX_SIZE, false);
    bindParam(&returnParam[1], MYSQL_TYPE_LONG, &quantità, sizeof(int), false);
    

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
        plant->giacenza = quantità;

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

bool addPackageDB(int ordine, char* specie, int quantità, int* pacco) {
    MYSQL_STMT *stmt;
    char procedureName[25] = "Aggiungi Pacco";
    if (!setupPreparedStatement(&stmt, "CALL aggiungi_pacco(?,?,?)", conn)) {
        char message[100];
        sprintf(message, "Impossibile Preparare Procedura %s", procedureName);
        printMysqlError(conn, message);
        return false;
    }

    int temp;

    MYSQL_BIND param[3];

    bindParam(&param[0], MYSQL_TYPE_LONG, &ordine, sizeof(int), false);
    bindParam(&param[1], MYSQL_TYPE_VAR_STRING, specie, strlen(specie), false);
    bindParam(&param[2], MYSQL_TYPE_LONG, &quantità, sizeof(int), false);
    
    
    if(!execute_stmt_and_store_result(stmt, param, procedureName)) {
        return 1;
    }

    MYSQL_BIND returnParam[1];

    bindParam(&returnParam[0], MYSQL_TYPE_LONG, &temp, sizeof(int), false);

    int ret = bind_and_fetch_one_result(stmt, returnParam, procedureName);
    *pacco = temp;

    if(ret != 1) return true;

    return false;
}

bool addPlantToPackageDB(int ordine, int pacco, char* specie, int quantità) {
    MYSQL_STMT *stmt;
    char procedureName[25] = "Aggiungi Specie a Pacco";
    if (!setupPreparedStatement(&stmt, "CALL aggiungi_pianta_a_pacco(?,?,?,?)", conn)) {
        char message[100];
        sprintf(message, "Impossibile Preparare Procedura %s", procedureName);
        printMysqlError(conn, message);
        return false;
    }

    MYSQL_BIND param[4];

    bindParam(&param[0], MYSQL_TYPE_LONG, &ordine, sizeof(int), false);
    bindParam(&param[1], MYSQL_TYPE_LONG, &pacco, sizeof(int), false);
    bindParam(&param[2], MYSQL_TYPE_VAR_STRING, specie, strlen(specie), false);
    bindParam(&param[3], MYSQL_TYPE_LONG, &quantità, sizeof(int), false);
    
    if(!execute_stmt(stmt, param, procedureName)) {
        return false;
    }

    freeStatement(stmt, false);

    return true;
}

bool closePackageDB(int ordine, int pacco) {
    MYSQL_STMT *stmt;
    char procedureName[25] = "Chiudi Pacco";
    if (!setupPreparedStatement(&stmt, "CALL chiudi_pacco(?,?)", conn)) {
        char message[100];
        sprintf(message, "Impossibile Preparare Procedura %s", procedureName);
        printMysqlError(conn, message);
        return false;
    }

    MYSQL_BIND param[2];

    bindParam(&param[0], MYSQL_TYPE_LONG, &ordine, sizeof(int), false);
    bindParam(&param[1], MYSQL_TYPE_LONG, &pacco, sizeof(int), false);
    
    if(!execute_stmt(stmt, param, procedureName)) {
        return false;
    }

    freeStatement(stmt, false);

    return true;
}