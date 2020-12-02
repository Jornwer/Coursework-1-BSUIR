#include "Header.h"

using namespace std;

void adminHaveAccount() {
	int8_t row = 0;

	Admin admin;
	vector<Credentials> admins;
	copyFile(admins, "data/admins.txt");
	while (true) {
		system("cls");

		if (admins.empty()) drawMenu({ L"  Создать аккаунт",L"\n\n  Назад" }, row);
		else drawMenu({ L"  Войти в аккаунт",L"\n\n  Назад" }, row);

		char a = getCharCode();

		if (a == VK_UP) row = (row + 1) % 2;
		else if (a == VK_DOWN) row = (row + 1) % 2;
		else if (a == 13) {
			if (admins.empty()) {
				if (row == 0) admin.createAccount(admins);
				else if (row == 1) {
					system("cls");
					break;
				}
				return;
			}
			else {
				if (row == 0) admin.enterAccount(admins);
				else if (row == 1) {
					system("cls");
					break;
				}
				return;
			}
		}
	}
}

void Admin::createAccount(vector<Credentials>& admins) {
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
		copyFile(users, "data/users.txt");

		for (const auto& i : users) {
			if (i.login == this->credentials.login) {
				haveAccess = false;
				break;
			}
		}

	} while (!haveAccess);

	if (!leave) {
		ofstream file("data/admins.txt", ios::trunc);
		file << this->credentials.login << '\n' << sha256(this->credentials.password) << endl;
		file.close();
		userMenu(admins);
	}
}

void Admin::userMenu(vector<Credentials>& admins) {
	int8_t row = 0;
	int8_t colNum = 7;
	Catalog catalog;
	Credentials::currentUser = this->credentials.login;
	while (true) {
		system("cls");
		drawMenu({ L" Изменить каталог", L"\n\n Изменить пароль",L"\n\n Удалить аккаунт",
			L"\n\n Назначить нового администратора", L"\n\n Удалить пользователя", L"\n\n Добавить нового пользователя", L"\n\n Назад" }, row);

		char a = getCharCode();

		if (a == VK_UP) row = (row + colNum - 1) % colNum;
		else if (a == VK_DOWN) row = (row + 1) % colNum;
		else if (a == 13) {
			if (row == 0) catalog.changeCatalog();
			else if (row == 1) changePassword(admins, "data/admins.txt");
			else if (row == 2) { deleteAccount(admins, "data/admins.txt"); break; }
			else if (row == 3) addAdmin();
			else if (row == 4) deleteUser();
			else if (row == 5) addUser();
			else if (row == 6) {
				system("cls");
				break;
			}
		}
	}
}

void Admin::addAdmin() {
	string str = getString(L"Введите имя пользователя. Введите exit для выхода");
	if (str == "exit") return;

	vector<Credentials> users;
	copyFile(users, "data/users.txt");
	auto it = users.begin();

	for (; it != users.end(); ++it)
		if (it->login == str)
			break;

	if (it != users.end()) {
		ofstream file("data/admins.txt", ios::app);
		file << it->login << '\n' << it->password << endl;

		users.erase(it);
		rewriteFile(users, "data/users.txt");

		getCharacter(L"Пользователь повышен. Для возвращения в меню нажмите любую клавишу");
		return;
	}
	else getCharacter(L"Пользователь не найден. Для возвращения в меню нажмите любую клавишу");
}

void Admin::deleteUser() {
	string str = getString(L"Введите имя пользователя. Введите exit для выхода");
	if (str == "exit") return;
	vector<Credentials> users;

	copyFile(users, "data/users.txt");
	auto it = users.begin();

	for (; it != users.end(); ++it)
		if (it->login == str)
			break;

	if (it != users.end()) {
		users.erase(it);
		rewriteFile(users, "data/users.txt");
		getCharacter(L"Пользователь удален. Для возвращения в меню нажмите любую клавишу");
		return;
	}
	else getCharacter(L"Пользователь не найден. Для возвращения в меню нажмите любую клавишу");
}

void Admin::addUser() {
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

		ifstream file("data/admins.txt");
		while (file) {
			string tmp;
			getline(file, tmp);
			if (tmp == user.login) {
				haveAccess = false;
				break;
			}
			getline(file, tmp);
		}
		file.close();
		file.open("data/users.txt");
		while (file) {
			string tmp;
			getline(file, tmp);
			if (tmp == user.login) {
				haveAccess = false;
				break;
			}
			getline(file, tmp);
		}

	} while (!haveAccess);

	if (!leave) {
		ofstream file("data/users.txt", ios::app);
		file << user.login << '\n' << sha256(user.password) << endl;
		getCharacter(L"Пользователь успешно добавлен. Для продолжения нажмите любую клавишу");
	}
}

