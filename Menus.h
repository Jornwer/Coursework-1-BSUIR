﻿#ifndef MENUS_H
#define MENUS_H
#include <string>
#include <iostream>
#include <fstream>
#include <Windows.h>
#include <vector>
#include <conio.h>
#include <algorithm>
#include <cstdio>
#include "SHA256.h"

class Catalog;
class Credentials;
class Car;

void mainMenu(); //функция вывода главного меню
void drawMenu(std::vector<std::wstring>, int8_t&); //функция вывода строк со стрелочкой на строке
void userHaveAccount(); //функция спрашиавющая у пользователя способ входа
void rewriteFile(std::vector<Credentials>&, std::string); //функция перезаписи пользовательского файла
void copyFile(std::vector<Credentials>&, std::string); //функция копирования данных с пользовательского файла
std::string getString(std::wstring); //функция ввода строки с сообщение
std::string getPassword(std::wstring); //функция ввода пароля в формате **** с сообщением
void getCharacter(std::wstring); //функция для нажатия любой клавишы без вывода
bool copyCatalogFile(Catalog&); //функция копирования данных с файла каталога
void rewriteCatalogFile(Catalog&); //функция перезаписи файла каталога
int8_t getCharCode(); //функция выводящая код нажатой клавиши
void displayDate(std::string); //функция вывода даты в формате ДД.ММ.ГГГГ
int stringToInt(std::string); //функция перевода строки в целое число
void adminHaveAccount(); //функция спрашиавющая у админа способ входа
std::string enterLogin(int8_t,bool&,bool&); //функция ввода логина с проверкой на правильность
std::string enterPassword(bool&); //функция ввода пароля с проверкой на правильность
bool carCorrect(Car, std::string);
bool carCorrect(Car);
bool dayCorrect(int8_t, int8_t, int16_t); //проверяет введенный день на правильность

class Credentials
{
public:
	Credentials(std::string, std::string); //конструктор логин+пароль
	Credentials() = default; // конструктор по умолчанию
	std::string login; //логин
	std::string password; //пароль

	friend bool operator==(const Credentials&, const Credentials&); //перегрузка оператора ==
};

class User
{
public:
	Credentials credentials; //логин+пароль

	virtual void enterAccount(std::vector<Credentials>&); //функция входа в аккаунт
	
	void userMenu(std::vector<Credentials>&); //основное меню пользователя
	void changePassword(std::vector<Credentials>&, std::string); //функция смены пароля
	void deleteAccount(std::vector<Credentials>&, std::string);// функция удаления аккаунта
	int checkPasswords();//функция ввода пароля и проверки его с существующим
};

class Admin: public User
{
public:
	void createAccount(std::vector<Credentials>&); //функция создания аккаунта
	void adminMenu(std::vector<Credentials>&); //основное меню админа
	void enterAccount(std::vector<Credentials>&) override; // переопределение функции входа в аккаунт
	void addAdmin(); // функция превращения пользователя в админа
	void deleteUser(); //функция удаления пользователя
	void addUser(); //функция добавления пользователя
};

class Date
{
private:
	int countDays() const;//функция подсчета дней с 0 года(приблизительно)
public:
	std::string day;//день
	std::string month;//месяц
	std::string year;//год

	Date(std::string); //конструктор в формате ДДММГГГГ
	Date();
	

	friend bool operator==(const Date&, const Date&);
	friend bool operator>(const Date&, const Date&);// перегрузка оператора больше
	friend bool operator<(const Date&, const Date&);//перегрузка оператора меньше
};

class Car
{
public:
	std::string brand;//марка
	std::string model;//модель
	std::string color;//цвет
	std::string price;//стоимость
	Date date;//дата

	friend bool operator==(const Car&, const Car&);
};

class Catalog
{
public:
	std::vector<Car> cars; // вектор машин

	void changeCatalog(); //функция меню каталога
	void displayCatalog(); //вывод каталога
	void addElement(); //функция добавления элемента в каталог
	void deleteElement();//функция удаления элемента из каталога
	void displayElement(Car car);//функция вывода 1 элемента каталога
	void approveDeletion(int, int);//функция подтверждения удаления
	void searchInCatalog();//функция поиска в каталоге
	void displaySearch(std::string, std::string, std::string, std::string, std::string, std::string, std::string);//функция выводящая найденные элементы в каталоге
};
#endif