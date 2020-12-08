#include "Header.h"
#include "Users/Admin.h"
#include "Users/User.h"

using namespace std;

int main() {
    system("chcp 65001");
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
		drawMenu({"Войти как администратор", "\n\nВойти как пользователь", "\n\nВыход"}, row);

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

bool isCharacterValid(char &a) {
    return ((a >= 'a' && a <= 'z') ||
            (a >= 'A' && a <= 'Z') ||
            (a >= '0' && a <= '9') ||
            a == ' ');
}

bool stringStartsFromString(string &stringForSearch, string &searchingString) {
	return (stringForSearch.find(searchingString) != std::string::npos);
}


