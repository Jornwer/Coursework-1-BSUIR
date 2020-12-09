#ifndef COURSEWORK_MENU_H
#define COURSEWORK_MENU_H

#include "Header.h"

template <typename T, typename fPtr = void (T::*)()>
class Menu {
private:
    std::vector<std::string> strings;
    std::vector<fPtr> functions;
    std::vector<int> exitOn;
    T clazz;

public:
    Menu(std::string description, fPtr function){
        strings.emplace_back(description);
        functions.emplace_back(function);
    }

    Menu* add(std::string description, fPtr function) {
        strings.emplace_back(description);
        functions.emplace_back(function);
        return this;
    }

    Menu* add(std::string description) {
        strings.emplace_back(description);
        return this;
    }

    Menu* exit(int exit) {
        exitOn.push_back(exit);
        return this;
    }

    Menu* addClass(T c){
        clazz = c;
        return this;
    }

    void whileTrue(){
        int8_t row = 0;
        uint32_t colNum = strings.size();
        if (exitOn.empty())
            throw "Endless loop";
        while (true) {
            system("cls");
            drawMenu(strings, row);

            char a = getCharCode();
            if (a == VK_UP) row = (row + colNum - 1) % colNum;
            else if (a == VK_DOWN) row = (row + 1) % colNum;
            else if (a == 13) {
                if (row < functions.size())
                    (clazz.*functions[row])();
                if (std::find(exitOn.begin(), exitOn.end(), row) != exitOn.end()) {
                    system("cls");
                    return;
                }
            }
        }
    }
};

#endif //COURSEWORK_MENU_H
