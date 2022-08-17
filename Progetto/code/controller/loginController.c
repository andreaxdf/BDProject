#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "../view/loginView.h"
#include "loginController.h"
#include "../model/User.h"
#include "../database/databaseLogin.h"

void successLogin(Role loginRole, char* username) {
    clearScreen();
    showHeader();

    if (switchRole(loginRole) == false) {
        return;
    }

    /*switch (loginRole) {
        case AMMINISTRAZIONE:
            administrationController();
            break;
        case SEGRETERIA:
            secretaryController();
            break;
        case INSEGNANTE:
            teacherController(username);
            break;
        case LOGIN:
            break;
    }

    switchRole(LOGIN);

    clearScreen();
    showHeader();
    */
    printf("loginRole");
}

void loginController() {
    User loginCredentials;

    do {
        memset(&loginCredentials, 0, sizeof(User));
        Role loginRole = LOGIN;

        if (showLoginView(&loginCredentials)) {
            loginRole = attemptLogin(&loginCredentials);
            if (loginRole == LOGIN) {
                colorPrint("Username e/o Password non validi\n\n", RED_TEXT);
            }
            else {
                successLogin(loginRole, loginCredentials.username);
            }
        }
    } while (true);
}