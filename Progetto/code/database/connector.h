#pragma once

#include <stdbool.h>
#include <mysql/mysql.h>
#include "../controller/LoginControllerHeader.h"
#include "../model/User.h"

#define DB_HOST "DB.HOST"
#define DB_PORT "DB.PORT" 
#define DB_NAME "DB.NAME"

#define DB_LOGIN_USER "LOGIN.USER"
#define DB_LOGIN_PASSWD "LOGIN.PASSWD"

extern MYSQL* conn;

bool connectToDatabase();

bool initializePreparedStatement(Role role);

bool closeAllStatement();