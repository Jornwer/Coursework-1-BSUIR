#ifndef MENUS_H
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

class Credentials;
class Catalog;
//
//extern Catalog catalog;
//extern std::vector<Credentials> users;

void init();
void mainMenu();
void drawMenu(std::vector<std::string>, int8_t&);
void userHaveAccount();
void rewriteUserFile();
void copyUserFile();
std::string getString(std::string);
std::string getPassword(std::string);
void getCharacter(std::string);
void copyCatalogFile();
void rewriteCatalogFile();
void copyAdminFile();
void rewriteAdminFile();
int getInt(std::string);


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

	void createAccount();
	void enterAccount();
	void enterLogin(int8_t,bool&,bool&);
	void enterPassword(bool&);
	void userMenu();
	void addCredentials();
	void changePassword();
	void deleteAccount();
	int checkPasswords();
};

class Admin
{
public:
	Credentials credentials;
	bool haveAccess;

};

class Date
{
public:
	std::string day;
	std::string month;
	std::string year;

	void getDate();
	int8_t enterDay(int8_t, int16_t);
	int8_t enterMonth();
	int16_t enterYear();
	bool dayCorrect(int8_t, int16_t, int8_t);

	bool operator==(Date);
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
};
#endif