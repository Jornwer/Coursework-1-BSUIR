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
#include "Data/Date.h"

namespace Constants {
    const std::string pathToUserFile = "data/users.json";
    const std::string pathToAdminFile = "data/admins.json";
    const std::string pathToCatalogFile = "data/catalog.json";
}

#define ultimateDebugging(x) cout << x;getCharCode();
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

void mainMenu();
void drawMenu(std::vector<std::string>, int8_t &);
std::string getString(const std::string &);
std::string getString();
void getCharacter(const std::string &);
int8_t getCharCode();
bool isCharacterValid(char &);

template<typename T>
int digitsInNumber(T number){
    int n=1;
    while ((number/=10) > 0) n++;
    return n;
}

/*void rewriteFile(std::vector<Credentials> &, const std::string &);
void copyFile(std::vector<Credentials> &, const std::string &);*/
/*bool copyCatalogFile(Catalog &);
void rewriteCatalogFile(Catalog &);
bool dealCorrect(const Deal &, const std::string &);
bool dealCorrect(const Deal &);*/
bool stringContainString(std::string &, std::string &);
bool comparePairs(std::pair<std::string, int>, std::pair<std::string, int>);

#endif