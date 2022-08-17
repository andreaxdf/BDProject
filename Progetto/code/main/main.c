#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

#include "../view/mainView.h"
#include "../database/connector.h"
#include "../config/environmentSetter.h"
#include "../controller/loginController.h"
#include "../view/ViewUtilsHeader.h"
#include "../utils/SystemUtilsHeader.h"
#include "../utils/TimeUtils.h"

int main() {
	showHeader();
	if (!loadConfiguration()) {
		exitWithError("Errore nella configuazione dell'ambiente.");
	}
	if (connectToDatabase()) {
		login();
	}
}