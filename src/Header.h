#ifndef MENUS_H
#define MENUS_H

#include <string>
#include <cmath>
#include <iostream>
#include <fstream>
#include <Windows.h>
#include <vector>
#include <conio.h>
#include <algorithm>
#include <cstdio>
#include <ctime>
#include <unordered_map>
#include "../lib/SHA256.h"
/*
везде слова предм.область заменить под свою
мало описания предм.области
никакого описания вашей шифровки

фильтрацию (скрины работы) и сортировку вообще не нашла
добавить про сортировку в:
    юз кейс
    дк
    описание
нормально код накидать
*/

namespace Constants {
    const std::string pathToUserFile = "data/users.json";
    const std::string pathToAdminFile = "data/admins.json";
    const std::string pathToCatalogFile = "data/catalog.json";
    const int elementsOnPage = 20;
}

void mainMenu();
void drawMenu(std::vector<std::string>, int8_t &);
std::string getString(const std::string &);
std::string getString();
void getCharacter(const std::string &);
int8_t getCharCode();
bool isCharacterValid(char &);
bool isStringContainString(std::string &stringForSearch, std::string &searchingString);

template<typename T>
int digitsInNumber(T number){
    int n=1;
    while ((number/=10) > 0) n++;
    return n;
}

#endif