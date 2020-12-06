#ifndef COURSEWORK_CATALOG_H
#define COURSEWORK_CATALOG_H

#include "Deal.h"

class Catalog {
private:
    std::vector<Deal> deals;

    void displaySearch(std::string&, std::string&, std::string&, std::string&,
                       std::string&, std::string&, std::string&, std::string&);
    void copyCatalogFile();
    void enterElement(Deal&);
    static void rewriteDealInCatalog(Deal&, Deal&);
    static void appendDealInCatalog(Deal &);

public:
    void changeCatalog();
    void displayCatalog();
    void enterElement();
    void approveDeletion(int &, int &);
    void searchInCatalog();
    void changeElement(int &, int &);
    void modifyElement(void (Catalog::*)(int &, int &));
    void showBestBrands();
};

#endif //COURSEWORK_CATALOG_H
