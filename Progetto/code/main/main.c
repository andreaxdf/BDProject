#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

#include "../view/mainView.h"
#include "../config/environmentSetter.h"
#include "../controller/loginController.h"

int main() {
	showHeader();
	if (!loadConfiguration()) {
		exitWithError("Errore nella configuazione dell'ambiente.");
	}
	if (connectToDatabase() && compileTimeRegex()) {
		login();
	}
}