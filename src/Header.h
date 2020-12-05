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
}

/*
� ����������� ������������, �������� ������ � ������������� ����;                               done
� �������� ����������� ����������;                                                              done
� ����������, �������������� � �������� �������;                                                done
� �����, ���������� � ���������� �������;                                                       done
� ���������� ������ (����� ������);                                                             done
� ������ ����� � ����������� �� ��������� ������.                                               done
- �������� � �������� ���������� ������ ��������� � ��������� (��������) ������.                done
� ���������� ������� ��������� ��������-���������������� ����������������;                      done
� ������������� ����������� � ���������������� �������, ������������� � ����������� �������;    done
� ������������� ����������� ���� (����������� � �����������);                                   done
� ���������� ��������� ������ ��������� (���������� ����� �);                                   done
� ������������� ���������� ����������;                                                          done
� ������������� ������� � ������������;                                                         done
� ������������� ���������� ���������� � �������;                                                done
� ������������� �������� ������� � �������;                                                     todo
� ������������� ������������� ��������� ������.                                                 done
- ����������� ������������ �������� ������ (������������),                                      done
- ��������������� �������,                                                                      done
- ����������� ������,                                                                           done
- �������� ���������� �� ������ � �� ��������,                                                  done
- ����������� ������ � ����.                                                                    done
*/
class Catalog;
class Car;
class Deal;

void mainMenu();
void drawMenu(std::vector<std::string>, int8_t &);
std::string getString(const std::string &);
std::string getString();
void getCharacter(const std::string &);
int8_t getCharCode();
void displayDate(std::string);
bool isCharacterValid(char &);

/*void rewriteFile(std::vector<Credentials> &, const std::string &);
void copyFile(std::vector<Credentials> &, const std::string &);*/
bool copyCatalogFile(Catalog &);
void rewriteCatalogFile(Catalog &);
bool dealCorrect(const Deal &, const std::string &);
bool dealCorrect(const Deal &);
bool stringContainString(std::string &, std::string &);
bool comparePairs(std::pair<std::string, int>, std::pair<std::string, int>);

class Car {
public:
    std::string brand;//�����
    std::string model;//������
    std::string color;//����
    std::string price;//���������
};

class Deal {
public:
    Car car;
    Date date;
    std::string seller;
    std::string buyerName;
    std::string buyerSurname;

    void displayElement();//������� ������ 1 �������� ��������

    friend bool operator==(const Deal &, const Deal &);
};

class Catalog {
public:
    std::vector<Deal> deals;

    void changeCatalog(); //������� ���� ��������
    void displayCatalog(); //����� ��������
    void enterElement(Deal &, std::string); //������� ���������� �������� � �������
    void approveDeletion(int &, int &);//������� ������������� ��������
    void searchInCatalog();//������� ������ � ��������
    void changeElement(int &, int &);

    void modifyElement(void (Catalog::*)(int &, int &));

    void displaySearch(std::string, std::string, std::string, std::string,
                       std::string, std::string, std::string, std::string,
                       std::string);//������� ��������� ��������� �������� � ��������
    void showBestBrands();
};

#endif