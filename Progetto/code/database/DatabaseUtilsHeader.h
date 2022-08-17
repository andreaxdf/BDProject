#pragma once

#include <mysql/mysql.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "../utils/TimeUtils.h"
#include "../utils/IOUtils.h"
#include "../utils/TimeUtils.h"

//Introduco per evitare di andare a riprendere il numero di righe ogni volta che devo stampare una tabella
typedef struct {
    int numRows ;
    void **rowsSet ;
} DatabaseResult ;

void freeDatabaseResult(DatabaseResult *databaseResultPtr) ; 


void bindParam(MYSQL_BIND *mysqlParam, enum enum_field_types mysqlType, void *paramPtr, unsigned long paramSize, bool nullable) ;

void prepareDateParam(Date *datePtr , MYSQL_TIME *mysqlTime) ;

void prepareTimeParam(Time *timePtr, MYSQL_TIME *mysqlTime) ;

void printMysqlError(MYSQL *conn, char *errorMessage) ;

void printStatementError(MYSQL_STMT *statement, char *errorMessage) ;

void freeStatement(MYSQL_STMT *statement, bool freeResultSet) ;

bool setupPreparedStatement(MYSQL_STMT **statement, char *statementCommand, MYSQL *conn) ;

void getDateParam(Date *datePtr, MYSQL_TIME *mysqlTime) ;

void getTimeParam(Time *timePtr, MYSQL_TIME *mysqlTime) ;