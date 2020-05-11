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
	Credentials::currentUser = this->credentials.login;
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
			else if (row == 1) changePassword(users, "users.txt");
			else if (row == 2) { deleteAccount(users, "users.txt"); break; }
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
		file << i.login << '\n' << i.password << '\n';
	file << flush;

	file.close();
}

void rewriteCatalogFile(Catalog& catalog)
{
	ofstream file("catalog.txt", ios::trunc);
	for (auto i : catalog.deals)
		file << i.car.brand << ' ' << i.car.model << ' ' << i.car.color << ' ' << i.car.price << ' '
			<< i.date.day << ' ' << i.date.month << ' ' << i.date.year << ' '
			<< i.buyerName << ' ' << i.buyerSurname << ' ' << i.seller << '\n';
	file << flush;

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

bool copyCatalogFile(Catalog& catalog)
{
	ifstream file("catalog.txt");
	if (!catalog.deals.empty()) catalog.deals.erase(catalog.deals.begin(), catalog.deals.end());
	while (file)
	{
		Deal temp;

		file >> temp.car.brand >> temp.car.model >> temp.car.color >> temp.car.price
			>> temp.date.day >> temp.date.month >> temp.date.year
			>> temp.buyerName >> temp.buyerSurname >> temp.seller;

		if (dealCorrect(temp)) 
			catalog.deals.push_back(temp);
		else
		{
			getCharacter(L"Файл каталога поврежден. Для возвращения в меню нажмите любую клавишу");
			return true;
		}
	}
	if (!catalog.deals.empty()) catalog.deals.erase(catalog.deals.end() - 1);
	file.close();
	return false;
}

int8_t User::checkPasswords()
{
	string pass1 = getPassword(L"Введите пароль от аккаунта. Для выхода введите exit");
	if (pass1 == "exit") return 2;
	string pass2 = getPassword(L"Введите пароль от аккаунта еще раз. Для выхода введите exit");
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
	int8_t colNum = 7;
	if(copyCatalogFile(*this)) return;
	while (true)
	{
		system("cls");
		drawMenu({ L" Просмотреть каталог", L"\n\n Добавить элемент в каталог", L"\n\n Удалить элемент из каталога",
					L"\n\n Поиск в каталоге", L"\n\n Изменить элемент", L"\n\n Показать самые продаваемые марки", L"\n\n Назад" }, row);

		char a = getCharCode();

		if (a == VK_UP) row = (row + colNum - 1) % colNum;
		else if (a == VK_DOWN) row = (row + 1) % colNum;
		else if (a == 13)
		{
			if (row == 0) displayCatalog();
			else if (row == 1)
			{
				Deal deal = Deal();
				enterElement(deal, string());
			}
			else if (row == 2) modifyElement(&Catalog::approveDeletion);
			else if (row == 3) searchInCatalog();
			else if (row == 4) modifyElement(&Catalog::changeElement);
			else if (row == 5) showBestBrands();
			else if (row == 6)
			{
				system("cls");
				break;
			}
		}
	}
}

void User::deleteAccount(vector<Credentials>& users, string path)
{
	do {
		int8_t i = checkPasswords();

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

void User::changePassword(vector<Credentials>& users, string path)
{
	do {
		int8_t i = checkPasswords();

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
				rewriteFile(users, path);
				return;
			}
		}
		else if (i == 2) break;
	} while (true);
}

void Catalog::displayCatalog()
{
	
	if (this->deals.empty()) {getCharacter(L"Каталог пуст. Для возвращения нажмите любую кнопку"); return;}
	int pages = ceil((double)this->deals.size() / 6.0), page = 0;
	while (true)
	{
		system("cls");
		wcout << L"Страница " << page+1 << L" из " << pages <<
			L". Для перемещения страниц испольуйте стрелки вправо/влево. Для выхода нажмите е(Е)" << L'\n';
		for (int i = page * 6; i < (page + 1) * 6 && i < this->deals.size(); ++i)
			this->deals[i].displayElement();

		char a = getCharCode();

		if (a == VK_LEFT) page = (page + pages - 1) % pages;
		else if (a == VK_RIGHT) page = (page + 1) % pages;
		else if (a == 'e' || a == 'E' || a == 'е' || a == 'Е') //русские и английские е
		{
			break;
		}
	}
}

void Catalog::enterElement(Deal& deal, string date)
{
	bool dealEmpty = (deal == Deal());
	Deal save = deal;
	int8_t row = 0;
	deal.seller = Credentials::currentUser;

	while (true)
	{
		system("cls");
		wcout << L"Марка       :";
		cout << deal.car.brand << string(16 - deal.car.brand.size(), ' ') << '|' << (!row ? " <--\n" : "\n");

		wcout << L"Модель      :";
		cout << deal.car.model << string(16 - deal.car.model.size(), ' ') << '|' << (row == 1 ? " <--\n" : "\n");

		wcout << L"Цвет        :";
		cout << deal.car.color << string(16 - deal.car.color.size(), ' ') << '|' << (row == 2 ? " <--\n" : "\n");

		wcout << L"Цена        :";
		cout << deal.car.price << string(16 - deal.car.price.size(), ' ') << '|' << (row == 3 ? " <--\n" : "\n");

		wcout << L"Дата        :"; displayDate(date);
		cout << '|' << (row == 4 ? " <--\n" : "\n");

		wcout << L"Имя пок.    :";
		cout << deal.buyerName << string(16 - deal.buyerName.size(), ' ') << '|' << (row == 5 ? " <--\n" : "\n");

		wcout << L"Фамилия пок.:";
		cout << deal.buyerSurname << string(16 - deal.buyerSurname.size(), ' ') << '|' << (row == 6 ? " <--\n" : "\n");

		wcout << L"Продавец    :";
		cout << deal.seller << string(16 - deal.seller.size(), ' ') << '|' << "\n";

		wcout << L"Ввести      " << (row == 7 ? L" <--\n" : L"\n") << L"Назад    " << (row == 8 ? L" <--\n" : L"\n\n");

		uint8_t a = _getch();

		if (a == 224)
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
			if (a == 13 && row == 7)
			{
				if (dealCorrect(deal, date))
				{
					if (!dealEmpty)
					{
						deal.date = date;
						break;
					}
					ofstream file("catalog.txt", ios::app);

					file << deal.car.brand << ' ' << deal.car.model << ' ' << deal.car.color << ' ' << deal.car.price << ' '
						<< date[0] << date[1] << ' ' << date[2] << date[3]
						<< ' ' << date[4] << date[5] << date[6] << date[7] << ' '
						<< deal.buyerName << ' ' << deal.buyerSurname << ' ' << deal.seller << '\n';

					file.close();
					deal.date = Date(date);
					this->deals.push_back(deal);
					getCharacter(L"Машина успешно добавлена. Для продолжения нажмите любую клавишу");
					continue;
				}
			}
			else if (a == 13 && row == 8)
			{
				if (!dealEmpty) deal = save;
				return;
			}
			else if ((a >= '0' && a <= '9') && (row == 3 || row == 4))
			{
				if (row == 3 && deal.car.price.size() < 16) deal.car.price += a;
				else if (row == 4 && date.size() < 8) date += a;
			}
			else if ((a >= 'a' && a <= 'z') || (a >= 'A' && a <= 'Z') || (a >= '0' && a <= '9'))
			{
				if (!row && deal.car.brand.size() < 16) deal.car.brand += a;
				else if (row == 1 && deal.car.model.size() < 16) deal.car.model += a;
				else if (row == 2 && deal.car.color.size() < 16) deal.car.color += a;
				else if (row == 5 && deal.buyerName.size() < 16) deal.buyerName += a;
				else if (row == 6 && deal.buyerSurname.size() < 16) deal.buyerSurname += a;
			}
			else if (a == 8 && row < 7)
			{
				if (!row && !deal.car.brand.empty()) deal.car.brand.erase(deal.car.brand.end() - 1);
				else if (row == 1 && !deal.car.model.empty()) deal.car.model.erase(deal.car.model.end() - 1);
				else if (row == 2 && !deal.car.color.empty()) deal.car.color.erase(deal.car.color.end() - 1);
				else if (row == 3 && !deal.car.price.empty()) deal.car.price.erase(deal.car.price.end() - 1);
				else if (row == 4 && !date.empty()) date.erase(date.end() - 1);
				else if (row == 5 && !deal.buyerName.empty()) deal.buyerName.erase(deal.buyerName.end() - 1);
				else if (row == 6 && !deal.buyerSurname.empty()) deal.buyerSurname.erase(deal.buyerSurname.end() - 1);
			}
		}
	}
}

