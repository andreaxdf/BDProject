#pragma once

enum DepartmentControllerOptions {
    ADD_STOCK,
    SELECT_STOCK,
    ADD_PURCHASE_ORDER,
    SHOW_PURCHASE_ORDER_OPEN,
    SHOW_PLANT_IN_ORDER,
    ADD_SUPPLIER,
    ADD_SUPPLIER_ADDRESS,
    SEARCH_SUPPLIER,
    END_OPTION_DEPARTMENT
};

void departmentController();

void selectStock();