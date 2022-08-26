#include <stdbool.h>
#include <ctype.h>

#include "../utils/IOUtils.h"

bool verifyCharResponse() {
    char response;
loop:
    getUserInput("", &response, 2);
    if(tolower(response) == 's') {
        return true;
    } else if(tolower(response) == 'n') {
        return false;
    } else {
        printError("Errore nell'acquisizione dell'input");
        goto loop;
    }
}

bool choiceContact() {
    colorPrint("\nUtilizzare il contatto preferito del cliente per l'ordine? (S/n)\n", GREEN_TEXT);
    return verifyCharResponse();
}

bool choiceExit() {
    colorPrint("\nLogout effettuato. Vuoi uscire dall'applicazione? (S/n)\n", RED_HIGH);
    return verifyCharResponse();
}

bool choiceFattAddress() {
    colorPrint("\nL'indirizzo di fatturazione è diverso da quello di residenza? (S/n)\n", GREEN_TEXT);
    return verifyCharResponse();
}