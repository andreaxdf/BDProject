#pragma once

#define VIA_MAX_SIZE 46
#define CITTA_MAX_SIZE 46
#define CIVICO_MAX_SIZE 8


typedef struct {
    char via[VIA_MAX_SIZE];
    char citta[CITTA_MAX_SIZE];
    char civico[CIVICO_MAX_SIZE];
} Address;

void printAddressInformation(Address address, char* initialMessage);