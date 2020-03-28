#include "Menus.h"

using namespace std;

vector<Credentials> users;
Catalog catalog;
User user;

void userHaveAccount()
{
	int8_t row = 0;
	copyUserFile();
	while (true)
	{
		system("cls");
		cout << "У вас есть аккаунт?\n\n";
		drawMenu({ "  Да", "\n\n  Нет","\n\n  Назад",}, row);

		char a = ' ';
		a = _getch();
		if (a == 72) row = (row + 2) % 3;
		else if (a == 80) row = (row + 1) % 3;
		else if (a == 13)
		{
			if (row == 0) user.enterAccount();
			else if (row == 1) user.createAccount();
			else if (row == 2)
			{
				system("cls");
				break;
			}
			return;
		}
	}
}

void User::userMenu()
{
	int8_t row = 0;
	int8_t colNum = 4;
	while (true)
	{
		system("cls");
		drawMenu({ " Изменить каталог", "\n\n Изменить пароль","\n\n Удалить аккаунт" ,"\n\n Назад" }, row);

		char a = ' ';
		a = _getch();
		if (a == 72) row = (row + colNum - 1) % colNum;
		else if (a == 80) row = (row + 1) % colNum;
		else if (a == 13)
		{
			if (row == 0) catalog.changeCatalog();
			else if (row == 1) user.changePassword();
			else if (row == 2) { user.deleteAccount(); break; }
			else if (row == 3)
			{
				system("cls");
				break;
			}
		}
	}

}

void User::createAccount()
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
		addCredentials();
		rewriteUserFile();
		userMenu();
	}
}

void User::enterAccount()
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
		if ( find(users.begin(), users.end(), tmp) == users.end())
			haveAccess = false;

	} while (!haveAccess);

	if (!leave)
	{
		userMenu();
	}
}

void User::enterLogin(int8_t mode,bool &haveAccess, bool &leave)
{
	int8_t error;
	switch (mode)
	{
	case 0: credentials.login = (haveAccess ? getString("Введите логин. Для выхода введите exit")
												: getString("Введите заново логин. Для выхода введите exit"));
		break;
	case 1: credentials.login =  getString("Неправильный логин и/или пароль. Повторите ввод. Для выхода введите exit"); break;
	case 2: credentials.login =  getString("Аккаунт с таким логином уже существует. Повторите ввод. Для выхода введите exit"); break;
	}

	do
	{
		error = 0;

		if (credentials.login == "exit")
		{
			leave = true;
			break;
		}

		if (credentials.login.size() < 4)
		{
			error = 1;
			credentials.login = getString("Введите логин из 4 или более символов. Для выхода введите exit");
		}

		for (int i = 0; i < credentials.login.size(); ++i)
			if (!isalnum(credentials.login[i]))
			{
				if (i == 0 && credentials.login[0] == ' ')
				{
					error = 2;
					credentials.login = getString("Логин содержит недопустимые символы. Для выхода введите exit");
					break;
				}
			}

	} while (error);
}

void User::enterPassword(bool& leave)
{
	int8_t error;

	credentials.password = getPassword("Введите пароль. Для выхода введите exit");

	do{
		error = 0;

		if (credentials.password == "exit")
			leave = true;

		for (auto a : credentials.password)
			if (!(isalnum(a) || a == '_'))
			{
				error = 1;
				credentials.password = getPassword("Пароль содержит недопустимые символы. Для выхода введите exit");
				break;
			}

		if (credentials.password.size() < 8)
		{
			error = 2;
			credentials.password = getPassword("Пароль должен содержать минимум 8 символов. Для выхода введите exit");
		}

	} while (error);
}

void rewriteUserFile()
{
	ofstream file("users.txt", ios::trunc);

	for (auto i : users)
		file << i.login << endl << i.password << endl;

	file.close();
}

