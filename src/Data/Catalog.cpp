#include "Catalog.h"
#include "../Users/AbstractUser.h"

using namespace std;
using namespace nlohmann;

void Catalog::changeCatalog() {
    int8_t row = 0;
    int8_t colNum = 7;
    copyCatalogFile();

    while (true) {
        system("cls");
        drawMenu({ " Просмотреть каталог", "\n\n Добавить элемент в каталог", "\n\n Удалить элемент из каталога",
                   "\n\n Поиск в каталоге", "\n\n Изменить элемент", "\n\n Показать самые продаваемые марки", "\n\n Назад" }, row);

        char a = getCharCode();

        if (a == VK_UP) row = (row + colNum - 1) % colNum;
        else if (a == VK_DOWN) row = (row + 1) % colNum;
        else if (a == 13) {
            if (row == 0) displayCatalog();
            else if (row == 1) enterElement();
            else if (row == 2) modifyElement(&Catalog::approveDeletion);
            else if (row == 3) searchInCatalog();
            else if (row == 4) modifyElement(&Catalog::changeElement);
            else if (row == 5) showBestBrands();
            else if (row == 6) {
                system("cls");
                break;
            }
        }
    }
}

void Catalog::displayCatalog() {
    if (this->deals.empty()) {getCharacter("Каталог пуст. Для возвращения нажмите любую кнопку"); return;}
    int pages = ceil((double)this->deals.size() / 6.0), page = 0;
    while (true) {
        system("cls");
        cout << "Страница " << page+1 << " из " << pages <<
              ". Для перемещения страниц испольуйте стрелки вправо/влево. Для выхода нажмите Escape" << "\n\n";

        fort::utf8_table table;
        table.set_border_style(FT_NICE_STYLE);
        table << fort::header << "Марка" << "Модель" << "Цвет" <<
              "Цена" << "Дата продажи" << "Имя покупаьеля" << "Имя продавца" << fort::endr;
        for (uint32_t i = page * 6; (int32_t)i < (page + 1) * 6 && i < this->deals.size(); ++i) {
            table.column(i%6).set_cell_text_align(fort::text_align::center);
            this->deals[i].fillTableRow(table);
        }
        cout << table.to_string();

        char a = getCharCode();

        if (a == VK_LEFT) page = (page + pages - 1) % pages;
        else if (a == VK_RIGHT) page = (page + 1) % pages;
        else if (a == VK_ESCAPE) break;
    }
}

void Catalog::showBestBrands() {
    if (this->deals.empty()) {
        getCharacter("Каталог пуст. Для продолжения введите любую клавишу");
        return;
    }
    unordered_map<string, int> umap;
    for (const auto &i : this->deals)
        umap[i.brand]++;
    vector<pair<string, int>> vec(umap.begin(), umap.end());
    sort(vec.begin(), vec.end(), comparePairs);
    system("cls");
    cout << "Самые продаваемые марки. Для выхода нажмите любую клавишу\n";
    for (uint32_t i = 0; i < 3 && i < vec.size(); ++i) {
        cout << vec[i].first << " - " << vec[i].second;
        cout << " машин продано\n";
    }
    getCharCode();
}

void Catalog::modifyElement(void (Catalog::*)(int &, int &)) {

}

void Catalog::changeElement(int &, int &) {

}

void Catalog::approveDeletion(int &, int &) {

}

void Catalog::searchInCatalog() {

}

void Catalog::displaySearch(std::string &, std::string &, std::string &, std::string &, std::string &, std::string &,
                            std::string &, std::string &) {

}

void Catalog::copyCatalogFile() {
    if(!deals.empty()) return;
    ifstream file(Constants::pathToCatalogFile);
    json j;

    try {
        file >> j;
    } catch (detail::parse_error &error) {
        j = json::array();
    }

    for (auto &i: j) {
        deals.emplace_back(Deal(i["date"], i["brand"], i["model"],
                i["color"], i["price"], i["seller"], i["buyerName"]));
    }

    file.close();
}

void Catalog::appendDealInCatalog(Deal &deal) {
    ifstream file(Constants::pathToCatalogFile);
    json j;
    try {
        file >> j;
    } catch (detail::parse_error &error) {
        j = json::array();
    }
    json user = deal.toJson();
    j.push_back(user);

    ofstream output(Constants::pathToCatalogFile, ios::trunc);
    output << j;

    file.close();
    output.close();
}

void Catalog::enterElement() {
    Deal deal;
    enterElement(deal);
}

