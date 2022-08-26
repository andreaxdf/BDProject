#pragma once

#include "Price.h"
#include "Supplier.h"

typedef struct {
    int codiceOrdine;
    Date data;
    int codiceFornitore;
    char nome[46];
    char codiceFiscale[16];
} PurchaseOrder ;