void Deal::displayElement()
{
	wcout << L'\n' << L"Марка: ";
	cout << car.brand;
	wcout << L"     Модель: ";
	cout << car.model;
	wcout << L"    Цвет: ";
	cout << car.color << '\n';
	wcout << L"Дата продажи: ";
	cout << date.day << '.' << date.month << '.' << date.year;
	wcout << L"    Цена: ";
	cout << car.price << '\n';
	wcout << L"Имя и фамилия покупателя: ";
	cout << buyerName << ' ' << buyerSurname;
	wcout << L"      Продавец: ";
	cout << seller << '\n';
}

void Catalog::approveDeletion(int &page, int &pos)
{
	string temp;
	system("cls");
	temp = getString(L"Введите \"yes\" для удаления элемента");
	if (temp == "yes")
	{
		this->deals.erase(this->deals.begin() + pos + page * 6);
		rewriteCatalogFile(*this);
	}
}

void Catalog::searchInCatalog()
{
	string brand = "", model = "", color = "", priceFrom = "", priceTo = "", dateFrom = "", dateTo = "", buyerName = "", buyerSurname= "";
	int8_t row = 0;
	while (true)
	{
		system("cls");
		wcout << L"Марка       :";
		cout  << brand << string(16 - brand.size(), ' ') << '|' << (!row ? " <--\n" : "\n");

		wcout << L"Модель      :";
		cout  << model << string(16 - model.size(), ' ') << '|' << (row == 1 ? " <--\n" : "\n");

		wcout << L"Цвет        :";
		cout  << color << string(16 - color.size(), ' ')<< '|' << (row == 2 ? " <--\n" : "\n");

		wcout << L"Цена с      :";
		cout  << priceFrom << string(16 - priceFrom.size(), ' ')<< '|' << (row == 3 ? " <--\n" : "\n");

		wcout << L"Цена до     :";
		cout  << priceTo << string(16 - priceTo.size(), ' ') << '|' << (row == 4 ? " <--\n" : "\n");

		wcout << L"С           :"; displayDate(dateFrom);
		cout  << '|' << (row == 5 ? " <--\n" : "\n");

		wcout << L"До          :"; displayDate(dateTo);
		cout  << '|' << (row == 6 ? " <--\n" : "\n");

		wcout << L"Имя пок.    :";
		cout << buyerName << string(16 - buyerName.size(), ' ') << '|' << (row == 7 ? " <--\n" : "\n");

		wcout << L"Фамилия пок.:";
		cout << buyerSurname << string(16 - buyerSurname.size(), ' ') << '|' << (row == 8 ? " <--\n" : "\n");

		wcout << L"Найти    " << (row == 9 ? L" <--\n" : L"\n") << L"Назад    " << (row == 10 ? L" <--\n" : L"\n\n");

		uint8_t a = _getch();

		if (a == 224)
		{
			if (GetAsyncKeyState('F') != -32767)
			{
				a = _getch();
				if (a == 'H') row = (row + 10) % 11;
				else if (a == 'P') row = (row + 1) % 11;
			}
		}
		else
		{
			if (a == 13 && row == 9) displaySearch(brand, model, color, priceFrom, priceTo, dateFrom, dateTo, buyerName, buyerSurname);
			else if (a == 13 && row == 10) return;
			else if ((a >= '0' && a <= '9') && row > 2 && row < 7)
			{
				if (row == 3 && priceFrom.size() < 11) priceFrom += a;
				else if (row == 4 && priceTo.size() < 11) priceTo += a;
				else if (row == 5 && dateFrom.size() < 8) dateFrom += a;
				else if (row == 6 && dateTo.size() < 8) dateTo += a;
			}
			else if ((a >= 'a' && a <= 'z') || (a >= 'A' && a <= 'Z') || (a >= '0' && a <= '9'))
			{
				if (!row && brand.size() < 16) brand += a;
				else if (row == 1 && model.size() < 16) model += a;
				else if (row == 2 && color.size() < 16) color += a;
				else if (row == 7 && buyerName.size() < 16) buyerName += a;
				else if (row == 8 && buyerSurname.size() < 16) buyerSurname += a;
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
				else if (row == 7 && !buyerName.empty()) buyerName.erase(buyerName.end() - 1);
				else if (row == 8 && !buyerSurname.empty()) buyerName.erase(buyerName.end() - 1);
			}
		}
	}
}

