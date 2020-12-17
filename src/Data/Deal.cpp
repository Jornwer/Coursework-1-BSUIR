#include "Deal.h"
#include "../Users/AbstractUser.h"

#include <utility>
using namespace std;

void Deal::fillTableRow(fort::utf8_table &table) {
    table << brand << model << color << price << date.toString() << buyerName << seller << fort::endr;
}

Deal::Deal(int date, std::string brand, std::string model, std::string color,
           int price, std::string seller, std::string buyerName) : date(Date(date)),
                       brand(std::move(brand)), model(std::move(model)), color(std::move(color)),
                       price(price), seller(std::move(seller)),  buyerName(std::move(buyerName)) {}

Deal::Deal(nlohmann::json &j){
    brand = j["brand"];
    model = j["model"];
    color = j["color"];
    price = j["price"];
    date = Date(j["date"]);
    buyerName = j["buyerName"];
    seller = j["seller"];
}

nlohmann::json Deal::toJson() {
    nlohmann::json j;
    j["brand"] = brand;
    j["model"] = model;
    j["color"] = color;
    j["price"] = price;
    j["date"] = date.dateToInt();
    j["buyerName"] = buyerName;
    j["seller"] = seller;
    return j;
}

void Deal::setSeller() {
    seller = AbstractUser::getCurrentUser();
}

bool Deal::isDealCorrect() {
    if (brand.empty() || color.empty() || model.empty() || price == 0 || buyerName.empty()) {
        getCharacter("Одно из полей пустое. Для повторного ввода данных нажмите любую клавишу");
        return false;
    }

    if (!date.dateCorrect()) {
        getCharacter("Дата должна состоять из 8 цифр и должна существовать. "
                     "Для повторного ввода данных нажмите любую клавишу");
        return false;
    }

    time_t now = time(0);
    tm* ltm = localtime(&now);
    string currTime = (ltm->tm_mday < 10 ? "0" + to_string(ltm->tm_mday) : to_string(ltm->tm_mday))
                      + (ltm->tm_mon < 9 ? "0" + to_string(1 + ltm->tm_mon) : to_string(1 + ltm->tm_mon))
                      + to_string(1900 + ltm->tm_year);

    if (date > Date(currTime)) {
        getCharacter("Введенная дата больше настоящей даты. Для продолжения нажмите любую клавишу");
        return false;
    }

    if (date < Date(1011990)) {
        getCharacter("Введенная дата дожна быть больше 01.01.1990. Для продолжения нажмите любую клавишу");
        return false;
    }

    return true;
}

bool operator==(const Deal &lhs, const Deal &rhs) {
    return lhs.date == rhs.date &&
           lhs.brand == rhs.brand &&
           lhs.model == rhs.model &&
           lhs.color == rhs.color &&
           lhs.price == rhs.price &&
           lhs.seller == rhs.seller &&
           lhs.buyerName == rhs.buyerName;
}

bool operator!=(const Deal &lhs, const Deal &rhs) {
    return !(rhs == lhs);
}
