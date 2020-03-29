#ifndef MENUS_H
#define MENUS_H
//todo проверять правильный ввод в поиске каталога
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

void mainMenu();
void drawMenu(std::vector<std::string>, int8_t&);
void userHaveAccount();
void rewriteUserFile(std::vector<Credentials>&);
void copyUserFile(std::vector<Credentials>&);
std::string getString(std::string);
std::string getPassword(std::string);
void getCharacter(std::string);
void copyCatalogFile(Catalog&);
void rewriteCatalogFile(Catalog&);
void copyAdminFile(std::vector<Credentials>&);
void rewriteAdminFile(std::vector<Credentials>&);
int getInt(std::string);
int8_t getCharCode();
void displayDate(std::string);
int stringToInt(std::string);
void adminHaveAccount();

class Credentials
{
public:
	Credentials(std::string, std::string);
	Credentials();
	std::string login;
	std::string password;

	bool operator==(Credentials);
};

class User
{
public:
	Credentials credentials;

	virtual void createAccount(std::vector<Credentials>&);
	virtual void enterAccount(std::vector<Credentials>&);
	void enterLogin(int8_t,bool&,bool&);
	void enterPassword(bool&);
	void userMenu(std::vector<Credentials>&);
	void addCredentials(std::vector<Credentials>&);
	void changePassword(std::vector<Credentials>&);
	void deleteAccount(std::vector<Credentials>&);
	int checkPasswords();
};

class Admin: public User
{
public:
	void createAccount(std::vector<Credentials>&) override;
	void adminMenu(std::vector<Credentials>&);
	void enterAccount(std::vector<Credentials>&) override;
	void addAdmin(std::vector<Credentials>&);
	void deleteUser();

};

class Date
{
public:
	std::string day;
	std::string month;
	std::string year;

	Date(std::string);
	Date();
	void getDate();
	int countDays();
	int8_t enterDay(int8_t, int16_t);
	int8_t enterMonth();
	int16_t enterYear();
	bool dayCorrect(int8_t, int16_t, int8_t);

	bool operator==(Date);
	friend std::ostream& operator<<(std::ostream&,const Date&);
	bool operator>(Date);
	bool operator<(Date);
};

class Car
{
public:
	std::string brand;
	std::string model;
	std::string color;
	std::string price;
	Date date;

	void getBrand();
	void getModel();
	void getColor();

	bool operator==(Car);
};

class Catalog
{
public:
	std::vector<Car> cars;

	void changeCatalog();
	void displayCatalog();
	void addElement();
	void deleteElement();
	void displayElement(Car car);
	void approveDeletion(int, int);
	void searchInCatalog();
	void displaySearch(std::string, std::string, std::string, std::string, std::string, std::string, std::string);
};
#endif