#pragma once

#include "../model/Price.h"
#include <stdio.h>
#include <stdbool.h>

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

bool getIntegerFromUser(int *classCodePtr, char *requestString) ;

void colorPrint(char *printText, TextColorEnum colorEnum) ;

void printError(char *errorMessage) ;

bool getCode(char* message,int* codice);

bool getWhileInputView(char* request, char* resultBuffer, int bufferSize);

bool getWhileIntegerInputView(char* request, int* result);

bool getWhileCharInputView(char* message, char charTrue, char charFalse);

bool getWhileDoubleInputView(char* inputString, double* result);

void getDateString(Date date, char* result);