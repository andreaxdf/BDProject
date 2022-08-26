#pragma once

#define NAME_MAX_SIZE 46
#define SURNAME_MAX_SIZE 46

typedef struct {
    int codiceContattabile;
    char nome[NAME_MAX_SIZE];
    char cognome[SURNAME_MAX_SIZE];
} Referent;