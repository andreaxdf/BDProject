#pragma once

#include <stdbool.h>
#include "databaseUtils.h"
#include "connector.h"


bool addPackageDB(int ordine, char* specie, int quantità, int* pacco);

bool addPlantToPackageDB(int ordine, int pacco, char* specie, int quantità);

bool closePackageDB(int ordine, int pacco);

DatabaseResult* selectPlantToSendDB(int ordine);

DatabaseResult* selectSalesOrderToSendDB();