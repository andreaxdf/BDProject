#pragma once

#define USERNAME_MAX_SIZE 100 + 1
#define PASSWORD_MAX_SIZE 45 + 1

typedef struct {
    char username[USERNAME_MAX_SIZE];
    char password[PASSWORD_MAX_SIZE];
} User;

typedef enum {
    CLIENTE = 0,
    MANAGER = 1,
    OPERATORE = 2,
    DIPARTIMENTO_MAGAZINO = 3,
    LOGIN = 4
} Role;