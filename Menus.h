#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <Windows.h>
#include <vector>
#include <conio.h>
#include <algorithm>
#include <cstdio>
#include "SHA256.h"

void init();
void mainMenu();
void userHaveAccount();
void rewriteUserFile();
void copyUserFile();
std::string getString(std::string);
std::string getPassword(std::string);
void getCharacter(std::string);

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

	bool haveAccess;
	bool leave;

	void createAccount();
	void enterAccount();
	void enterLogin(int);
	void enterPassword();
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
	int8_t day;
	int8_t month;
	int16_t year;
};

class Car
{
public:
	std::string brand;
	std::string model;
	std::string color;
	int price;
	Date date;
};

class Catalog
{
public:
	std::vector<Car> cars;

	void changeCatalog();
};

