#include "../model/User.h"
#include "../model/Plant.h"
#include "../model/Price.h"

void managerViewHeader();

int getManagerOption();

bool getUserInformationView(User* user, Role* role);

bool getPlantInformationView(Plant* plant);

bool getColorView(char* codiceSpecie, char* colore);

bool getPriceView(char* codiceSpecie, double* price);

void printAllPrices(Price** prices, int numPrices);