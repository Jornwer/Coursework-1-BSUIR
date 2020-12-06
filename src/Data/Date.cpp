#include "Date.h"
#include "../Header.h"

using namespace std;

Date::Date(int date) {
    year = date % 10000;
    date /= 10000;
    month = date % 100;
    day = date / 100;
}

Date::Date(std::string &date): Date((date.empty()?
                                                    0 :
                                                    ((date[0] == '0')?
                                                        ((date[1] == '0')?
                                                        0:
                                                        stoi(date.substr(1, 7))):
                                                    stoi(date)))) {}

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

bool Date::dateCorrect() {
    return dateCorrect(day, month, year);
}

std::string Date::toString() {
    return ((day < 10) ? "0" + to_string(day) : to_string(day)) + "." +
           ((month < 10) ? "0" + to_string(month) : to_string(month)) + "." +
           to_string(year) + string(4 - digitsInNumber(year), ' ');
}

int Date::dateToInt() {
    return day * 1000000 + month * 10000 + year;
}

std::string Date::displayDate(std::string &date) {
    uint8_t index = 0;
    string str = "ДД.ММ.ГГГГ";
    for (uint32_t i = 0; i < str.size() && index < date.size(); ++i) {
        if (i == 2 || i == 5)
            continue;
        str[i++] = date[index++];
    }
    return str;
}
