#include "Menus.h"

using namespace std;

vector<Credentials> users;
User user;
Catalog catalog;

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

string getString(string msg)
{
	system("cls");
	string tmp;
	cout << msg << endl;
	getline(cin, tmp);
	return tmp;
}

string getPassword(string msg)
{
	string password = "";
	char a = '1';

	system("cls");
	cout << msg << endl;

	while ((a = _getch()) != 13)
	{
		if ((int)a == 8)
		{
			if (password.empty()) continue;

			password.erase(password.size() - 1, password.size());

			cout << '\b' << ' ' << '\b';
		}
		else
		{
			password += a;
			cout << '*';
		}
	}
	return password;
}

void getCharacter(string msg)
{
	system("cls");
	cout << msg << endl;
	char a = _getch();
}


void rewriteUserFile()
{
	ofstream file("users.txt", ios::trunc);

	for(auto i : users)
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
	if (!users.empty()) users.erase(users.end()-1);
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


bool Credentials::operator==(Credentials a)
{
	return (this->login == a.login && this->password == a.password) ? true : false;
}

void User::addCredentials()
{
	Credentials tmp(user.credentials.login, sha256(user.credentials.password));
	users.push_back(tmp);
}


Credentials::Credentials(string login, string password)
{
	this->login = login;
	this->password = password;
}

Credentials::Credentials(){}


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

void Car::getBrand()
{
	string tmp;
	do
	{
		bool isOk = true;
		tmp = getString("Введте марку автомобиля");
		for (auto i : tmp)
			if (!isalpha(i)) 
			{
				getCharacter("Название должно состоять из английских букв. Для повторного ввода нажмите любую клавишу");
				isOk = false;
				break;
			}
		if (isOk && tmp.size())
		{
			brand = tmp;
			return;
		}
	} while (true);
}

void Car::getModel()
{
	string tmp;
	do
	{
		bool isOk = true;
		tmp = getString("Введте модель автомобиля");
		for (auto i : tmp)
			if (!(isalnum(i) || i == ' '))
			{
				getCharacter("Название должно состоять из английских букв и цифр. Для повторного ввода нажмите любую клавишу");
				isOk = false;
				break;
			}
		if (isOk && tmp.size())
		{
			model = tmp;
			return;
		}
	} while (true);
}

void Car::getColor()
{
	string tmp;
	do
	{
		bool isOk = true;
		tmp = getString("Введите цвет автомобиля");
		for (auto i : tmp)
			//if ((i >= 'а' && i <= 'я') || (i >= 'А' && i <= 'Я'))
			if(!(i >= -64 && i < 0))
			{
				getCharacter("Цвет должен состоять из русских букв. Для повторного ввода нажмите любую клавишу   " + to_string((int)i));
				isOk = false;
				break;
			}
		if (isOk && tmp.size())
		{
			color = tmp;
			return;
		}
	} while (true);
}

bool Car::operator==(Car a)
{
	if (a.brand == brand && a.color == color && a.date == date && a.model == model && a.price == price) return true;
	else return false;
}

bool Date::operator==(Date a)
{
	if (a.day == day && a.year == year && a.month == month) return true;
	else return false;
}

void Date::getDate()
{
	int iYear = enterYear(), iMonth = enterMonth();
	year = to_string(iYear);
	month = to_string(iMonth);
	day = to_string(enterDay(iMonth, iYear));
}

int8_t Date::enterDay(int8_t month, int16_t year)
{
	int8_t tmp;
	do
	{
		tmp = getInt("Введите день продажи");
		if (dayCorrect(month, year, tmp)) return tmp;
		else getCharacter("Введенного дня не существует. Для повторного ввода дня нажмите любую клавишу");
	} while (true);
}

int8_t Date::enterMonth()
{
	int8_t tmp;
	do
	{
		tmp = getInt("Введите месяц продажи");
		if (tmp > 0 && tmp < 13) return tmp;
		else getCharacter("Месяц должен быть от 1 до 12. Для повторного ввода нажмите любую клавишу");
	} while (true);
}

int16_t Date::enterYear()
{
	int16_t tmp;
	do
	{
		tmp = getInt("Введите год продажи");
		if (tmp >= 1000 && tmp <= 9999) return tmp;
		else getCharacter("Год должен состоять из 4 цифр. Для повторного ввода нажмите любую клавишу");
	} while (true);
}

bool Date::dayCorrect(int8_t month, int16_t year, int8_t day)
{
	if (day > 31 || !day) return false;
	if (day < 29) return true;
	if (month == 2)
	{
		if (year % 4) return false;
		if (!year % 100)
			if (year % 400) return false;
		return true;
	}
	if (month <= 7)
	{
		if (month % 2) return true;
		else if (day != 31) return true;
		else return false;
	}
	else
	{
		if (!month % 2) return true;
		else if (day != 31) return true;
		else return false;
	}
}

int getInt(string msg)
{
	int a;
	system("cls");
	cout << msg << endl;
	while (true) {
		cin >> a;
		if (cin.peek() == '\n') {
			cin.get();
			return a;
		}
		else {
			system("cls");
			cout << "Повторите ввод (ожидается целое число):" << endl;
			cin.clear();
			while (cin.get() != '\n') {}
		}
	}
}
