#pragma once

enum SecretaryControllerOptions {
    REGISTER_ORDER = 0,
    AVAILABLE_PLANT,
    SEARCH_PRIVATE_CLIENT,
    SEARCH_COMPANY,
    ADD_PRIVATE_CLIENT,
    ADD_COMPANY,
    ADD_CONTACT,
    SELECT_CONTACT,
    CHANGE_PREFERITE_CONTACT,
    SHOW_SALES_ORDER,
    END_OPTION
};

void secretaryController();

void showSalesOrder();