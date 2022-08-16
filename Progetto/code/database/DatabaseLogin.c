#include "DatabaseLogin.h"
#include <mysql/mysql.h>
#include "../controller/loginController.h"
#include "connector.h"
#include "DatabaseUtilsHeader.h"
#include <string.h>
#include <stdio.h>
#include "../utils/IOUtils.h"
#include "../model/User.h"

Role attemptLogin(User *loginCredentials) {
    MYSQL_STMT *loginProcedure ;
    if (!setupPreparedStatement(&loginProcedure, "CALL login(?,?,?) ", conn)) {
        printMysqlError(conn, "Impossibile Preparare Procedura 'Login'") ;
        return false ;
    }
    /*
        La procedura di Login ritorna un'unica tabella nel result set contenente
        il Terzo parametro che è un parametro di OUT: questo parametro è un codice numerico associato
        all'utente e che indica il ruolo dell'utente. 
        Il codice numerico tornato dal DB è mappato nell'enum Role 
    */

    MYSQL_BIND param[3] ;
    int role = LOGIN ; // Default set to LOGIN

    bindParam(&param[0], MYSQL_TYPE_VAR_STRING, loginCredentials->username, strlen(loginCredentials->username), false) ; //Set username Param
    bindParam(&param[1], MYSQL_TYPE_VAR_STRING, loginCredentials->password, strlen(loginCredentials->password), false) ; // Bind password Param
    bindParam(&param[2], MYSQL_TYPE_LONG, &role, sizeof(role), false) ;

    //Binding parametri della procedura
    if (mysql_stmt_bind_param(loginProcedure, param) != 0) {
        printStatementError(loginProcedure, "Impossibile Fare Binding Dei Parametri di Login") ;
        freeStatement(loginProcedure, false) ;
        return role ;
    }

    //Esecuzione della procedura
    if (mysql_stmt_execute(loginProcedure) != 0) {
        printStatementError(loginProcedure, "Impossibile Eseguire Procedeura di Login") ;
        freeStatement(loginProcedure, false) ;
        return role ;
    }

    //Set Parametro di OUT
    bindParam(&param[2], MYSQL_TYPE_LONG, &role, sizeof(role), false) ;

    //Bind del parametro di OUT
    if (mysql_stmt_bind_result(loginProcedure, &param[2]) != 0) {
        printStatementError(loginProcedure, "Impossibile Fare il Bind del risultato di Login") ;
        freeStatement(loginProcedure, true) ;
        return role ;
    }

    //Presa del parametro di OUT

    if (mysql_stmt_fetch(loginProcedure) != 0) {
        printStatementError(loginProcedure, "Impossibile Recuperare Ruolo") ;
        return role ;
    }

    //Consumo resto del resul set attuale
    while (mysql_stmt_fetch(loginProcedure) == 0) ;

    freeStatement(loginProcedure, true) ;

    return role ;

}

/*
bool switchRole(Role role) {
    char *databaseName = getenv("DB.NAME") ;
    char *username ;
    char *password ;

    switch(role) {
        case AMMINISTRAZIONE :
            username = getenv(DB_AMMINISTRAZIONE_USER) ;
            password = getenv(DB_AMMINISTRAZIONE_PASSWD) ;
            break ;
        case SEGRETERIA :
            username = getenv(DB_SEGRETERIA_USER) ;
            password = getenv(DB_SEGRETERIA_PASSWD) ;
            break ;
        case INSEGNANTE :
            username = getenv(DB_INSEGNANTE_USER) ;
            password = getenv(DB_INSEGNANTE_PASSWD) ;
            break ;
        case LOGIN :
            username = getenv(DB_LOGIN_USER) ;
            password = getenv(DB_LOGIN_PASSWD) ;
            break ;
    }

    if (username == NULL || password == NULL || databaseName == NULL) {
        printError("Errore : Variabili d'Ambiente Non Trovate") ;
        return false ;
    }

    if (mysql_change_user(conn, username, password, databaseName) != 0) {
        printMysqlError(conn, "Impossibile Cambiare Privilegi Utente") ;
        return false ;
    }

    return true ;
}*/