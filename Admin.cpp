﻿#include "Menus.h"

using namespace std;

void adminHaveAccount()
{
	int8_t row = 0;

	Admin admin;
	vector<Credentials> admins;
	copyFile(admins, "admins.txt");
	while (true)
	{
		system("cls");

		if (admins.empty()) drawMenu({ L"  Создать аккаунт",L"\n\n  Назад" }, row);
		else drawMenu({ L"  Войти в аккаунт",L"\n\n  Назад" }, row);

		char a = getCharCode();

		if (a == VK_UP) row = (row + 1) % 2;
		else if (a == VK_DOWN) row = (row + 1) % 2;
		else if (a == 13)
		{
			if (admins.empty())
			{
				if (row == 0) admin.createAccount(admins);
				else if (row == 1)
				{
					system("cls");
					break;
				}
				return;
			}
			else {
				if (row == 0) admin.enterAccount(admins);
				else if (row == 1)
				{
					system("cls");
					break;
				}
				return;
			}
		}
	}
}

void Admin::createAccount(vector<Credentials>& admins)
{
	system("cls");

	bool haveAccess = true;
	bool leave = false;

	do {
		if (haveAccess) this->credentials.login = enterLogin(0, haveAccess, leave);
		else this->credentials.login = enterLogin(2, haveAccess, leave);
		if (leave) break;
		this->credentials.password = enterPassword(leave);
		if (leave) break;

		vector<Credentials> users;
		copyFile(users, "users.txt");

		for (auto i : users)
		{
			if (i.login == this->credentials.login)
			{
				haveAccess = false;
				break;
			}
		}

	} while (!haveAccess);

	if (!leave)
	{
		ofstream file("users.txt", ios::app);
		file << this->credentials.login << endl << sha256(this->credentials.password) << endl;
		file.close();
		delete &file;
		adminMenu(admins);
	}
}

void Admin::enterAccount(vector<Credentials>& admins)
{
	system("cls");
	bool haveAccess = true;
	bool leave = false;

	do {
		if (haveAccess) this->credentials.login = enterLogin(0, haveAccess, leave);
		else this->credentials.login = enterLogin(1, haveAccess, leave);
		if (leave) break;
		this->credentials.password = enterPassword(leave);
		if (leave) break;

		haveAccess = true;

		Credentials tmp(credentials.login, sha256(credentials.password));
		if (find(admins.begin(), admins.end(), tmp) == admins.end())
			haveAccess = false;

	} while (!haveAccess);

	if (!leave)
	{
		adminMenu(admins);
	}
}

void Admin::adminMenu(vector<Credentials>& admins)
{
	int8_t row = 0;
	int8_t colNum = 7;
	Catalog catalog;
	while (true)
	{
		system("cls");
		drawMenu({ L" Изменить каталог", L"\n\n Изменить пароль",L"\n\n Удалить аккаунт",
			L"\n\n Назначить нового администратора", L"\n\n Удалить пользователя", L"\n\nДобавить нового пользователя", L"\n\n Назад" }, row);

		char a = getCharCode();

		if (a == VK_UP) row = (row + colNum - 1) % colNum;
		else if (a == VK_DOWN) row = (row + 1) % colNum;
		else if (a == 13)
		{
			if (row == 0) catalog.changeCatalog();
			else if (row == 1) changePassword(admins);
			else if (row == 2) { deleteAccount(admins); break; }
			else if (row == 3) addAdmin();
			else if (row == 4) deleteUser();
			else if (row == 5) addUser();
			else if (row == 6)
			{
				system("cls");
				break;
			}
		}
	}
}

void Admin::addAdmin()
{
	string str = getString(L"Введите имя пользователя. Введите exit для выхода");
	if (str == "exit") return;

	vector<Credentials> users;
	copyFile(users, "users.txt");
	auto it = users.begin();

	for (; it != users.end(); ++it)
		if (it->login == str)
			break;

	if (it != users.end())
	{
		ofstream file("admins.txt", ios::app);
		file << it->login << endl << it->password << endl;

		users.erase(it);
		rewriteFile(users, "users.txt");

		getCharacter(L"Пользователь повышен. Для возвращения в меню нажмите любую клавишу");
		return;
	}
	else getCharacter(L"Пользователь не найден. Для возвращения в меню нажмите любую клавишу");
}

void Admin::deleteUser()
{
	string str = getString(L"Введите имя пользователя. Введите exit для выхода");
	if (str == "exit") return;
	vector<Credentials> users;

	copyFile(users, "users.txt");
	auto it = users.begin();

	for (; it != users.end(); ++it)
		if (it->login == str)
			break;

	if (it != users.end())
	{
		users.erase(it);
		rewriteFile(users, "users.txt");
		getCharacter(L"Пользователь удален. Для возвращения в меню нажмите любую клавишу");
		return;
	}
	else getCharacter(L"Пользователь не найден. Для возвращения в меню нажмите любую клавишу");
}

void Admin::addUser()
{
	system("cls");

	Credentials user;

	bool haveAccess = true;
	bool leave = false;
	do {
		if (haveAccess) user.login = enterLogin(0, haveAccess, leave);
		else user.login = enterLogin(2, haveAccess, leave);
		if (leave) break;

		user.password = enterPassword(leave);
		if (leave) break;

		haveAccess = true;

		ifstream file("admins.txt");
		while (file)
		{
			string tmp;
			getline(file, tmp);
			if (tmp == user.login)
			{
				haveAccess = false;
				break;
			}
			getline(file, tmp);
		}
		file.close();
		file.open("users.txt");
		while (file)
		{
			string tmp;
			getline(file, tmp);
			if (tmp == user.login)
			{
				haveAccess = false;
				break;
			}
			getline(file, tmp);
		}

	} while (!haveAccess);

	if (!leave)
	{
		ofstream file("users.txt", ios::app);
		file << user.login << endl << sha256(user.password) << endl;
		getCharacter(L"Пользователь успешно добавлен. Для продолжения нажмите любую клавишу");
	}
}

void Admin::deleteAccount(vector<Credentials>& admins)
{
	do {
		int i = checkPasswords();

		if (!i)
		{
			Credentials tmp(this->credentials.login, sha256(this->credentials.password));

			auto ptr = find(admins.begin(), admins.end(), tmp);
			if (ptr != admins.end())
			{
				admins.erase(ptr);
				rewriteFile(admins, "admins.txt");
				return;
			}
		}
		else if (i == 2) break;

	} while (true);
}
