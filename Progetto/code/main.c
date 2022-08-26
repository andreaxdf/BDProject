#include <stdbool.h>
#include <string.h>
#include <stdio.h>


#include "database/connector.h"
#include "config/environmentSetter.h"
#include "controller/loginController.h"
#include "view/viewUtils.h"
#include "utils/systemUtils.h"
#include "view/choiceView.h"

int main() {
	showHeader();
	if (!loadConfiguration()) {
		exitWithError("Errore nella configuazione dell'ambiente.");
	}
	if (connectToDatabase()) {
		do {
			loginController();
		} while(!choiceExit());
	}
}