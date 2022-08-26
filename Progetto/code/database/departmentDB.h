#pragma once

#include "databaseUtils.h"
#include "../model/Supplier.h"
#include "../model/Address.h"

DatabaseResult* searchSupplierDB(char* codiceSpecie);

bool closeOrderAndAddStockDB(int codiceOrdine);

DatabaseResult* selectStocksDB();

bool registerPurchaseOrderDB(int codiceFornitore, char* specie, int quantità, double prezzo, int* codiceOrdine);

bool registerPlantInPurchaseOrderDB(int codiceOrdine, char* specie, int quantità, double prezzo);

bool addSupplierDB(Supplier* supplier, Address* address, char* specie);

bool addAddressToSupplierDB(int codiceFornitore, Address* address);

DatabaseResult* selectOpenedOrderDB();

bool addPlantToSupplierDB(int codiceFornitore, char* specie);

DatabaseResult* selectPlantInOrderDB(int ordine);

int getTotalPurchaseOrderDB(int ordine, double* totale);