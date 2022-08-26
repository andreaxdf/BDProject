#include "../model/Contact.h"
#include "../model/Address.h"
#include "../model/Referent.h"
#include "../utils/IOUtils.h"
#include "../model/Client.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

void clearScreen();

void showHeader();

int getUserOption(const char *menuOption[], int menuLen);

bool getContactView(Contact* contact);

bool getAddressView(Address* address);

bool getWhileInputView(char* request, char* resultBuffer, int bufferSize);

void printValue(char* value, int width);

bool getReferentView(Referent* referent);