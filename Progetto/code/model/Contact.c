#include "Contact.h"
#include <stdio.h>
#include <string.h>

void getName(type_contact type, char* result) {
    switch (type) {
        case PHONE:
            sprintf(result, "telefono");
            break;
        case CELL_PHONE:
            sprintf(result, "cellulare");
            break;
        case EMAIL:
            sprintf(result, "email");
            break;
    }
}

type_contact getType(char* typeName) {
    if(strcmp(typeName, "telefono") == 0) {
        return PHONE;
    } else if(strcmp(typeName, "cellulare") == 0) {
        return CELL_PHONE;
    } else if(strcmp(typeName, "email") == 0) {
        return EMAIL;
    }

    return PHONE;
}