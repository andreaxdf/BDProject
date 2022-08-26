#pragma once

#include <stdbool.h>
#include <stdio.h>
#include <mysql/mysql.h>
#include "../model/User.h"
#include "databaseUtils.h"
#include <stdbool.h>
#include "../controller/loginController.h"

#define DB_HOST "DB.HOST"
#define DB_PORT "DB.PORT" 
#define DB_NAME "DB.NAME"

#define DB_LOGIN_USER "LOGIN.USER"
#define DB_LOGIN_PASSWD "LOGIN.PASSWD"

#define DB_SEGRETERIA_USER "SEGRETERIA.USER"
#define DB_SEGRETERIA_PASSWD "SEGRETERIA.PASSWD"

#define DB_MANAGER_USER "MANAGER.USER"
#define DB_MANAGER_PASSWD "MANAGER.PASSWD"

#define DB_GESTORE_USER "GESTORE.USER"
#define DB_GESTORE_PASSWD "GESTORE.PASSWD"

#define DB_OPERATORE_USER "OPERATORE.USER"
#define DB_OPERATORE_PASSWD "OPERATORE.PASSWD"

extern MYSQL* conn;

bool connectToDatabase();

bool initializePreparedStatement(Role role);

bool closeAllStatement();