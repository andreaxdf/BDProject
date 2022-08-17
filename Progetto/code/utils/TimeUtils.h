#pragma once

#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <regex.h>


typedef struct {
    int year ;
    int month ;
    int day ;
} Date ;

typedef struct {
    int hour ;
    int minute ;
    int second ;
} Time ;


bool verifyAndParseDate(Date *datePtr, char *stringDate) ;

bool verifyAndParseTime(Time *timePtr, char *stringTime) ;

bool compileTimeRegex() ;