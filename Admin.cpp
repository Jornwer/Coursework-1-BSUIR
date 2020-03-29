#include "Menus.h"

using namespace std;

void copyAdminFile(vector<Credentials>& admins)
{
	ifstream file("admins.txt");
	while (file)
	{
		Credentials temp;
		getline(file, temp.login);
		getline(file, temp.password);
		admins.push_back(temp);
	}
	if (!admins.empty()) admins.erase(admins.end() - 1);
	file.close();
}

void rewriteAdminFile(vector<Credentials>& admins)
{
	ofstream file("admins.txt", ios::trunc);

	for (auto i : admins)
		file << i.login << endl << i.password << endl;

	file.close();
}

void adminHaveAccount()
{
	int8_t row = 0;

	Admin admin;
	vector<Credentials> admins;
	copyAdminFile(admins);
	while (true)
	{
		system("cls");

		if (admins.empty()) drawMenu({ "  Создать аккаунт","\n\n  Назад", }, row);
		else drawMenu({ "  Войти в аккаунт","\n\n  Назад", }, row);

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
		if (haveAccess) enterLogin(0, haveAccess, leave);
		else enterLogin(2, haveAccess, leave);
		if (leave) break;
		enterPassword(leave);
		if (leave) break;

		vector<Credentials> users;
		copyUserFile(users);

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
		addCredentials(admins);
		rewriteAdminFile(admins);
		adminMenu(admins);
	}
}

void Admin::enterAccount(vector<Credentials>& admins)
{
	system("cls");
	bool haveAccess = true;
	bool leave = false;

	do {
		if (haveAccess) enterLogin(0, haveAccess, leave);
		else enterLogin(1, haveAccess, leave);
		if (leave) break;
		enterPassword(leave);
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
	int8_t colNum = 6;
	Catalog catalog;
	while (true)
	{
		system("cls");
		drawMenu({ " Изменить каталог", "\n\n Изменить пароль","\n\n Удалить аккаунт" , "\n\n Назначить нового администратора", "\n\n Удалить пользователя", "\n\n Назад" }, row);

		char a = getCharCode();

		if (a == VK_UP) row = (row + colNum - 1) % colNum;
		else if (a == VK_DOWN) row = (row + 1) % colNum;
		else if (a == 13)
		{
			if (row == 0) catalog.changeCatalog();
			else if (row == 1) changePassword(admins);
			else if (row == 2) { deleteAccount(admins); break; }
			else if (row == 3) addAdmin(admins);
			else if (row == 4) deleteUser();
			else if (row == 5)
			{
				system("cls");
				break;
			}
		}
	}
}

void Admin::addAdmin(vector<Credentials>& admins)
{
	string str = getString("Введите имя пользователя. Введите exit для выхода");
	if (str == "exit") return;
	vector<Credentials> users;

	copyUserFile(users);
	auto it = users.begin();

	for (; it != users.end(); ++it)
		if (it->login == str)
			break;

	if (it != users.end())
	{
		Credentials tmp(it->login, it->password);
		admins.push_back(tmp);
		rewriteAdminFile(admins);
		users.erase(it);
		rewriteUserFile(users);
		getCharacter("Пользователь повышен. Для возвращения в меню нажмите любую клавишу");
		return;
	}
	else getCharacter("Пользователь не найден. Для возвращения в меню нажмите любую клавишу");
}

void Admin::deleteUser()
{
	string str = getString("Введите имя пользователя. Введите exit для выхода");
	if (str == "exit") return;
	vector<Credentials> users;

	copyUserFile(users);
	auto it = users.begin();

	for (; it != users.end(); ++it)
		if (it->login == str)
			break;

	if (it != users.end())
	{
		users.erase(it);
		rewriteUserFile(users);
		getCharacter("Пользователь удален. Для возвращения в меню нажмите любую клавишу");
		return;
	}
	else getCharacter("Пользователь не найден. Для возвращения в меню нажмите любую клавишу");
}
