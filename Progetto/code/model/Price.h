#pragma once

typedef struct {
    int year ;
    int month ;
    int day ;
} Date ;

typedef struct {
    double price;
    Date initialDate;
    Date finalDate;
} Price;