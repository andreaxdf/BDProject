#include "Address.h"
#include "../utils/IOUtils.h"

void printAddressInformation(Address address, char* initialMessage) {
    colorPrint(initialMessage, GREEN_TEXT);
    char message[150];
    sprintf(message, "%s %s, %s\n",
        address.via,
        address.civico,
        address.citta);
    colorPrint(message, GREEN_TEXT);
}