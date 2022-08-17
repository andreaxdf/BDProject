

#include "ViewUtilsHeader.h"

void showMenu(char **menuOptionsArray, int optionsNumber) {
    for (int i = 0 ; i < optionsNumber ; i++) {
        printf("%d. %s\n", i, menuOptionsArray[i]) ;
    }
    printf("Scegliere Opzione [%d-%d] >>> ", 0, optionsNumber - 1) ;
}

void showOptionHeader() {
    int headerSize = 75 ;
    printf("\n") ;
    for (int i = 0 ; i < headerSize ; i ++) putchar('*') ;
    printf("\n") ;
}

void printOptionTitle(char *optionName) {
    colorPrint(optionName, GREEN_TEXT) ;
    printf("\n") ;
}

int getUserOption(char *menuOption[], int menuLen) {
    printf("\n") ;
    colorPrint("Cosa Posso Fare Per Te??", GREEN_HIGH) ;
    printf("\n") ;
    showMenu(menuOption, menuLen) ;

    int selectedOption ;
    if (!getIntegerFromUser(&selectedOption, "")) {
        selectedOption = -1 ;
    }

    return selectedOption ;
}

void clearScreen() {
    printf("\033[2J\033[H");
}

void printHeaderLine(int headerLenght) {
    for (int i = 0 ; i < headerLenght ; i++) {
        if (i == 0 || i == headerLenght - 1) putchar('+') ;
        else putchar('-') ;
    }
}

void showAppHeader() {
    char *appTitleHeader = "| English School Management System |" ;
    printHeaderLine(strlen(appTitleHeader)) ;
    printf("\n%s\n", appTitleHeader) ;
    printHeaderLine(strlen(appTitleHeader)) ;
    printf("\n") ;
}