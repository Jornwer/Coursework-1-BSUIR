#include "Catalog.h"
#include "../Users/AbstractUser.h"
#include "../Menu.h"

using namespace std;
using namespace nlohmann;
using Constants::elementsOnPage;

void Catalog::changeCatalog() {
    copyCatalogFile();

    (new Menu<Catalog>(" Просмотреть каталог", &Catalog::displayCatalog))
    ->add("\n\n Добавить элемент в каталог", &Catalog::enterElement)
    ->add("\n\n Удалить элемент из каталога", &Catalog::deleteDeal)
    ->add("\n\n Поиск в каталоге",  &Catalog::searchInCatalog)
    ->add("\n\n Изменить элемент", &Catalog::changeDeal)
    ->add("\n\n Показать самые продаваемые марки", &Catalog::showBestBrands)
    ->add("\n\n Показать все сделки пользователя", &Catalog::showUserDeals)
    ->add("\n\n Отсортировать каталог", &Catalog::sortCatalog)
    ->add("\n\n Назад")
    ->exit(8)
    ->addClass(*this)
    ->whileTrue();
}

void Catalog::displayCatalog() {
    if (this->deals.empty()) {getCharacter("Каталог пуст. Для возвращения нажмите любую кнопку"); return;}
    int pages = ceil((double)this->deals.size() / elementsOnPage), page = 0;
    while (true) {
        system("cls");
        cout << "Страница " << page+1 << " из " << pages <<
              ". Для перемещения страниц испольуйте стрелки вправо/влево. Для выхода нажмите Escape" << "\n\n";

        fort::utf8_table table;
        table.set_border_style(FT_NICE_STYLE);
        table << fort::header << "Марка" << "Модель" << "Цвет" <<
              "Цена" << "Дата продажи" << "Имя покупателя" << "Имя продавца" << fort::endr;
        for (uint32_t i = page * elementsOnPage; (int32_t)i < (page + 1) * elementsOnPage && i < this->deals.size(); ++i) {
            table.row(i%elementsOnPage+1).set_cell_text_align(fort::text_align::center);
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
    sort(vec.begin(), vec.end(),
            [](const pair<string, int>& l, const pair<string, int>& r) {
        return l.second > r.second;
    });

    system("cls");
    cout << "Самые продаваемые марки. Для выхода нажмите любую клавишу\n\n";

    fort::utf8_table table;
    table.set_border_style(FT_NICE_STYLE);
    table.column(0).set_cell_text_align(fort::text_align::center);
    table.column(1).set_cell_text_align(fort::text_align::center);
    table << fort::header << "Марка" << "Машин продан" << fort::endr;
    for (uint32_t i = 0; i < vec.size() && i < elementsOnPage; ++i) {
        table << vec[i].first << vec[i].second << fort::endr;
    }
    cout << table.to_string();
    getCharCode();
}

void Catalog::modifyElement(void (Catalog::*f)(int &, int &)) {
    int pages, page = 0, pos = 0, elOnPage;
    while (true) {
        if (this->deals.empty()) { getCharacter("Каталог пуст. Для возвращения нажмите любую кнопку"); return; }
        pages = ceil((double)this->deals.size() / elementsOnPage);
        if (page == pages - 1) {
            elOnPage = (int)this->deals.size() - page * elementsOnPage;
            pos = (pos >= elOnPage ? 0 : pos);
        } else elOnPage = elementsOnPage;

        system("cls");

        cout << "Страница " << page + 1 << " из " << pages <<
              ". Для перемещения страниц испольуйте стрелки вправо/влево. Для выхода нажмите Escape" << '\n' <<
              "Для выбора элемента нажмите Enter" << '\n' << '\n';

        fort::utf8_table table;
        table.set_border_style(FT_NICE_STYLE);
        table << fort::header << "Марка" << "Модель" << "Цвет" <<
              "Цена" << "Дата продажи" << "Имя покупателя" << "Имя продавца" << fort::endr;

        for (uint32_t i = page * elementsOnPage; (int32_t)i < (page + 1) * elementsOnPage && i < this->deals.size(); ++i) {
            if (i%elementsOnPage == (uint32_t)pos)
                table.row(i%elementsOnPage+1).set_cell_bg_color(fort::color::light_green);
            table.row(i%elementsOnPage+1).set_cell_text_align(fort::text_align::center);
            this->deals[i].fillTableRow(table);
        }
        cout << table.to_string();

        char a = getCharCode();

        if (a == VK_LEFT) page = (page + pages - 1) % pages;
        else if (a == VK_RIGHT) page = (page + 1) % pages;
        else if (a == VK_UP) pos = (pos + elOnPage - 1) % elOnPage;
        else if (a == VK_DOWN) pos = (pos + 1) % elOnPage;
        else if (a == 13) {
            uint32_t num = deals.size();
            (this->*f)(page, pos);
            if (num != deals.size())   //если элемент не удален то правда, иначе ложь
                if (pos + page * elementsOnPage == (int)deals.size()) {
                    if (pos == 0) {
                        if (page == 0) {
                            getCharacter("Каталог пуст. Для возвращения нажмите любую кнопку");
                            return;
                        }
                        else {
                            page--;
                            pos = elementsOnPage-1;
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

void Catalog::changeElement(int& page, int& pos) {
    enterElement(deals[pos + page * elementsOnPage]);
}

void Catalog::approveDeletion(int &page, int &pos) {
    string temp;
    system("cls");
    temp = getString("Введите \"yes\" для удаления элемента");
    if (temp == "yes") {
        deleteDeal(deals[pos + page * elementsOnPage]);
        getCharacter("Сделка удалена. Для продолжения нажмите любую клавишу");
    }
}

void Catalog::searchInCatalog() {
    string brand, model, color, priceFrom, priceTo, dateFrom, dateTo, buyerName, buyerSurname;
    int8_t row = 0;
    while (true) {
        system("cls");
        cout << "Марка       :" << brand << string(16 - brand.size(), ' ') << '|' << (!row ? " <--\n" : "\n");
        cout << "Модель      :" << model << string(16 - model.size(), ' ') << '|' << (row == 1 ? " <--\n" : "\n");
        cout << "Цвет        :" << color << string(16 - color.size(), ' ')<< '|' << (row == 2 ? " <--\n" : "\n");
        cout << "Цена с      :" << priceFrom << string(16 - priceFrom.size(), ' ')<< '|' << (row == 3 ? " <--\n" : "\n");
        cout << "Цена до     :" << priceTo << string(16 - priceTo.size(), ' ') << '|' << (row == 4 ? " <--\n" : "\n");
        cout << "С           :" << Date::displayDate(dateFrom) << string(6, ' ') << '|' << (row == 5 ? " <--\n" : "\n");
        cout << "До          :" << Date::displayDate(dateTo) << string(6, ' ') << '|' << (row == 6 ? " <--\n" : "\n");
        cout << "Имя пок.    :" << buyerName << string(16 - buyerName.size(), ' ') << '|' << (row == 7 ? " <--\n" : "\n");
        cout << "Фамилия пок.:" << buyerSurname << string(16 - buyerSurname.size(), ' ') << '|' << (row == 8 ? " <--\n" : "\n");
        cout << "Найти    " << (row == 9 ? " <--\n" : "\n");
        cout << "Назад    " << (row == 10 ? " <--\n" : "\n\n");

        uint8_t a = _getch();

        if (a == 224) {
            if (GetAsyncKeyState('F') != -32767) {
                a = _getch();
                if (a == 'H') row = (row + 10) % 11;
                else if (a == 'P') row = (row + 1) % 11;
            }
        }
        else {
            if (a == 13 && row == 9) displaySearch(brand, model, color,
                                                priceFrom, priceTo, dateFrom,
                                                dateTo, buyerName, buyerSurname);
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
            else if (a == 8 && row < 9) {
                if (row == 0 && !brand.empty()) brand.pop_back();
                else if (row == 1 && !model.empty()) model.pop_back();
                else if (row == 2 && !color.empty()) color.pop_back();
                else if (row == 3 && !priceFrom.empty()) priceFrom.pop_back();
                else if (row == 4 && !priceTo.empty()) priceTo.pop_back();
                else if (row == 5 && !dateFrom.empty()) dateFrom.pop_back();
                else if (row == 6 && !dateTo.empty()) dateTo.pop_back();
                else if (row == 7 && !buyerName.empty()) buyerName.pop_back();
                else if (row == 8 && !buyerSurname.empty()) buyerSurname.pop_back();
            }
        }
    }
}

void Catalog::displaySearch(string &brand, string &model, string &color,
        string &priceFrom, string &priceTo, string &dateFrom,
        string &dateTo, string &buyerName, string &buyerSurname) {

    Date from{}, to{};

    if (!dateFrom.empty()){
        if (!Date::validateDate(dateFrom)) {
            getCharacter("Дата введена неправильно. Для возвращения нажмите любую кнопку");
            return;
        }
        from = Date(dateFrom);
    }

    if (!dateTo.empty()){
        if (!Date::validateDate(dateTo)) {
            getCharacter("Дата введена неправильно. Для возвращения нажмите любую кнопку");
            return;
        }
        to = Date(dateTo);
    }

    int pTo = 0, pFrom = 0;
    try {
        pTo = stoi(priceTo.empty()?"0":priceTo), pFrom = stoi(priceFrom.empty()?"0":priceFrom);
    } catch (...){
        getCharacter("Одна из стоимостей введена неправильно. Для возвращения нажмите любую кнопку");
        return;
    }

    Catalog cat;

    for (auto& deal : this->deals) {
        string dealBuyerName = deal.buyerName, dealBuyerSurname;
        for (uint32_t i = 0; i < dealBuyerName.size(); ++i) {
            if (dealBuyerName[i] == ' ') {
                dealBuyerSurname = dealBuyerName.substr(i + 1);
                dealBuyerName.erase(dealBuyerName.begin() + i, dealBuyerName.end());
                break;
            }
        }

        if (!isStringContainString(deal.brand, brand) || !isStringContainString(deal.model, model)
            || !isStringContainString(deal.color, color) || !isStringContainString(dealBuyerName, buyerName)
            || !isStringContainString(dealBuyerSurname, buyerSurname)) continue;

        if (!(pTo == 0 && pFrom == 0)) {
            if (pTo != 0 && pFrom != 0) {
                if (pFrom > deal.price || pTo < deal.price)
                    continue;
            } else if (pTo == 0) {
                if (deal.price < pFrom) continue;
            } else {
                if (deal.price > pTo) continue;
            }
        }

        if (!(dateTo.empty() && dateFrom.empty())) {
            if (!(dateFrom.empty() || dateTo.empty())) {
                if (from > deal.date || to < deal.date) continue;
            } else if (dateTo.empty()) {
                if (deal.date < from) continue;
            } else {
                if (deal.date > to) continue;
            }
        }
        cat.deals.push_back(deal);
    }

    if (cat.deals.empty()) {
        getCharacter("Найдено 0 элементов. Для продолжения нажмите любую клавишу");
        return;
    }
    cat.displayCatalog();
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
    this->deals.push_back(deal);

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
    int8_t row = 0;
    string buyerSurname, date = (digitsInNumber(deal.date.dateToInt()) == 7?
                                                    "0" + to_string(deal.date.dateToInt()):
                                                     (deal.date.dateToInt()==0?
                                                         "":
                                                         to_string(deal.date.dateToInt())));
    if (!dealEmpty) {
        save = deal;
        for (uint32_t i = 0; i < deal.buyerName.size(); ++i) {
            if (deal.buyerName[i] == ' ') {
                buyerSurname = deal.buyerName.substr(i + 1);
                deal.buyerName.erase(deal.buyerName.begin() + i, deal.buyerName.end());
                break;
            }
        }
    } else {
        deal.setSeller();
    }

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
                else if (row == 4 && !date.empty()) date.pop_back();
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
    for (uint32_t i = 0; i < j.size(); ++i) {
        if (oldDeal == Deal(j[i])){
            j[i] = newDeal.toJson();
            ofstream output(Constants::pathToCatalogFile, ios::trunc);
            output << j;
            deals[i] = newDeal;
            return;
        }
    }
    appendDealInCatalog(newDeal);
}

void Catalog::deleteDeal(Deal &deal) {
    ifstream file(Constants::pathToCatalogFile);
    json j;
    try {
        file >> j;
    } catch (detail::parse_error &error) {
        return;
    }
    for (uint32_t i = 0; i < j.size(); i++){
        if (deal == Deal(j[i])){
            j.erase(j.begin() + i);
            deals.erase(deals.begin() + i);
            ofstream output(Constants::pathToCatalogFile, ios::trunc);
            output << j;
            return;
        }
    }
}

void Catalog::showUserDeals() {
    string user = AbstractUser::getCurrentUser();
    Catalog cat;
    for (auto & item: deals){
        if (item.seller == user)
            cat.deals.emplace_back(item);
    }
    if (cat.deals.empty()){
        getCharacter("Данный пользователь создал 0 сделок. Для выхода в меню нажмите любую клавишу");
        return;
    }
    cat.displayCatalog();
}

void Catalog::deleteDeal() {
    modifyElement(&Catalog::approveDeletion);
}

void Catalog::changeDeal() {
    modifyElement(&Catalog::changeElement);
}

void Catalog::sortCatalog() {
    int column = (new Menu<Catalog>("Сортировать по марке"))
            ->add("\n\nСортировать по моделе")
            ->add("\n\nСортировать по цвету")
            ->add("\n\nСортировать по цене")
            ->add("\n\nСортировать по дате")
            ->add("\n\nСортировать по имени покупателя")
            ->add("\n\nСортировать по имени продавца")
            ->chooseRow();
    
    Catalog catalog;
    catalog.deals = deals;
    sort(catalog.deals.begin(), catalog.deals.end(), chooseComparator(column));
    catalog.displayCatalog();
}

std::function<bool(const Deal&, const Deal&)> Catalog::chooseComparator(int column) {
    switch (column){
        case 0:
            return [](const Deal& d1, const Deal& d2){
                return d1.brand < d2.brand;
            };
        case 1:
            return [](const Deal& d1, const Deal& d2){
                return d1.model < d2.model;
            };
        case 2:
            return [](const Deal& d1, const Deal& d2){
                return d1.color < d2.color;
            };
        case 3:
            return [](const Deal& d1, const Deal& d2){
                return d1.price < d2.price;
            };
        case 4:
            return [](const Deal& d1, const Deal& d2){
                return d1.date < d2.date;
            };
        case 5:
            return [](const Deal& d1, const Deal& d2){
                return d1.buyerName < d2.buyerName;
            };
        case 6:
            return [](const Deal& d1, const Deal& d2){
                return d1.seller < d2.seller;
            };
        default:
            return [](const Deal&, const Deal&){
                return true;
            };
    }
}
