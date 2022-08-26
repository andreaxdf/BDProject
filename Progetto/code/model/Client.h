#pragma once

#include "Referent.h"
#include "Address.h"
#include <stdbool.h>

#define NAME_MAX_SIZE 46
#define SURNAME_MAX_SIZE 46
#define CF_MAX_SIZE 17
#define PARTITAIVA_MAX_SIZE 12

typedef struct {
    int codiceCliente;
    char nome[NAME_MAX_SIZE];
    char cognome[SURNAME_MAX_SIZE];
    char codiceFiscale[CF_MAX_SIZE];
    Address indirizzoFatturazione;
    Address indirizzoResidenza;
} Private_client;

typedef struct {
    int codiceCliente;
    char nome[NAME_MAX_SIZE];
    char partitaIVA[PARTITAIVA_MAX_SIZE];
    Referent referente;
    Address indirizzoFatturazione;
    Address indirizzoResidenza;
} Company;

bool checkCodiceFiscale(char* codiceFiscale);

void printClientInformation(Private_client client);

void printCompanyInformation(Company company);