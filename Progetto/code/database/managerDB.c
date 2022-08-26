#include "managerDB.h"
#include "databaseUtils.h"
#include <stdbool.h>
#include "../model/Client.h"
#include "../model/Contact.h"
#include "../model/Plant.h"

bool changePriceDB(char* codiceSpecie, double price) {
    MYSQL_STMT *changePriceProcedure;
    char procedureName[35] = "Modifica prezzo a specie";
    if (!setupPreparedStatement(&changePriceProcedure, "CALL modifica_prezzo_specie(?,?)", conn)) {
        char message[100];
        sprintf(message, "Impossibile Preparare Procedura %s", procedureName);
        printMysqlError(conn, message);
        return false;
    }

    MYSQL_BIND param[2];

    bindParam(&param[0], MYSQL_TYPE_VARCHAR, codiceSpecie, strlen(codiceSpecie), false);
    bindParam(&param[1], MYSQL_TYPE_DOUBLE, &price, sizeof(double), false);

    if(!execute_stmt(changePriceProcedure, param, procedureName)) {
        return false;
    }

    freeStatement(changePriceProcedure, false);

    return true;
}

bool addColorToPlantDB(char* codiceSpecie, char* colore) {
    MYSQL_STMT *addColorToPlantProcedure;
    char procedureName[35] = "Aggiungi nuova colorazione";
    if (!setupPreparedStatement(&addColorToPlantProcedure, "CALL aggiungi_colorazione(?,?)", conn)) {
        char message[100];
        sprintf(message, "Impossibile Preparare Procedura %s", procedureName);
        printMysqlError(conn, message);
        return false;
    }

    MYSQL_BIND param[2];

    bindParam(&param[0], MYSQL_TYPE_VARCHAR, codiceSpecie, strlen(codiceSpecie), false);
    bindParam(&param[1], MYSQL_TYPE_VARCHAR, colore, strlen(colore), false);

    if(!execute_stmt(addColorToPlantProcedure, param, procedureName)) {
        return false;
    }

    freeStatement(addColorToPlantProcedure, false);

    return true;
}

bool addPlantDB(Plant* plant) {
    MYSQL_STMT *addPlantProcedure;
    char procedureName[35] = "Aggiungi nuova specie";
    if (!setupPreparedStatement(&addPlantProcedure, "CALL aggiungi_specie(?,?,?,?,?,?,?)", conn)) {
        char message[100];
        sprintf(message, "Impossibile Preparare Procedura %s", procedureName);
        printMysqlError(conn, message);
        return false;
    }

    MYSQL_BIND param[7];

    bindParam(&param[0], MYSQL_TYPE_VARCHAR, plant->codiceSpecie, strlen(plant->codiceSpecie), false);
    bindParam(&param[1], MYSQL_TYPE_VARCHAR, plant->nomeLatino, strlen(plant->nomeLatino), false);
    bindParam(&param[2], MYSQL_TYPE_VARCHAR, plant->nomeComune, strlen(plant->nomeComune), false);
    bindParam(&param[3], MYSQL_TYPE_LONG, &(plant->giacenza), strlen(plant->nomeLatino), false);
    bindParam(&param[4], MYSQL_TYPE_VARCHAR, &(plant->esotica), 1, false);
    bindParam(&param[5], MYSQL_TYPE_VARCHAR, &(plant->interno), 1, false);
    bindParam(&param[6], MYSQL_TYPE_DOUBLE, &(plant->prezzo), sizeof(double), false);

    if(!execute_stmt(addPlantProcedure, param, procedureName)) {
        return false;
    }

    freeStatement(addPlantProcedure, false);

    return true;
}

bool registerUserDB(User* user, Role* role) {
    MYSQL_STMT *registrationProcedure;
    char procedureName[35] = "Aggiungi nuovo utente";
    if (!setupPreparedStatement(&registrationProcedure, "CALL aggiungi_utente(?,?,?)", conn)) {
        char message[100];
        sprintf(message, "Impossibile Preparare Procedura %s", procedureName);
        printMysqlError(conn, message);
        return false;
    }

    MYSQL_BIND param[3];
    char type[20];
    getRoleName(*role, type);

    bindParam(&param[0], MYSQL_TYPE_VARCHAR, user->username, strlen(user->username), false);
    bindParam(&param[1], MYSQL_TYPE_VARCHAR, user->password, strlen(user->password), false);
    bindParam(&param[2], MYSQL_TYPE_VARCHAR, type, strlen(type), false);

    if(!execute_stmt(registrationProcedure, param, procedureName)) {
        return false;
    }

    freeStatement(registrationProcedure, false);

    return true;
}

