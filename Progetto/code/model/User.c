#include "User.h"
#include <stdio.h>
#include "../utils/IOUtils.h"

void getRoleName(Role role, char* result) {
    switch (role) {
        case SEGRETERIA:
            sprintf(result, "segreteria");
            break;
        case MANAGER:
            sprintf(result, "manager");
            break;
        case OPERATORE:
            sprintf(result, "operatore");
            break;
        case GESTORE_MAGAZINO:
            sprintf(result, "gestore_magazzino");
            break;
        default:
            printError("Errore - Conversione da Role a String impossibile");
    }
}