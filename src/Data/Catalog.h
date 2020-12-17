#ifndef COURSEWORK_CATALOG_H
#define COURSEWORK_CATALOG_H

#include "Deal.h"

class Catalog {
private:
    std::vector<Deal> deals;

    void displaySearch(std::string&, std::string&, std::string&, std::string&,
                       std::string&, std::string&, std::string&, std::string&, std::string&);
    void copyCatalogFile();
    void enterElement(Deal&);
    void rewriteDealInCatalog(Deal&, Deal&);
    void appendDealInCatalog(Deal &);
    void displayCatalog();
    void enterElement();
    void approveDeletion(int &, int &);
    void searchInCatalog();
    void changeElement(int &, int &);
    void modifyElement(void (Catalog::*)(int &, int &));
    void showBestBrands();
    void deleteDeal(Deal &);
    void showUserDeals();
    void deleteDeal();
    void changeDeal();
    void sortCatalog();
    static std::function<bool(const Deal&, const Deal&)> chooseComparator(int);

public:
    void changeCatalog();
};

#endif //COURSEWORK_CATALOG_H