void rewriteCatalogFile()
{
	ofstream file("catalog.txt", ios::trunc);
	for (auto i : catalog.cars)
		file << i.brand << endl << i.model << endl << i.color << endl << i.price << endl
		<< i.date.day << ' ' << i.date.month << ' ' << i.date.year << endl;

	file.close();
}

void copyUserFile()
{
	ifstream file("users.txt");
	while (file)
	{
		Credentials temp;
		getline(file, temp.login);
		getline(file, temp.password);
		users.push_back(temp);
	}
	if (!users.empty()) users.erase(users.end() - 1);
	file.close();

}

void copyCatalogFile()
{
	ifstream file("catalog.txt");
	if (!catalog.cars.empty()) catalog.cars.erase(catalog.cars.begin(), catalog.cars.end());
	while (file)
	{
		Car temp;
		getline(file, temp.brand);
		if (temp.brand.empty()) getline(file, temp.brand);
		getline(file, temp.model);
		getline(file, temp.color);
		file >> temp.price >> temp.date.day >> temp.date.month >> temp.date.year;
		catalog.cars.push_back(temp);
	}
	if (!catalog.cars.empty()) catalog.cars.erase(catalog.cars.end() - 1);
	file.close();
}


void User::addCredentials()
{
	Credentials tmp(user.credentials.login, sha256(user.credentials.password));
	users.push_back(tmp);
}


int User::checkPasswords()
{
	string pass1 = getPassword("Ведите пароль от аккаунта. Для выхода введите exit");
	if (pass1 == "exit") return 2;
	string pass2 = getPassword("Ведите пароль от аккаунта еще раз. Для выхода введите exit");
	if (pass2 == "exit") return 2;

	if (pass1 != pass2 || pass1 != this->credentials.password)
	{
		getCharacter("Пароли не совпадают. Для продолжения нажмите любую клавишу");
		return 1;
	}
	return 0;
}

void Catalog::changeCatalog()
{
	int8_t row = 0;
	int8_t colNum = 4;
	copyCatalogFile();
	while (true)
	{
		system("cls");
		drawMenu({ " Просмотреть каталог", "\n\n Добавить элемент в каталог","\n\n Удалить элемент из каталога" ,"\n\n Назад" }, row);

		char a = ' ';
		a = _getch();
		if (a == 72) row = (row + colNum - 1) % colNum;
		else if (a == 80) row = (row + 1) % colNum;
		else if (a == 13)
		{
			if (row == 0) displayCatalog();
			else if (row == 1) addElement();
			else if (row == 2) deleteElement();
			else if (row == 3)
			{
				system("cls");
				break;
			}
		}
	}
}

void User::deleteAccount()
{
	do {
		int i = checkPasswords();

		if (!i)
		{
			Credentials tmp(this->credentials.login, sha256(this->credentials.password));

			auto ptr = find(users.begin(), users.end(), tmp);
			if (ptr != users.end())
			{
				users.erase(ptr);
				rewriteUserFile();
				return;
			}
		}
		else if (i == 2) break;

	} while (true);
}

void User::changePassword()
{
	do {
		int i = checkPasswords();

		if (!i)
		{
			Credentials tmp(this->credentials.login, sha256(this->credentials.password));

			auto ptr = find(users.begin(), users.end(), tmp);
			if (ptr != users.end())
			{
				bool leave = false;
				enterPassword(leave);
				if (leave) return;

				ptr->password = sha256(credentials.password);
				rewriteUserFile();
				return;
			}
		}
		else if (i == 2) break;
	} while (true);
}

void Catalog::displayCatalog()
{

}

void Catalog::addElement()
{
	Car tmp;
	tmp.getBrand();
	tmp.getModel();
	tmp.getColor();
	tmp.date.getDate();
	tmp.price = to_string(getInt("Введите цену автомобиля"));
	cars.push_back(tmp);
	rewriteCatalogFile();
}

void Catalog::deleteElement()
{

}