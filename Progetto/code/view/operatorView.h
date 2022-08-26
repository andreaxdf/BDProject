#pragma once

#include "../model/Plant.h"

void operatorViewHeader();

int getOperatorOption();

void printCodeAndQuantity(Plant** plants, int numPlants);

void printOrderCodes(int** orders, int numOrders);