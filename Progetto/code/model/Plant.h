#pragma once

#define CODE_MAX_SIZE 11
#define NAME_MAX_SIZE 46


typedef struct {
    char codiceSpecie[CODE_MAX_SIZE];
    char nomeLatino[NAME_MAX_SIZE];
    char nomeComune[NAME_MAX_SIZE];
    int giacenza;
    double prezzo;
    char esotica; //'e' esotica, 'n' non esotica
    char interno; //'i' da interno, 'e' da esterno
    char** colori;
} Plant;