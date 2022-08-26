#include "databaseUtils.h"


bool is_null = true ;
/*
    Funzione per effettuare il binding dei parametri ed eseguire uno statement.
    - stmt : statement da eseguire
    - param : parametri di cui effettuare il binding
    Ritorna true in caso di successo, false di fallimento.
*/
bool execute_stmt(MYSQL_STMT* stmt, MYSQL_BIND* param, char* procedureName) {

    //Binding parametri della procedura
    if (mysql_stmt_bind_param(stmt, param) != 0) {
        char message[100];
        sprintf(message, "Impossibile Fare Binding Dei Parametri di %s", procedureName);
        printStatementError(stmt, message);
        return false;
    }

    //Esecuzione della procedura
    if (mysql_stmt_execute(stmt) != 0) {
        char message[100];
        sprintf(message, "Impossibile Eseguire Procedeura %s", procedureName);
        printStatementError(stmt, message);
        freeStatement(stmt, true);
        return false;
    }

    return true;
}

int bind_and_fetch_one_result(MYSQL_STMT* stmt, MYSQL_BIND* returnParam, char* procedureName) {
    if (mysql_stmt_bind_result(stmt, returnParam) != 0) {
        printStatementError(stmt, "Errore Bind Del Risultato");
        freeStatement(stmt, false);
        return 1;
    }

    int hasResult = mysql_stmt_fetch(stmt);

    if (hasResult == 1) {
        freeStatement(stmt, true);
        char message[100];
        sprintf(message, "Fetch Risultato Impossibile per %s", procedureName);
        printStatementError(stmt, message);
        return 1;
    }

    if(hasResult == MYSQL_NO_DATA) {
        freeStatement(stmt, true);
        return 2;
    }

    // Consume the possibly-returned table for the output parameter
	while(mysql_stmt_fetch(stmt) == 0) {}

    freeStatement(stmt, true);
        
    return 0;
}

bool execute_stmt_and_store_result(MYSQL_STMT* stmt, MYSQL_BIND* param, char* procedureName) {

    if(!execute_stmt(stmt, param, procedureName)) {
        freeStatement(stmt, true);
        return false;
    }

    if (mysql_stmt_store_result(stmt) != 0) {
        char message[100];
        sprintf(message, "Store Risultato Impossibile per %s", procedureName);
        printStatementError(stmt, message);
        freeStatement(stmt, true);
        return false;
    }

    return true;
}

/*
    Funzione per preparare un paramentro MYSQL_BIND.
    - mysqlParam : Puntatore al MYSQL_BIND da preparare
    - mysqlType : Tipo di dato MYSQL da legare
    - paramPtr : Puntatore al buffer da cui prendere/in cui inserire l'informazione
    - paramSize : Lunghezza del buffer puntato da paramPtr (strlen / sizeof)
    - nullable : Indica se il paramPtr è NULL
*/
void bindParam(MYSQL_BIND *mysqlParam, enum enum_field_types mysqlType, void *paramPtr, unsigned long paramSize, bool nullable) {

    memset(mysqlParam, 0, sizeof(MYSQL_BIND));

    mysqlParam->buffer = paramPtr;
    mysqlParam->buffer_type = mysqlType;
    mysqlParam->buffer_length = paramSize;

    if (nullable) mysqlParam->is_null = &is_null ;
}

//Fa la Conversione da Tipo Date a MYSQL_TIME
void prepareDateParam(Date *datePtr , MYSQL_TIME *mysqlTime) {

    memset(mysqlTime, 0, sizeof(MYSQL_TIME)) ;

    mysqlTime->day = datePtr->day ;
    mysqlTime->month = datePtr->month ;
    mysqlTime->year = datePtr->year ;
}

void getDateParam(Date *datePtr, MYSQL_TIME *mysqlTime) {
    memset(datePtr, 0, sizeof(Date)) ;
    datePtr->year = mysqlTime->year ;
    datePtr->month = mysqlTime->month ;
    datePtr->day = mysqlTime->day ;
}

void printMysqlError(MYSQL *conn, char *errorMessage) {
    char sqlErrorMessage[500] ;
    if (conn != NULL) {
#if MYSQL_VERSION_ID >= 40101
	    sprintf (sqlErrorMessage, "%s\nError %u (%s): %s\n",
			errorMessage, mysql_errno (conn), mysql_sqlstate(conn), mysql_error (conn));
#else
		sprintf(sqlErrorMessage, "%s\nErrore %u : %s", 
            errorMessage, mysql_errno(conn), mysql_error(conn)) ;
#endif
	}
    printError(sqlErrorMessage) ;
}

void printStatementError(MYSQL_STMT *statement, char *errorMessage) {
    char statementErrorMessage[500] ;
    sprintf(statementErrorMessage, "\n%s\nErrore %d : %s\n", errorMessage, mysql_stmt_errno(statement), mysql_stmt_error(statement)) ;
    printError(statementErrorMessage) ;
}


/*
 Funzione per liberare uno statement dopo l'esecuzione
    - statement : statement da liberare
    - freeSet : Indica se deve essere consumato il result set ritornato dallo statement
 */
void freeStatement(MYSQL_STMT *statement, bool freeSet) {
    
    if (freeSet) while (mysql_stmt_next_result(statement) == 0);
    mysql_stmt_free_result(statement);
    mysql_stmt_reset(statement);

    mysql_stmt_close(statement);
}

//Inizializza uno statement MYSQL
bool setupPreparedStatement(MYSQL_STMT **statement, char *statementCommand, MYSQL *conn) {

    *statement = mysql_stmt_init(conn) ;

    if (*statement == NULL) {
        printMysqlError(conn, "Impossibile Inizializzare Statement") ;
        return false ;
    }

    if (mysql_stmt_prepare(*statement, statementCommand, strlen(statementCommand)) != 0) {
        printStatementError(*statement, "Impossibile Associare Comando e Statement") ;
        return false ;
    }

    bool update = true ;
    mysql_stmt_attr_set(*statement, STMT_ATTR_UPDATE_MAX_LENGTH, &update) ;

    return true ;
}

void freeDatabaseResult(DatabaseResult *databaseResultPtr) {
    if (databaseResultPtr == NULL) {
        free(databaseResultPtr) ;
        return ;
    }
    int numRows = databaseResultPtr->numRows ;
    for (int i = 0 ; i < numRows ; i++) {
        free(databaseResultPtr->rowsSet[i]) ;
    }
    free(databaseResultPtr->rowsSet) ;
    free(databaseResultPtr) ;
}

