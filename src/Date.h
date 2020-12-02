#ifndef COURSEWORK_DATE_H
#define COURSEWORK_DATE_H

#include <string>

class Date {
private:
    std::string day;
    std::string month;
    std::string year;

    [[nodiscard]] int countDays() const;

public:
    explicit Date(const std::string &); //конструктор в формате ДДММГГГГ
    Date() = default;

    friend bool operator==(const Date &, const Date &);

    friend bool operator>(const Date &, const Date &);

    friend bool operator<(const Date &, const Date &);
};


#endif //COURSEWORK_DATE_H
