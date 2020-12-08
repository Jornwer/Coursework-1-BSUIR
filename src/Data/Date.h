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
    explicit Date(int); //конструктор в формате ДДММГГГГ
    explicit Date(std::string &);
    Date() = default;

    bool dateCorrect();
    std::string toString();
    int dateToInt();
    static bool dateCorrect(int8_t, int8_t, int16_t);
    static std::string displayDate(std::string&);
    static bool validateDate(std::string &);

    friend bool operator==(const Date &, const Date &);
    friend bool operator>(const Date &, const Date &);
    friend bool operator<(const Date &, const Date &);
};


#endif //COURSEWORK_DATE_H