void Catalog::enterElement(Deal &deal) {
    Deal save;
    bool dealEmpty = (deal == Deal());
    if (!dealEmpty){
        save = deal;
    }
    int8_t row = 0;
    deal.setSeller();
    string buyerSurname, date;

    while (true) {
        system("cls");
        cout << "Марка       :" << deal.brand << string(16 - deal.brand.size(), ' ') << '|' << (row == 0 ? " <--\n" : "\n");
        cout << "Модель      :" << deal.model << string(16 - deal.model.size(), ' ') << '|' << (row == 1 ? " <--\n" : "\n");
        cout << "Цвет        :" << deal.color << string(16 - deal.color.size(), ' ') << '|' << (row == 2 ? " <--\n" : "\n");
        cout << "Цена        :" << deal.price << string(16 - digitsInNumber(deal.price), ' ') << '|' << (row == 3 ? " <--\n" : "\n");
        cout << "Дата        :" << Date::displayDate(date) << string(6, ' ') << '|' << (row == 4 ? " <--\n" : "\n");
        cout << "Имя пок.    :" << deal.buyerName << string(16 - deal.buyerName.size(), ' ') << '|' << (row == 5 ? " <--\n" : "\n");
        cout << "Фамилия пок.:" << buyerSurname << string(16 - buyerSurname.size(), ' ') << '|' << (row == 6 ? " <--\n" : "\n");
        cout << "Продавец    :" << deal.seller << string(16 - deal.seller.size(), ' ') << '|' << "\n";
        cout << "Ввести      " << (row == 7 ? " <--\n" : "\n");
        cout << "Назад    " << (row == 8 ? " <--\n" : "\n\n");

        uint8_t a = _getch();

        if (a == 224) {
            if (GetAsyncKeyState('F') != -32767) {
                a = _getch();
                if (a == 'H') row = (row + 8) % 9;
                else if (a == 'P') row = (row + 1) % 9;
            }
        } else {
            if (a == 13 && row == 7) {
                string saveBuyerName = deal.buyerName;
                deal.buyerName += (buyerSurname.empty()? "":" " + buyerSurname);
                deal.date = Date(date);
                if (deal.isDealCorrect()) {
                    if (dealEmpty) {
                        appendDealInCatalog(deal);
                        this->deals.push_back(deal);
                        getCharacter("Сделка успешно добавлена. Для продолжения нажмите любую клавишу");
                        continue;
                    } else {
                        rewriteDealInCatalog(save, deal);
                        getCharacter("Сделка успешно изменена. Для продолжения нажмите любую клавишу");
                        return;
                    }
                }
                deal.buyerName = saveBuyerName;
            } else if (a == 13 && row == 8) {
                if (!dealEmpty) deal = save;
                return;
            } else if ((a >= '0' && a <= '9') && (row == 3 || row == 4)) {
                if (row == 3 && digitsInNumber(deal.price) < 9) deal.price = 10 * deal.price + a - '0';
                else if (row == 4 && date.size() < 8) date += a;
            } else if ((a >= 'a' && a <= 'z') || (a >= 'A' && a <= 'Z') || (a >= '0' && a <= '9')) {
                if (!row && deal.brand.size() < 16) deal.brand += a;
                else if (row == 1 && deal.model.size() < 16) deal.model += a;
                else if (row == 2 && deal.color.size() < 16) deal.color += a;
                else if (row == 5 && deal.buyerName.size() < 16) deal.buyerName += a;
                else if (row == 6 && buyerSurname.size() < 16) buyerSurname += a;
            } else if (a == 8 && row < 7) {
                if (row == 0 && !deal.brand.empty()) deal.brand.pop_back();
                else if (row == 1 && !deal.model.empty()) deal.model.pop_back();
                else if (row == 2 && !deal.color.empty()) deal.color.pop_back();
                else if (row == 3) deal.price /= 10;
                else if (row == 4 && !date.empty()) date.erase(date.end() - 1);
                else if (row == 5 && !deal.buyerName.empty()) deal.buyerName.pop_back();
                else if (row == 6 && !buyerSurname.empty()) buyerSurname.pop_back();
            }
        }
    }
}

void Catalog::rewriteDealInCatalog(Deal &oldDeal, Deal &newDeal) {
    ifstream file(Constants::pathToCatalogFile);
    json j;
    try {
        file >> j;
    } catch (detail::parse_error &error) {
        appendDealInCatalog(newDeal);
        return;
    }
    for (auto &i: j){
        if (oldDeal == Deal(i)){
            i = newDeal.toJson();
            ofstream output(Constants::pathToCatalogFile, ios::trunc);
            output << j;
            return;
        }
    }
    appendDealInCatalog(newDeal);
}

