#ifndef COURSEWORK_DATE_H
#define COURSEWORK_DATE_H

#include <string>
#include "../../lib/json.hpp"

class Date {
private:
    int day;
    int month;
    int year;

    [[nodiscard]] int countDays() const;

public:
    explicit Date(const std::string &); //конструктор в формате ДДММГГГГ
    explicit Date(nlohmann::json &);
    Date() = default;

    static bool dateCorrect(int8_t, int8_t, int16_t);

    friend bool operator==(const Date &, const Date &);
    friend bool operator>(const Date &, const Date &);
    friend bool operator<(const Date &, const Date &);

    [[nodiscard]] int getDay() const;
    [[nodiscard]] int getMonth() const;
    [[nodiscard]] int getYear() const;
};


#endif //COURSEWORK_DATE_H
