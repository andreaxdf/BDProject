#include "../model/Client.h"
#include "../model/Contact.h"
#include "../model/Plant.h"
#include "../model/SalesOrder.h"
#include "../model/OrderRow.h"
#include "../model/Price.h"

int getSecretaryOption();

void secretaryViewHeader();

bool addPrivateClientView(Private_client* client, Contact* contact);

bool searchPrivateClientView(char* codiceFiscale);

bool searchCompanyView(char* partitaIVA);

bool getCode(char* message,int* codice);

void printAllPlant(Plant** plants, int numPlants);

bool addCompanyView(Company* company, Contact* contactCompany, Contact* contactReferent);

void printContactInformation(Contact** contacts, int numContacts);

bool getOrderValues(char* specie, int* quantità);

bool registerAddressView(SalesOrder* order);

bool getOrderValues(char* specie, int* quantità);

void printOrderInformation(int codiceOrdine, Address address, Date date, char* contatto, int codiceCliente);

void printOrderRowsInformation(OrderRow** rows, int numRows);