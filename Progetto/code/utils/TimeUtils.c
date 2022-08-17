#include "TimeUtils.h"
#include "IOUtils.h"

regex_t dateRegex ;
regex_t timeRegex ;


bool compileTimeRegex() {
    if (regcomp(&dateRegex, "[0-9]{4}-[0-9]{2}-[0-9]{2}", REG_EXTENDED) != 0) {
        printError("Impossibile Inizializzare Regex di Controllo Data") ;
        return false ;
    }
    if (regcomp(&timeRegex, "[0-9]{2}:[0-9]{2}", REG_EXTENDED) != 0) {
        printError("Impossibile Inizializzare Regex di Controllo Orario") ;
        return false ;
    }
    return true ;
}



bool verifyAndParseDate(Date *datePtr, char *dateString) {
    //Verifica su Correttezza rapporto tra mese-giorno e anno è fatta dal DBMS
    if (regexec(&dateRegex, dateString, 0, NULL, 0) != 0) {
        return false ;
    }

    char *yearString = strtok(dateString, "-") ;
    char *monthString = strtok(NULL, "-") ;
    char *dayString = strtok(NULL, "-") ;

    if (strlen(yearString) == 4 && strlen(monthString) == 2 && strlen(dayString) == 2) {
        datePtr->year = atoi(yearString) ;
        datePtr->month = atoi(monthString) ;
        datePtr->day = atoi(dayString) ;
        return true ;
    }

    return false ;
}

bool verifyAndParseTime(Time *timePtr, char *timeStr) {
    if (regexec(&timeRegex, timeStr, 0, NULL, 0) != 0) {
        return false ;
    }

    char *hourString = strtok(timeStr, ":") ;
    char *minuteString = strtok(NULL, ":") ;

    if (strlen(hourString) == 2 && strlen(minuteString) == 2) {
        timePtr->hour = atoi(hourString) ;
        timePtr->minute = atoi(minuteString) ;
        return true ;
    }

    return false ;
}