#pragma once

#include <stdbool.h>
#include "../model/Plant.h"
#include "../model/Supplier.h"
#include "../model/PurchaseOrder.h"

void departmentViewHeader();

int getDepartmentOption();

bool getStockView(char* codiceSpecie, int* giacenza);

void printAllStocks(Plant** plants, int numPlants);

bool getPurchaseOrderValues(char* specie, int* quantità, double* prezzo);

bool getSupplierView(Supplier* supplier);

void printSupplierInformation(Supplier** supplier, int num);

void printAllPurchaseOrder(PurchaseOrder** orders, int numOrders);

void printAllPlantName(Plant** plants, int numPlants);