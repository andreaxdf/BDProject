#pragma once

#define CONTACT_VALUE_MAX_SIZE 46

typedef enum {
    PHONE = 0,
    CELL_PHONE = 1,
    EMAIL = 2
} type_contact;

typedef struct {
    type_contact type;
    char value[CONTACT_VALUE_MAX_SIZE];
} Contact;

void getName(type_contact type, char* result);

type_contact getType(char* typeName);