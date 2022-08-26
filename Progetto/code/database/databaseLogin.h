#pragma once

#include <stdbool.h>
#include "../controller/loginController.h"
#include "databaseUtils.h"
#include "../model/User.h"

Role attemptLogin(User *loginDredentialsPtr) ;

bool switchRole(Role newRole) ;