/*
void Catalog::approveDeletion(int &page, int &pos) {
	string temp;
	system("cls");
	temp = getString("Введите \"yes\" для удаления элемента");
	if (temp == "yes") {
		this->deals.erase(this->deals.begin() + pos + page * 6);
		rewriteCatalogFile(*this);
	}
}

void Catalog::searchInCatalog() {
	string brand, model, color, priceFrom, priceTo, dateFrom, dateTo, buyerName, buyerSurname;
	int8_t row = 0;
	while (true) {
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

		if (a == 224) {
			if (GetAsyncKeyState('F') != -32767) {
				a = _getch();
				if (a == 'H') row = (row + 10) % 11;
				else if (a == 'P') row = (row + 1) % 11;
			}
		}
		else {
			if (a == 13 && row == 9) displaySearch(brand, model, color, priceFrom, priceTo, dateFrom, dateTo, buyerName, buyerSurname);
			else if (a == 13 && row == 10) return;
			else if ((a >= '0' && a <= '9') && row > 2 && row < 7) {
				if (row == 3 && priceFrom.size() < 11) priceFrom += a;
				else if (row == 4 && priceTo.size() < 11) priceTo += a;
				else if (row == 5 && dateFrom.size() < 8) dateFrom += a;
				else if (row == 6 && dateTo.size() < 8) dateTo += a;
			}
			else if ((a >= 'a' && a <= 'z') || (a >= 'A' && a <= 'Z') || (a >= '0' && a <= '9')) {
				if (!row && brand.size() < 16) brand += a;
				else if (row == 1 && model.size() < 16) model += a;
				else if (row == 2 && color.size() < 16) color += a;
				else if (row == 7 && buyerName.size() < 16) buyerName += a;
				else if (row == 8 && buyerSurname.size() < 16) buyerSurname += a;
			}
			else if (a == 8 && row < 7) {
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
							string priceTo, string dateFrom, string dateTo, string buyerName, string buyerSurname) {
	if ((dateFrom.size() != 8 && !dateFrom.empty()) || (dateTo.size() != 8 && !dateTo.empty())) {
		getCharacter("Дата введена неправильно. Для возвращения нажмите любую кнопку");
		return;
	}

	Catalog cat;

	for (auto i : this->deals) {
		if (!stringContainString(i.car.brand, brand) || !stringContainString(i.car.model, model)
			|| !stringContainString(i.car.color, color) || !stringContainString(i.buyerName, buyerName)
			|| !stringContainString(i.buyerSurname, buyerSurname)) continue;
		if (!(priceTo.empty() && priceFrom.empty())) {
			if (priceTo.empty()) {
				if (stringToInt(i.car.price) < stringToInt(priceFrom)) continue;
			}
			if (priceFrom.empty()) {
				if (stringToInt(i.car.price) > stringToInt(priceTo)) continue;
			}
			if (!priceFrom.empty() && !priceTo.empty())
				if (stringToInt(priceFrom) > stringToInt(i.car.price) || stringToInt(priceTo) < stringToInt(i.car.price)) continue;
		}
		if (!(dateTo.empty() && dateFrom.empty())) {
			if (dateTo.empty()) {
				Date from(dateFrom), date(i.date.day + i.date.month + i.date.year);
				if (date < from) continue;
			}
			if (dateFrom.empty()) {
				Date to(dateTo), date(i.date.day + i.date.month + i.date.year);
				if (date > to) continue;
			}
			if (!(dateFrom.empty() || dateTo.empty())) {
				Date from(dateFrom), to(dateTo), date(i.date.day + i.date.month + i.date.year);
				if (from > date || to < date) continue;
			}
		}
		cat.deals.push_back(i);
	}

	if (cat.deals.empty()) {
		getCharacter("Найдено 0 элементов. Для продолжения нажмите любую клавишу");
		return;
	}
	cat.displayCatalog();
}

void Catalog::changeElement(int& page, int& pos) {
	enterElement(deals[pos + page * 6], deals[pos + page * 6].date.day + deals[pos + page * 6].date.month + deals[pos + page * 6].date.year);
	rewriteCatalogFile(*this);
}

void Catalog::modifyElement(void (Catalog::*f)(int&, int&)) {
	int pages, page = 0, pos = 0, elInPage;
	while (true)
	{
		if (this->deals.empty()) { getCharacter("Каталог пуст. Для возвращения нажмите любую кнопку"); return; }
		pages = ceil((double)this->deals.size() / 6.0);
		if (page == pages - 1) { elInPage = this->deals.size() - page * 6; pos = (pos >= elInPage ? 0 : pos); }
		else elInPage = 6;
		system("cls");

		wcout << L"Страница " << page + 1 << L" из " << pages <<
			L". Для перемещения страниц испольуйте стрелки вправо/влево. Для выхода нажмите Escape" << L'\n' <<
			L"Для выбора элемента нажмите Enter" << L'\n' << L'\n';

		for (int i = page * 6; i < (page + 1) * 6 && i < this->deals.size(); ++i) {
			if (i % 6 == pos) cout << string(40, '/');
			this->deals[i].displayElement();
			if (i % 6 == pos) cout << string(40, '\\');
		}

		char a = getCharCode();

		if (a == VK_LEFT) page = (page + pages - 1) % pages;
		else if (a == VK_RIGHT) page = (page + 1) % pages;
		else if (a == VK_UP) pos = (pos + elInPage - 1) % elInPage;
		else if (a == VK_DOWN) pos = (pos + 1) % elInPage;
		else if (a == 13) {
			int num = deals.size();
			(this->*f)(page, pos);
			if (num != deals.size())   //если элемент не удален то правда, иначе ложь
				if (pos + page * 6 == deals.size()) {
					if (pos == 0) {
						if (page == 0) {
							getCharacter("Каталог пуст. Для возвращения нажмите любую кнопку");
							return;
						}
						else {
							page--;
							pos = 6;
						}
					}
					else {
						pos--;
					}
				}
		}
		else if (a == VK_ESCAPE) {
			return;
		}
	}
}
*/