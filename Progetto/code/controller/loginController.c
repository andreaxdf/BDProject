#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../view/loginView.h"
#include "loginController.h"
#include "../utils/systemUtils.h"
#include "../utils/IOUtils.h"
#include "../view/viewUtils.h"
#include "../model/User.h"
#include "../database/databaseLogin.h"
#include "secretaryController.h"
#include "managerController.h"
#include "departmentController.h"
#include "operatorController.h"

void successLogin(Role loginRole) {
    clearScreen();
    showHeader();

    if (switchRole(loginRole) == false) {
        return;
    }

    switch (loginRole) {
        case SEGRETERIA:
            secretaryController();
            break;
        case MANAGER:
            managerController();
            break;
        case OPERATORE:
            operatorController();
            break;
        case GESTORE_MAGAZINO:
            departmentController();
            break;
        default:
            break;
    }

    switchRole(LOGIN);

    clearScreen();
    showHeader();
}

void loginController() {
    User loginCredentials;
    bool condition = true;

    do {
        memset(&loginCredentials, 0, sizeof(User));
        Role loginRole = LOGIN;

        if (showLoginView(&loginCredentials)) {
            loginRole = attemptLogin(&loginCredentials);
            if (loginRole == LOGIN) {
                colorPrint("\nUsername e/o Password non validi\n\n", RED_TEXT);
            }
            else {
                successLogin(loginRole);
                condition = false;
            }
        }
    } while (condition);
}