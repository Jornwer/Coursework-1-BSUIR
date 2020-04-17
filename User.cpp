#include "Menus.h"

using namespace std;

void userHaveAccount()
{
	int8_t row = 0;
	User user;
	vector<Credentials> users;
	copyFile(users, "users.txt");
	while (true)
	{
		system("cls");
		
		if (users.empty()) 
		{
			getCharacter(L"Список пользователей пуст. Для продолжения нажмите любую кнопку");
			return;
		}
		else
		{
			drawMenu({L"Войти в аккаунт   ", L"\n\nНазад   "}, row);

			char a = getCharCode();

			if (a == VK_UP || a == VK_DOWN) row = (row + 1) % 2;
			else if (a == 13)
			{
				if (row == 0) user.enterAccount(users);
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
		drawMenu({ L" Изменить каталог", L"\n\n Изменить пароль",L"\n\n Удалить аккаунт" ,L"\n\n Назад" }, row);

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

void User::enterAccount(vector<Credentials>& users)
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
		if ( find(users.begin(), users.end(), tmp) == users.end())
			haveAccess = false;

	} while (!haveAccess);

	if (!leave)
	{
		userMenu(users);
	}
}

void rewriteFile(vector<Credentials>& users, string path)
{
	ofstream file(path, ios::trunc);

	for (auto i : users)
		file << i.login << endl << i.password << endl;

	file.close();
}

void rewriteCatalogFile(Catalog& catalog)
{
	ofstream file("catalog.txt", ios::trunc);
	for (auto i : catalog.cars)
		file << i.brand << endl << i.model << endl << i.color << endl << i.price << endl
		<< (i.date.day.size() == 1 ? '0' + i.date.day : i.date.day) << ' ' << (i.date.month.size() == 1 ? '0' + i.date.month : i.date.month) 
					<< ' ' << (i.date.year.size() == 1 ? '0' + i.date.year : i.date.year) << endl;

	file.close();
}

void copyFile(vector<Credentials>& users, string path)
{
	ifstream file(path);
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

int User::checkPasswords()
{
	string pass1 = getPassword(L"Ведите пароль от аккаунта. Для выхода введите exit");
	if (pass1 == "exit") return 2;
	string pass2 = getPassword(L"Ведите пароль от аккаунта еще раз. Для выхода введите exit");
	if (pass2 == "exit") return 2;

	if (pass1 != pass2 || pass1 != this->credentials.password)
	{
		getCharacter(L"Пароли не совпадают. Для продолжения нажмите любую клавишу");
		return 1;
	}
	return 0;
}

void Catalog::changeCatalog()
{
	int8_t row = 0;
	int8_t colNum = 5;
	copyCatalogFile(*this);
	while (true)
	{
		system("cls");
		drawMenu({ L" Просмотреть каталог", L"\n\n Добавить элемент в каталог",
						L"\n\n Удалить элемент из каталога",L"\n\n Поиск в каталоге" ,L"\n\n Назад" }, row);

		char a = getCharCode();

		if (a == VK_UP) row = (row + colNum - 1) % colNum;
		else if (a == VK_DOWN) row = (row + 1) % colNum;
		else if (a == 13)
		{
			if (row == 0) displayCatalog();
			else if (row == 1) addElement();
			else if (row == 2) deleteElement();
			else if (row == 3) searchInCatalog();
			else if (row == 4)
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
				rewriteFile(users, "users.txt");
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
				this->credentials.password = enterPassword(leave);
				if (leave) return;

				ptr->password = sha256(credentials.password);
				rewriteFile(users, "users.txt");
				return;
			}
		}
		else if (i == 2) break;
	} while (true);
}

void Catalog::displayCatalog()
{
	
	if (this->cars.empty()) {getCharacter(L"Каталог пуст. Для возвращения нажмите любую кнопку"); return;}
	int pages = ceil((double)this->cars.size() / 7.0), page = 0;
	while (true)
	{
		system("cls");
		wcout << L"Страница " << page+1 << L" из " << pages <<
			L". Для перемещения страниц испольуйте стрелки вправо/влево. Для выхода нажмите е(Е)" << endl;
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
	Car car;
	string date;
	int8_t row = 0;

	while (true)
	{
		system("cls");
		wcout << L"Марка   :";
		cout << car.brand << string(16 - car.brand.size(), ' ') << '|' << (!row ? " <--\n" : "\n");

		wcout << L"Модель  :";
		cout << car.model << string(16 - car.model.size(), ' ') << '|' << (row == 1 ? " <--\n" : "\n");

		wcout << L"Цвет    :";
		cout << car.color << string(16 - car.color.size(), ' ') << '|' << (row == 2 ? " <--\n" : "\n");

		wcout << L"Цена    :";
		cout << car.price << string(16 - car.price.size(), ' ') << '|' << (row == 3 ? " <--\n" : "\n");

		wcout << L"Дата    :"; displayDate(date);
		cout << '|' << (row == 4 ? " <--\n" : "\n");

		wcout << L"Добавить    " << (row == 5 ? L" <--\n" : L"\n") << L"Назад    " << (row == 6 ? L" <--\n" : L"\n\n");

		int8_t a = _getch();

		if (a == 'а')
		{
			if (GetAsyncKeyState('F') != -32767)
			{
				a = _getch();
				if (a == 'H') row = (row + 6) % 7;
				else if (a == 'P') row = (row + 1) % 7;
			}
		}
		else
		{
			if (a == 13 && row == 5)
			{
				if (carCorrect(car, date))
				{
					ofstream file("catalog.txt", ios::app);

					file << car.brand << endl << car.model << endl << car.color << endl << car.price << endl
					<< date[0] << date[1] << ' ' << date[2] << date[3]
					<< ' ' << date[4] << date[5] << date[6] << date[7] << endl;

					file.close();

					getCharacter(L"Машина успешно добавлена. Для продолжения нажмите любую клавишу");
				}
				continue;
			}
			else if (a == 13 && row == 6) return;
			else if ((a >= 'a' && a <= 'z') || (a >= 'A' && a <= 'Z') || (a >= '0' && a <= '9') && row < 3)
			{
				if (!row && car.brand.size() < 16) car.brand += a;
				else if (row == 1 && car.model.size() < 16) car.model += a;
				else if (row == 2 && car.color.size() < 16) car.color += a;
			}
			else if ((a >= '0' && a <= '9'))
			{
				if (row == 3 && car.price.size() < 16) car.price += a;
				else if (row == 4 && date.size() < 8) date += a;
			}
			else if (a == 8 && row < 5)
			{
				if (!row && !car.brand.empty()) car.brand.erase(car.brand.end() - 1);
				else if (row == 1 && !car.model.empty()) car.model.erase(car.model.end() - 1);
				else if (row == 2 && !car.color.empty()) car.color.erase(car.color.end() - 1);
				else if (row == 3 && !car.price.empty()) car.price.erase(car.price.end() - 1);
				else if (row == 4 && !date.empty()) date.erase(date.end() - 1);
			}
		}
	}
}

void Catalog::deleteElement()
{
	if (this->cars.empty()) { getCharacter(L"Каталог пуст. Для возвращения нажмите любую кнопку"); return; }
	int pages = ceil((double)this->cars.size() / 7.0), page = 0, pos = 0, elInPage = 7;
	while (true)
	{
		system("cls");
		wcout << L"Страница " << page + 1 << " из " << pages <<
			L". Для перемещения страниц испольуйте стрелки вправо/влево. Для выхода нажмите е(Е)" << endl <<
			L"Для удаления элемента нажмите Enter" << endl << endl;

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
	wcout << endl << L"Марка: ";
	cout << car.brand;
	wcout << L"     Модель: ";
	cout << car.model;
	wcout << L"    Цвет: ";
	cout << car.color << endl;
	wcout << L"Дата продажи: ";
	cout << car.date.day << '.' << car.date.month << '.' << car.date.year;
	wcout << L"    Цена: ";
	cout << car.price << endl;
}

void Catalog::approveDeletion(int page, int pos)
{
	string temp;
	system("cls");
	temp = getString(L"Введите \"yes\" для удаления элемента");
	if (temp == "yes")
	{
		this->cars.erase(this->cars.begin() + pos + page * 7);
		rewriteCatalogFile(*this);
	}
}

void Catalog::searchInCatalog()
{
	string brand = "", model = "", color = "", priceFrom = "", priceTo = "", dateFrom = "", dateTo = "";
	int8_t row = 0;
	while (true)
	{
		system("cls");
		wcout << L"Марка   :";
		cout  << brand << string(16 - brand.size(), ' ') << '|' << (!row ? " <--\n" : "\n");

		wcout << L"Модель  :";
		cout  << model << string(16 - model.size(), ' ') << '|' << (row == 1 ? " <--\n" : "\n");

		wcout << L"Цвет    :";
		cout  << color << string(16 - color.size(), ' ')<< '|' << (row == 2 ? " <--\n" : "\n");

		wcout << L"Цена с  :";
		cout  << priceFrom << string(16 - priceFrom.size(), ' ')<< '|' << (row == 3 ? " <--\n" : "\n");

		wcout << L"Цена до :";
		cout  << priceTo << string(16 - priceTo.size(), ' ') << '|' << (row == 4 ? " <--\n" : "\n");

		wcout << L"С       :"; displayDate(dateFrom);
		cout  << '|' << (row == 5 ? " <--\n" : "\n");

		wcout << L"До      :"; displayDate(dateTo);
		cout  << '|' << (row == 6 ? " <--\n" : "\n");

		wcout << L"Найти    " << (row == 7 ? L" <--\n" : L"\n") << L"Назад    " << (row == 8 ? L" <--\n" : L"\n\n");

		int8_t a = _getch();

		if (a == 'а')
		{
			if (GetAsyncKeyState('F') != -32767)
			{
				a = _getch();
				if (a == 'H') row = (row + 8) % 9;
				else if (a == 'P') row = (row + 1) % 9;
			}
		}
		else
		{
			if (a == 13 && row == 7) displaySearch(brand, model, color, priceFrom, priceTo, dateFrom, dateTo);
			else if (a == 13 && row == 8) return;
			else if ((a >= 'a' && a <= 'z') || (a >= 'A' && a <= 'Z') || (a >= '0' && a <= '9') && row < 5 )
			{
				if (!row && brand.size() < 16) brand += a;
				else if (row == 1 && model.size() < 16) model += a;
				else if (row == 2 && color.size() < 16) color += a;
				else if (row == 3 && priceFrom.size() < 11) priceFrom += a;
				else if (row == 4 && priceTo.size() < 11) priceTo += a;
			}
			else if ((a >= '0' && a <= '9'))
			{
				if (row == 5 && dateFrom.size() < 8) dateFrom += a;
				else if (row == 6 && dateTo.size() < 8) dateTo += a;
			}
			else if (a == 8 && row < 7)
			{
				if (!row && !brand.empty()) brand.erase(brand.end() - 1);
				else if (row == 1 && !model.empty()) model.erase(model.end() - 1);
				else if (row == 2 && !color.empty()) color.erase(color.end() - 1);
				else if (row == 3 && !priceFrom.empty()) priceFrom.erase(priceFrom.end() - 1);
				else if (row == 4 && !priceTo.empty()) priceTo.erase(priceTo.end() - 1);
				else if (row == 5 && !dateFrom.empty()) dateFrom.erase(dateFrom.end() - 1);
				else if (row == 6 && !dateTo.empty()) dateTo.erase(dateTo.end() - 1);
			}
		}
	}
}

void Catalog::displaySearch(string brand, string model, string color, string priceFrom, string priceTo, string dateFrom, string dateTo)
{
	if ((dateFrom.size() != 8 && !dateFrom.empty()) || (dateTo.size() != 8 && !dateTo.empty()))
	{
		getCharacter(L"Дата введена неправильно. Для возвращения нажмите любую кнопку");
		return;
	}

	vector<Car> cat;

	for (auto i : this->cars)
	{
		int8_t j;
		if (brand.size() > i.brand.size()) continue;
		else
		{
			for (j = 0; j < brand.size(); ++j)
				if (brand[j] != i.brand[j]) goto again;
		}
		if (model.size() > i.model.size()) continue;
		else
		{
			for (j = 0; j < model.size(); ++j)
				if (model[j] != i.model[j]) goto again;
		}
		if (color.size() > i.color.size()) continue;
		else
		{
			for (j = 0; j < color.size(); ++j)
				if (color[j] != i.color[j]) goto again;
		}
		if (!(priceTo.empty() && priceFrom.empty()))
		{
			if (priceTo.empty())
			{
				if (stringToInt(i.price) < stringToInt(priceFrom)) continue;
			}
			if (priceFrom.empty())
			{
				if (stringToInt(i.price) > stringToInt(priceTo)) continue;
			}
			if (!priceFrom.empty() && !priceTo.empty())
				if (stringToInt(priceFrom) > stringToInt(i.price) || stringToInt(priceTo) < stringToInt(i.price)) continue;
		}
		if (!(dateTo.empty() && dateFrom.empty()))
		{
			if (dateTo.empty())
			{
				Date from(dateFrom), date(i.date.day + i.date.month + i.date.year);
				if (date < from) continue;
			}
			if (dateFrom.empty())
			{
				Date to(dateTo), date(i.date.day + i.date.month + i.date.year);
				if (date > to) continue;
			}
			if (!(dateFrom.empty() || dateTo.empty()))
			{
				Date from(dateFrom), to(dateTo), date(i.date.day + i.date.month + i.date.year);
				if (from > date || to < date) continue;
			}
		}
		cat.push_back(i);
	again:
		continue;
	}
				

	if (cat.empty()) { getCharacter(L"В каталоге нет подходящих элементов. Для возвращения нажмите любую кнопку"); return; }
	int pages = ceil((double)cat.size() / 7.0), page = 0;
	while (true)
	{
		system("cls");
		wcout << L"Страница " << page + 1 << L" из " << pages <<
			L". Для перемещения страниц испольуйте стрелки вправо/влево. Для выхода нажмите е(Е)" << endl;
		for (int i = page * 7; i < (page + 1) * 7 && i < cat.size(); ++i)
			displayElement(cat[i]);

		char a = getCharCode();

		if (a == VK_LEFT) page = (page + pages - 1) % pages;
		else if (a == VK_RIGHT) page = (page + 1) % pages;
		else if (a == 'e' || a == 'E' || a == 'е' || a == 'Е') //русские и английские е
		{
			break;
		}
	}
}
