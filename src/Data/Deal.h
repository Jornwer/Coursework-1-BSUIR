#ifndef COURSEWORK_DEAL_H
#define COURSEWORK_DEAL_H

#include "Date.h"
#include <fort.hpp>

class Deal {
private:
    Date date{};
    std::string brand;
    std::string model;
    std::string color;
    int price{};
    std::string seller;
    std::string buyerName;

    friend class Catalog;
public:
    Deal() = default;
    explicit Deal(nlohmann::json&);
    Deal(int, std::string brand, std::string model, std::string color,
         int price, std::string seller, std::string buyerName);

    void fillTableRow(fort::utf8_table &);
    nlohmann::json toJson();
    void setSeller();
    bool isDealCorrect();

    bool operator==(const Deal &rhs) const;
    bool operator!=(const Deal &rhs) const;
};


#endif //COURSEWORK_DEAL_H
