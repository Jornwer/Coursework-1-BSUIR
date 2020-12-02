#include "Date.h"

using namespace std;

Date::Date(const string &str) {
    if (str.size() == 8) {
        day += str[0];
        day += str[1];
        month += str[2];
        month += str[3];
        year += str[4];
        year += str[5];
        year += str[6];
        year += str[7];
    }
}

int Date::countDays() const {
    //return 372 * stringToInt(year) + 31 * stringToInt(month) + stringToInt(day);
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