void Catalog::displaySearch(string brand, string model, string color, string priceFrom,
							string priceTo, string dateFrom, string dateTo, string buyerName, string buyerSurname)
{
	if ((dateFrom.size() != 8 && !dateFrom.empty()) || (dateTo.size() != 8 && !dateTo.empty()))
	{
		getCharacter(L"Дата введена неправильно. Для возвращения нажмите любую кнопку");
		return;
	}

	Catalog cat;

	for (auto i : this->deals)
	{
		if (!stringContainString(i.car.brand, brand) || !stringContainString(i.car.model, model)
			|| !stringContainString(i.car.color, color) || !stringContainString(i.buyerName, buyerName)
			|| !stringContainString(i.buyerSurname, buyerSurname)) continue;
		if (!(priceTo.empty() && priceFrom.empty()))
		{
			if (priceTo.empty())
			{
				if (stringToInt(i.car.price) < stringToInt(priceFrom)) continue;
			}
			if (priceFrom.empty())
			{
				if (stringToInt(i.car.price) > stringToInt(priceTo)) continue;
			}
			if (!priceFrom.empty() && !priceTo.empty())
				if (stringToInt(priceFrom) > stringToInt(i.car.price) || stringToInt(priceTo) < stringToInt(i.car.price)) continue;
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
		cat.deals.push_back(i);
	}
	
	if (cat.deals.empty())
	{
		getCharacter(L"Найдено 0 элементов. Для продолжения нажмите любую клавишу");
		return;
	}
	cat.displayCatalog();
}

void Catalog::changeElement(int& page, int& pos)
{
	enterElement(deals[pos + page * 6], deals[pos + page * 6].date.day + deals[pos + page * 6].date.month + deals[pos + page * 6].date.year);
	rewriteCatalogFile(*this);
}

void Catalog::modifyElement(void (Catalog::*f)(int&, int&))
{
	int pages, page = 0, pos = 0, elInPage;
	while (true)
	{
		if (this->deals.empty()) { getCharacter(L"Каталог пуст. Для возвращения нажмите любую кнопку"); return; }
		pages = ceil((double)this->deals.size() / 6.0);
		if (page == pages - 1) { elInPage = this->deals.size() - page * 6; pos = (pos >= elInPage ? 0 : pos); }
		else elInPage = 6;
		system("cls");

		wcout << L"Страница " << page + 1 << L" из " << pages <<
			L". Для перемещения страниц испольуйте стрелки вправо/влево. Для выхода нажмите е(Е)" << L'\n' <<
			L"Для выбора элемента нажмите Enter" << L'\n' << L'\n';

		for (int i = page * 6; i < (page + 1) * 6 && i < this->deals.size(); ++i)
		{
			if (i % 6 == pos) cout << string(40, '/');
			this->deals[i].displayElement();
			if (i % 6 == pos) cout << string(40, '\\');
		}

		char a = getCharCode();

		if (a == VK_LEFT) page = (page + pages - 1) % pages;
		else if (a == VK_RIGHT) page = (page + 1) % pages;
		else if (a == VK_UP) pos = (pos + elInPage - 1) % elInPage;
		else if (a == VK_DOWN) pos = (pos + 1) % elInPage;
		else if (a == 13)
		{
			int num = deals.size();
			(this->*f)(page, pos);
			if (num != deals.size())   //если элемент не удален то правда, иначе ложь
				if (pos + page * 6 == deals.size())
				{
					if (pos == 0)
					{
						if (page == 0)
						{
							getCharacter(L"Каталог пуст. Для возвращения нажмите любую кнопку");
							return;
						}
						else
						{
							page--;
							pos = 6;
						}
					}
					else
					{
						pos--;
					}
				}
		}
		else if (a == 'e' || a == 'E' || a == 'е' || a == 'Е') //русские и английские е
		{
			return;
		}
	}
}

void Catalog::showBestBrands()
{
	if (this->deals.empty())
	{
		getCharacter(L"Каталог пуст. Для продолжения введите любую клавишу");
		return;
	}
	unordered_map<string, int> umap;
	for (auto i : this->deals)
		umap[i.car.brand]++;
	vector<pair<string, int>> vec(umap.begin(), umap.end());
	sort(vec.begin(), vec.end(), comparePairs);
	system("cls");
	wcout << L"Самые продаваемые марки. Для выхода нажмите любую клавишу\n";
	for (int8_t i = 0; i < 3 && i < vec.size(); ++i)
	{
		cout << vec[i].first << " - " << vec[i].second;
		wcout << L" машин продано\n";
	}
	char a = getCharCode();
}