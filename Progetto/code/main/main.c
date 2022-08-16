#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

#include "../View/mainView.h"
#include "../Controller/LoginController.h"

int main() {
	showHeader();
	if (!loadConfiguration()) {
		exitWithError("Errore nella configuazione dell'ambiente.");
	}
	if (connectToDatabase() && compileTimeRegex()) {
		login();
	}
}