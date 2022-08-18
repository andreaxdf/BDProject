#include <stdbool.h>
#include <stdio.h>
#include <mysql/mysql.h>
#include "connector.h"

MYSQL* conn;

bool connectToDatabase() {
    char* host = getenv(DB_HOST);
    char* port = getenv(DB_PORT);
    char* databaseName = getenv(DB_NAME);
    char* loginUser = getenv(DB_LOGIN_USER);
    char* loginPassword = getenv(DB_LOGIN_PASSWD);
	bool* reconnect = true;

    if (host == NULL || port == NULL || loginUser == NULL || loginPassword == NULL || databaseName == NULL) {
        fprintf(stderr, "Variabili d'Ambiente non Trovate\n");
        return false;
    }

    conn = mysql_init(NULL);
    if (conn == NULL) {
        fprintf(stderr, "Errore in Inizializzazione Librerie\n");
        return false;
    }

    if (mysql_real_connect(
            conn,
            host,
            loginUser,
            loginPassword,
            databaseName,
            atoi(port),
            NULL, 
            CLIENT_MULTI_STATEMENTS | CLIENT_MULTI_RESULTS | 
            CLIENT_COMPRESS | CLIENT_INTERACTIVE | CLIENT_REMEMBER_OPTIONS) == NULL) {
        fprintf(stderr, "Errore di Connessione al Database\n%s\n\n", mysql_error(conn));
        return false;
    }
	
	if(mysql_options(conn, MYSQL_OPT_RECONNECT, &reconnect)) {
		print_error(conn, "[mysql_options] failed.");
	}

    return true;
}