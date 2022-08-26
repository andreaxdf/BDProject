#include "../view/viewUtils.h"
#include "databaseUtils.h"
#include "connector.h"
#include "../model/Plant.h"

bool registerUserDB(User* user, Role* role);

bool addPlantDB(Plant* plant);

bool addColorToPlantDB(char* codiceSpecie, char* colore);

bool changePriceDB(char* codiceSpecie, double price);

DatabaseResult* selectPlantPricesDB(char* codiceSpecie);