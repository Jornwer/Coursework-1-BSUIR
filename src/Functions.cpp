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
    class MainMenu{
    public:
        void adminHaveAccount(){
            Admin::adminHaveAccount();
        }
        void userHaveAccount(){
            User::userHaveAccount();
        }
    };

    (new Menu<MainMenu>("Войти как администратор", &MainMenu::adminHaveAccount))
    ->add("\n\nВойти как пользователь", &MainMenu::userHaveAccount)
    ->add("\n\nВыход")
    ->exit(2)
    ->addClass(*(new MainMenu()))
    ->whileTrue();
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
