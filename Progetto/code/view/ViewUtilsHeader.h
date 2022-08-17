#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "../utils/IOUtils.h"

void showMenu(char **menuOptionsArray, int optionsNumber) ;
void showOptionHeader() ;

void printOptionTitle(char *optionName) ;

int getUserOption(char *menuOption[], int menuLen) ;

void clearScreen() ;

void showAppHeader() ;