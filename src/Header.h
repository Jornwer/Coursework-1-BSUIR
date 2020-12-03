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
­ авторизация пользователя, хранение пароля в зашифрованном виде;                               done
­ просмотр необходимой информации;                                                              done
­ добавление, редактирование и удаление записей;                                                done
­ поиск, сортировка и фильтрация записей;                                                       done
­ реализация заказа (вывод отчета);                                                             done
­ другие опции в зависимости от специфики задачи.                                               done
- Исходная и итоговая информация должна храниться в текстовых (бинарных) файлах.                done
­ реализация базовых принципов объектно-ориентированного программирования;                      done
­ использование стандартных и пользовательских функции, дружественных и виртуальных функции;    done
­ использование пространств имен (стандартных и собственных);                                   done
­ реализация обработки ошибок программы (средствами языка С);                                   done
­ использование механизмов абстракции;                                                          done
­ использование классов и наследования;                                                         done
­ использование перегрузки операторов и методов;                                                done
­ использование шаблонов классов и методов;                                                     todo
­ использование динамического выделения памяти.                                                 done
- Обязательно использовать сокрытие данных (инкапсуляцию),                                      done
- переопределение методов,                                                                      done
- абстрактные классы,                                                                           done
- передачу параметров по ссылке и по значению,                                                  done
- статические методы и поля.                                                                    done
*/
class Catalog;
class Car;
class Deal;

void mainMenu();
void drawMenu(std::vector<std::string>, int8_t &);
std::string getString(const std::string &);
void getCharacter(const std::string &);
int8_t getCharCode();
void displayDate(std::string);

void userHaveAccount();
void rewriteFile(std::vector<Credentials> &, const std::string &);
void copyFile(std::vector<Credentials> &, const std::string &);
std::string getString(const std::string &);
void getCharacter(const std::string &);
bool copyCatalogFile(Catalog &);
void rewriteCatalogFile(Catalog &);
void adminHaveAccount();
bool dealCorrect(const Deal &, const std::string &);
bool dealCorrect(const Deal &);
bool stringContainString(std::string &, std::string &);
bool comparePairs(std::pair<std::string, int>, std::pair<std::string, int>);

class Car {
public:
    std::string brand;//марка
    std::string model;//модель
    std::string color;//цвет
    std::string price;//стоимость
};

class Deal {
public:
    Car car;
    Date date;
    std::string seller;
    std::string buyerName;
    std::string buyerSurname;

    void displayElement();//функция вывода 1 элемента каталога

    friend bool operator==(const Deal &, const Deal &);
};

class Catalog {
public:
    std::vector<Deal> deals;

    void changeCatalog(); //функция меню каталога
    void displayCatalog(); //вывод каталога
    void enterElement(Deal &, std::string); //функция добавления элемента в каталог
    void approveDeletion(int &, int &);//функция подтверждения удаления
    void searchInCatalog();//функция поиска в каталоге
    void changeElement(int &, int &);

    void modifyElement(void (Catalog::*)(int &, int &));

    void displaySearch(std::string, std::string, std::string, std::string,
                       std::string, std::string, std::string, std::string,
                       std::string);//функция выводящая найденные элементы в каталоге
    void showBestBrands();
};

#endif