#pragma once

#include "../model/Price.h"
#include <mysql/mysql.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "../utils/IOUtils.h"
#include "../model/Price.h"
#include "../utils/systemUtils.h"

//Introduco per evitare di andare a riprendere il numero di righe ogni volta che devo stampare una tabella
typedef struct {
    int numRows ;
    void **rowsSet ;
} DatabaseResult ;

bool execute_stmt(MYSQL_STMT* stmt, MYSQL_BIND* param, char* procedureName);

bool execute_stmt_and_store_result(MYSQL_STMT* stmt, MYSQL_BIND* param, char* procedureName);

int bind_and_fetch_one_result(MYSQL_STMT* stmt, MYSQL_BIND* returnParam, char* procedureName);

void freeDatabaseResult(DatabaseResult *databaseResultPtr) ; 

void bindParam(MYSQL_BIND *mysqlParam, enum enum_field_types mysqlType, void *paramPtr, unsigned long paramSize, bool nullable) ;

void printMysqlError(MYSQL *conn, char *errorMessage) ;

void printStatementError(MYSQL_STMT *statement, char *errorMessage) ;

void freeStatement(MYSQL_STMT *statement, bool freeResultSet) ;

bool setupPreparedStatement(MYSQL_STMT **statement, char *statementCommand, MYSQL *conn) ;

void getDateParam(Date *datePtr, MYSQL_TIME *mysqlTime) ;