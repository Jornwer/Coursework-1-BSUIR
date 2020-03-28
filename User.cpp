#include "Menus.h"

using namespace std;

void userHaveAccount()
{
	int8_t row = 0;
	User user;
	vector<Credentials> users;
	copyUserFile(users);
	while (true)
	{
		system("cls");
		
		if (users.empty()) drawMenu({ "  Создать аккаунт","\n\n  Назад", }, row);
		else
		{
			cout << "У вас есть аккаунт?\n\n";
			drawMenu({ "  Да", "\n\n  Нет","\n\n  Назад", }, row);
		}

		char a = getCharCode();

		if (!users.empty())
		{
			if (a == VK_UP) row = (row + 2) % 3;
			else if (a == VK_DOWN) row = (row + 1) % 3;
			else if (a == 13)
			{
				if (row == 0) user.enterAccount(users);
				else if (row == 1) user.createAccount(users);
				else if (row == 2)
				{
					system("cls");
					break;
				}
				return;
			}
		}
		else
		{
			if (a == VK_UP) row = (row + 1) % 2;
			else if (a == VK_DOWN) row = (row + 1) % 2;
			else if (a == 13)
			{
				if (row == 0) user.createAccount(users);
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

void User::userMenu(vector<Credentials>& users)
{
	int8_t row = 0;
	int8_t colNum = 4;
	Catalog catalog;
	while (true)
	{
		system("cls");
		drawMenu({ " Изменить каталог", "\n\n Изменить пароль","\n\n Удалить аккаунт" ,"\n\n Назад" }, row);

		char a = getCharCode();

		if (a == VK_UP) row = (row + colNum - 1) % colNum;
		else if (a == VK_DOWN) row = (row + 1) % colNum;
		else if (a == 13)
		{
			if (row == 0) catalog.changeCatalog();
			else if (row == 1) changePassword(users);
			else if (row == 2) { deleteAccount(users); break; }
			else if (row == 3)
			{
				system("cls");
				break;
			}
		}
	}

}

void User::createAccount(vector<Credentials>& users)
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
		addCredentials(users);
		rewriteUserFile(users);
		userMenu(users);
	}
}

void User::enterAccount(vector<Credentials>& users)
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
		userMenu(users);
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

void rewriteUserFile(vector<Credentials>& users)
{
	ofstream file("users.txt", ios::trunc);

	for (auto i : users)
		file << i.login << endl << i.password << endl;

	file.close();
}

void rewriteCatalogFile(Catalog& catalog)
{
	ofstream file("catalog.txt", ios::trunc);
	for (auto i : catalog.cars)
		file << i.brand << endl << i.model << endl << i.color << endl << i.price << endl
		<< i.date.day << ' ' << i.date.month << ' ' << i.date.year << endl;

	file.close();
}

void copyUserFile(vector<Credentials>& users)
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

void copyCatalogFile(Catalog& catalog)
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

void User::addCredentials(vector<Credentials>& users)
{
	Credentials tmp(this->credentials.login, sha256(this->credentials.password));
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
	copyCatalogFile(*this);
	while (true)
	{
		system("cls");
		drawMenu({ " Просмотреть каталог", "\n\n Добавить элемент в каталог","\n\n Удалить элемент из каталога" ,"\n\n Назад" }, row);

		char a = getCharCode();

		if (a == VK_UP) row = (row + colNum - 1) % colNum;
		else if (a == VK_DOWN) row = (row + 1) % colNum;
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

void User::deleteAccount(vector<Credentials>& users)
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
				rewriteUserFile(users);
				return;
			}
		}
		else if (i == 2) break;

	} while (true);
}

void User::changePassword(vector<Credentials>& users)
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
				rewriteUserFile(users);
				return;
			}
		}
		else if (i == 2) break;
	} while (true);
}

void Catalog::displayCatalog()
{
	
	if (this->cars.empty()) {getCharacter("Каталог пуст. Для возвращения нажмите любую кнопку"); return;}
	int pages = ceil((double)this->cars.size() / 7.0), page = 0;
	while (true)
	{
		system("cls");
		cout << "Страница " << page+1 << " из " << pages <<
			". Для перемещения страниц испольуйте стрелки вправо/влево. Для выхода нажмите е(Е)" << endl;
		for (int i = page * 7; i < (page + 1) * 7 && i < this->cars.size(); ++i)
			displayElement(this->cars[i]);

		char a = getCharCode();

		if (a == VK_LEFT) page = (page + pages - 1) % pages;
		else if (a == VK_RIGHT) page = (page + 1) % pages;
		else if (a == 'e' || a == 'E' || a == 'е' || a == 'Е') //русские и английские е
		{
			break;
		}
	}
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
	rewriteCatalogFile(*this);
}

void Catalog::deleteElement()
{
	if (this->cars.empty()) { getCharacter("Каталог пуст. Для возвращения нажмите любую кнопку"); return; }
	int pages = ceil((double)this->cars.size() / 7.0), page = 0, pos = 0, elInPage = 7;
	while (true)
	{
		system("cls");
		cout << "Страница " << page + 1 << " из " << pages <<
			". Для перемещения страниц испольуйте стрелки вправо/влево. Для выхода нажмите е(Е)" << endl <<
			"Для удаления элемента нажмите Enter" << endl << endl;

		for (int i = page * 7; i < (page + 1) * 7 && i < this->cars.size(); ++i)
		{
			if (i % 7 == pos) cout << endl << string(40, '/');
			displayElement(this->cars[i]);
			if (i % 7 == pos) cout << string(40, '\\') << endl;
		}

		char a = getCharCode();

		if (a == VK_LEFT) page = (page + pages - 1) % pages;
		else if (a == VK_RIGHT) page = (page + 1) % pages;
		else if (a == VK_UP) pos = (pos + elInPage - 1) % elInPage;
		else if (a == VK_DOWN) pos = (pos + 1) % elInPage;
		else if (a == 13) approveDeletion(page, pos);
		else if (a == 'e' || a == 'E' || a == 'е' || a == 'Е') //русские и английские е
		{
			return;
		}
		if (page == pages - 1) { elInPage = this->cars.size() - page * 7; pos = (pos >= elInPage ? 0 : pos);}
		else elInPage = 7;
	}
}

void Catalog::displayElement(Car car)
{
	cout << endl << "Марка: " << car.brand << "     Модель: " << car.model << "    Цвет: " << car.color << endl;
	cout << "Дата продажи: " << car.date << "    Цена: " << car.price << endl;
}

void Catalog::approveDeletion(int page, int pos)
{
	string temp;
	system("cls");
	temp = getString("Введите \"yes\" для удаления элемента");
	if (temp == "yes")
	{
		this->cars.erase(this->cars.begin() + pos + page * 7);
		rewriteCatalogFile(*this);
	}
}
