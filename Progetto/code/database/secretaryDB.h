#include "../model/Client.h"
#include "../controller/secretaryController.h"
#include "../view/viewUtils.h"
#include "databaseUtils.h"
#include "connector.h"
#include "../view/secretaryView.h"
#include "../model/Contact.h"
#include "../model/SalesOrder.h"

int searchPrivateClientDB(Private_client* clientResult, char* codiceFiscale);

int searchCompanyDB(Company* company, char* partitaIVA);

int getTotalSalesOrderDB(int ordine, double* totale);

bool addPrivateClientDB(Private_client* client, Contact* contact);

bool addContactDB(int codiceContattabile, Contact* contact);

DatabaseResult* selectAvailablePlantDB();

DatabaseResult* selectContatsDB(int codiceContattabile);

bool addCompanyDB(Company* company, Contact* contactCompany, Contact* contactReferent);

bool changePreferiteContactDB(int codiceContattabile, char* preferiteContact);

bool registerOrderDB(SalesOrder* order, char* specie, int quantità);

bool registerPlantInOrderDB(int codiceOrdine, char* specie, int quantità);

bool closeOrderDB(int codiceOrdine);

int selectSalesOrderDB(int codiceOrdine, Address* address, Date* date, char* contatto, int* codiceCliente);

DatabaseResult* selectOrderRowsDB(int codiceOrdine);

int getPreferiteContactDB(int codiceCliente, Contact* preferiteContact);