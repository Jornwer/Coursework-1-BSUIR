#include "Header.h"
#include "Users/Admin.h"
#include "Users/User.h"

using namespace std;

int main() {
    system("chcp 1251");
	mainMenu();
    return 0;
}

void drawMenu(vector<string> msg, int8_t &row) {
    for (int i = 0; (unsigned long long) i < msg.size(); ++i) {
        cout << msg.at(i);
        if (row == i) cout << "   <--";
    }
    cout << "\n\n" << flush;
}

void mainMenu() {
	int8_t row = 0;

	while (true) {
		system("cls");
		drawMenu({"����� ��� �������������", "\n\n����� ��� ������������", "\n\n�����"}, row);

		int8_t a = getCharCode();

		if (a == VK_UP) row = (row + 2) % 3;
		else if (a == VK_DOWN) row = (row + 1) % 3;
		else if (a == 13) {
			if (row == 0) Admin::adminHaveAccount();
			else if (row == 1) User::userHaveAccount();
			else if (row == 2) {
				system("cls");
				break;
			}
		}
	}
}

string getString(const string &msg) {
    system("cls");
    cout << msg << '\n';
    string tmp;
    return getString();
}

string getString() {
    string pass;
    unsigned char a;

    while ((a = getch()) != 13) {
        if (a == 27) return "exit";

        if ((int) a == 8) {
            if (pass.empty()) continue;

            pass.pop_back();
            cout << '\b' << ' ' << '\b';
        } else {
            pass += a;
            cout << a;
        }
    }
    return pass;
}

void getCharacter(const string &msg) {
    system("cls");
    cout << msg << '\n';
    getCharCode();
}

int8_t getCharCode() {
	HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
	INPUT_RECORD rec;
	DWORD numRead;
	while (ReadConsoleInput(hIn, &rec, 1, &numRead) && numRead == 1) {
		if (rec.EventType == KEY_EVENT && rec.Event.KeyEvent.bKeyDown) {
			return rec.Event.KeyEvent.wVirtualKeyCode;
		}
	}
    return 0;
}

void displayDate(string date) {
	int8_t index = 0;
	string str = "��.��.����      ";
	for (int8_t i = 0; i < 16; ++i) {
		if (index + 1 > date.size() || index == 8)
		    cout << str[i];
		else {
			if (i == 2 || i == 5)
			    cout << '.';
			else
			    cout << date[index++];
		}
	}
}

bool isCharacterValid(char &a) {
    return ((a >= 'a' && a <= 'z') ||
            (a >= 'A' && a <= 'Z') ||
            (a >= '0' && a <= '9') ||
            a == ' ');
}

/*bool dealCorrect(const Deal& deal, const string& date) {
	if (deal.car.brand.empty() || deal.car.color.empty() || deal.car.model.empty()
			|| deal.car.price.empty() || deal.buyerName.empty() || deal.buyerSurname.empty()) {
		getCharacter(L"���� �� ����� ������. ��� ���������� ����� ������ ������� ����� �������");
		return false;
	}

	if (date.size() != 8) {
		getCharacter(L"���� ������ �������� �� 8 ����. ��� ���������� ����� ������ ������� ����� �������");
		return false;
	}

	if (deal.car.price.size() > 9) {
		getCharacter(L"��������� ������ ������ ���� ������ 1000000000. ��� ����������� ������� ����� �������");
		return false;
	}

	if (stringToInt(deal.car.price) == 0) {
		getCharacter(L"��������� ������ ������ ���� ������ 0. ��� ����������� ������ ����� �������");
		return false;
	}

	Date tmp(date);
	if (!dayCorrect(stringToInt(tmp.day), stringToInt(tmp.month), stringToInt(tmp.year))) {
		getCharacter(L"��������� ���� �� ����������. ��� ����������� ������� ����� �������");
		return false;
	}

	time_t now = time(0);
	tm* ltm = localtime(&now);
	string currTime = (ltm->tm_mday < 10 ? "0" + to_string(ltm->tm_mday) : to_string(ltm->tm_mday))
		+ (ltm->tm_mon < 9 ? "0" + to_string(1 + ltm->tm_mon) : to_string(1 + ltm->tm_mon))
		+ to_string(1900 + ltm->tm_year);

	if (tmp > Date(currTime)) {
		getCharacter(L"��������� ���� ������ ��������� ����. ��� ����������� ������� ����� �������");
		return false;
	}

	if (tmp < Date("01011990")) {
		getCharacter(L"��������� ���� ����� ���� ������ 01.01.1990. ��� ����������� ������� ����� �������");
		return false;
	}
	
	return true;
}

bool dealCorrect(const Deal& deal)
{
	string date = deal.date.day + deal.date.month + deal.date.year;

	if (deal.car.brand.empty() && deal.car.color.empty() && deal.car.model.empty() && deal.car.price.empty()
		&& date.empty() && deal.buyerName.empty() && deal.buyerSurname.empty() && deal.seller.empty()) return true;

	if (deal.date.day.size() != 2 || deal.date.month.size() != 2 || deal.date.year.size() != 4) return false;

	if (deal.car.brand.empty() || deal.car.color.empty() || deal.car.model.empty() || deal.car.price.empty()
		|| deal.buyerName.empty() || deal.buyerSurname.empty() || deal.seller.empty()) return false;

	if (stringToInt(deal.car.price) == 0 || stringToInt(deal.car.price) == -1) return false;

	if (deal.car.price.size() > 9) return false;

	for (const auto& str : vector<string>{ deal.car.color, deal.car.brand, deal.car.model, deal.buyerName, deal.buyerSurname})
		for (auto i : str)
			if (!((i >= 'a' && i <= 'z') || (i >= 'A' && i <= 'Z') || (i >= '0' && i <= '9')))
				return false;

	for (auto i : deal.seller)
		if (!((i >= 'a' && i <= 'z') || (i >= 'A' && i <= 'Z') || (i >= '0' && i <= '9') || i == ' '))
			return false;


	for (auto a : date)
		if (a < '0' || a > '9') return false;

	if (!dayCorrect(stringToInt(deal.date.day), stringToInt(deal.date.month), stringToInt(deal.date.year))) return false;

	time_t now = time(nullptr);
	tm* ltm = localtime(&now);

	string currTime = (ltm->tm_mday < 10 ? "0" + to_string(ltm->tm_mday) : to_string(ltm->tm_mday)) 
					+ (ltm->tm_mon < 9 ? "0" + to_string(1 + ltm->tm_mon) : to_string(1 + ltm->tm_mon))
					+ to_string(1900 + ltm->tm_year);
	if (deal.date > Date(currTime)) return false;

	if (deal.date < Date("01011990")) return false;

	return true;
}*/

bool stringContainString(string& stringForSearch, string& searchingString) {
	if (stringForSearch.size() < searchingString.size()) return false;
	for (int i = 0; i < searchingString.size(); ++i)
		if (stringForSearch[i] != searchingString[i])
			return false;
	return true;
}

bool comparePairs(pair<string, int> l, pair<string, int> r) {
	return l.second > r.second;
}

bool operator==(const Deal& l, const Deal& r) {
    return (l.car.brand == r.car.brand && l.car.color == r.car.color && l.date == r.date
            && l.car.model == r.car.model && l.car.price == r.car.price
            && l.buyerName == r.buyerName && l.buyerSurname == r.buyerSurname && l.seller == r.seller);
}
