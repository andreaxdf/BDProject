#pragma once

#include "Address.h"


typedef struct {
    int codiceOrdine;
    int codiceCliente;
    Address indirizzoSpedizione;
    char contatto[CONTACT_VALUE_MAX_SIZE];
} SalesOrder;