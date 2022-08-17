#include "loginView.h"

bool showLoginView(User* loginCredentialsPtr) {

    printf("Inserire Credenziali:\n");

    if (!getUserInput("Username >>> ", loginCredentialsPtr->username, USERNAME_MAX_SIZE)) {
        printError("Errore Lettura Username");
        return false;
    }
    if (!getUserInput("Password >>> ", loginCredentialsPtr->password, PASSWORD_MAX_SIZE)) {
        printError("Errore Lettura Password");
        return false;
    }

    return true;

}