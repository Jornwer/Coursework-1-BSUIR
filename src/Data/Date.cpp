#include "Date.h"

using namespace std;

Date::Date(int date) {
    year = date % 10000;
    date /= 10000;
    month = date % 100;
    day = date / 100;
}

Date::Date(nlohmann::json &j) {
    day = j["day"];
    month = j["month"];
    year = j["year"];
}

int Date::countDays() const {
    return 372 * year + 31 * month + day;
}

bool operator==(const Date &l, const Date &r) {
    return (l.day == r.day && l.year == r.year && l.month == r.month);
}

bool operator<(const Date &l, const Date &r) {
    return l.countDays() < r.countDays();
}

bool operator>(const Date &l, const Date &r) {
    return l.countDays() > r.countDays();
}

bool Date::dateCorrect(int8_t day, int8_t month, int16_t year) {
    if (day > 31 || day == 0 || month == 0 || month > 12 || year == 0) return false;
    if (day < 29) return true;
    if (month == 2) {
        if (day == 29) {
            if (year % 4) return false;
            if (!(year % 400)) return true;
            return year % 100 != 0;

        }
        return false;
    }
    if (month <= 7) {
        if (month % 2) return true;
        else if (day != 31) return true;
        else return false;
    }
    else {
        if (!month % 2) return true;
        else if (day != 31) return true;
        else return false;
    }
}

int Date::getDay() const {
    return day;
}

int Date::getMonth() const {
    return month;
}

int Date::getYear() const {
    return year;
}