DatabaseResult* selectPlantPricesDB(char* codiceSpecie) {
    MYSQL_STMT *selectPlantPricesProcedure;

    char procedureName[35] = "Selezione prezzi di una specie";
    if (!setupPreparedStatement(&selectPlantPricesProcedure, "CALL seleziona_prezzi(?) ", conn)) {
        char message[100];
        sprintf(message, "Impossibile Preparare Procedura %s", procedureName);
        printMysqlError(conn, message);
        return NULL;
    }

    MYSQL_BIND param[1];

    bindParam(&param[0], MYSQL_TYPE_VARCHAR, codiceSpecie, strlen(codiceSpecie), false);

    if (mysql_stmt_bind_param(selectPlantPricesProcedure, param) != 0) {
        char message[100];
        sprintf(message, "Impossibile Fare Binding Dei Parametri di %s", procedureName);
        printStatementError(selectPlantPricesProcedure, message);
        return false;
    }

    //Esecuzione della procedura
    if (mysql_stmt_execute(selectPlantPricesProcedure) != 0) {
        char message[100];
        sprintf(message, "Impossibile Eseguire Procedeura %s", procedureName);
        printStatementError(selectPlantPricesProcedure, message);
        return NULL;
    }

    if (mysql_stmt_store_result(selectPlantPricesProcedure) != 0) {
        char message[100];
        sprintf(message, "Store Risultato Impossibile per %s", procedureName);
        printStatementError(selectPlantPricesProcedure, message);
        freeStatement(selectPlantPricesProcedure, true);
        return NULL;
    }

    double prezzo;
    MYSQL_TIME initialDate;
    MYSQL_TIME finalDate;
    MYSQL_BIND returnParam[3];

    bindParam(&returnParam[0], MYSQL_TYPE_DOUBLE, &(prezzo), sizeof(double), false);
    bindParam(&returnParam[1], MYSQL_TYPE_DATE, &(initialDate), sizeof(MYSQL_TIME), false);
    bindParam(&returnParam[2], MYSQL_TYPE_DATE, &(finalDate), sizeof(MYSQL_TIME), false);
    

    if (mysql_stmt_bind_result(selectPlantPricesProcedure, returnParam) != 0) {
        char message[100];
        sprintf(message, "Impossibile Recuperare Risultato %s", procedureName);
        printStatementError(selectPlantPricesProcedure, message) ;
        freeStatement(selectPlantPricesProcedure, false);
        return NULL;
    }

    DatabaseResult *result = myMalloc(sizeof(DatabaseResult));
    result->numRows = mysql_stmt_num_rows(selectPlantPricesProcedure);
    result->rowsSet = myMalloc(sizeof(Price *) * result->numRows);
    

    int hasResult = mysql_stmt_fetch(selectPlantPricesProcedure);
    int i = 0;
    while (hasResult != 1 && hasResult != MYSQL_NO_DATA) {
        Price* price = myMalloc(sizeof(Price));
        printf("prezzo -> %.2f\n", prezzo);
        price->price = prezzo;
        getDateParam(&(price->initialDate), &initialDate);
        printf("data inizio -> %d-%d-%d\n", initialDate.day, initialDate.month, initialDate.year);
        getDateParam(&(price->finalDate), &finalDate);
        printf("data fine -> %d-%d-%d\n", finalDate.day, finalDate.month, finalDate.year);

        result->rowsSet[i] = price;

        hasResult = mysql_stmt_fetch(selectPlantPricesProcedure);
        i++;
    }

    if (hasResult == 1) {
        char message[100];
        sprintf(message, "Fetch Risultato Impossibile per %s", procedureName);
        printStatementError(selectPlantPricesProcedure, message);
        freeStatement(selectPlantPricesProcedure, true);
        freeDatabaseResult(result);
        return NULL;
    }
    
    freeStatement(selectPlantPricesProcedure, true);

    return result;
}