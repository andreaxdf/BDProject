#pragma once

#include <stdbool.h>
#include "TimeUtils.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>
#include <errno.h>
#include <ctype.h>
#include <limits.h>
#include "SystemUtilsHeader.h"


#define RED_TEXT_MARK 31 
#define GREEN_TEXT_MARK 32 

#define RED_HIGH_TEXT_MARK 41
#define GREEN_HIGH_TEXT_MARK 42

typedef enum {
    RED_TEXT ,
    GREEN_TEXT ,
    RED_HIGH ,
    GREEN_HIGH 
} TextColorEnum ;


bool getUserInput(char *requestString, char *inputBuffer, int inputMaxSize) ;

bool getDateFromUser(Date *datePtr, char *requestString) ;

bool getTimeFromUser(Time *timePtr, char *requestString) ;

bool getIntegerFromUser(int *classCodePtr, char *requestString) ;

void colorPrint(char *printText, TextColorEnum colorEnum) ;

void printError(char *errorMessage) ;
