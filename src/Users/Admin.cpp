#include "Admin.h"

using namespace std;
using namespace nlohmann;

string Admin::pathToData() {
    return Constants::pathToAdminFile;
}

void Admin::addAdmin() {
    string login = getString("Введите имя пользователя. Введите exit для выхода");
    if (login == "exit") return;
    if (deleteUserThenAddAdmin(login)) {
        getCharacter("Пользователь повышен. Для возвращения в меню нажмите любую клавишу");
    }
    else getCharacter("Пользователь не найден. Для возвращения в меню нажмите любую клавишу");
}

void Admin::deleteUser() {
    string login = getString("Введите имя пользователя. Введите exit для выхода");
    if (login == "exit") return;

    if (deleteUser(login)) {
        getCharacter("Пользователь удален. Для возвращения в меню нажмите любую клавишу");
        return;
    }
    else getCharacter("Пользователь не найден. Для возвращения в меню нажмите любую клавишу");
}

void Admin::addUser() {
    system("cls");

    bool leave = false;
    core::AbstractUserImpl newUser;
    newUser.enterCredentials(leave);
    if (leave) return;

    if (newUser.isUserLoginInFile(Constants::pathToAdminFile) || newUser.isUserLoginInFile(Constants::pathToUserFile)){
        getCharacter("Пользователь с таким логином уже существует. Для возвращения в меню нажмите любую клавишу");
    } else {
        getCharacter("Пользователь успешно добавлен. Для продолжения нажмите любую клавишу");
    }

}

void Admin::createAccount() {
    system("cls");

    bool leave = false;

    this->login = enterLogin(0, leave);
    if (leave) return;
    enterPassword(leave);
    if (leave) return;

    if (isUserLoginInFile(Constants::pathToUserFile)){
        getCharacter("Пользователь с таким логином уже существует. Для возвращения в меню нажмите любую клавишу");
    } else {
        addUserToFile(Constants::pathToAdminFile);
        userMenu();
    }

}

void Admin::userMenu() {
    int8_t row = 0;
    int8_t colNum = 7;
    Catalog catalog;
    setCurrentUser();
    while (true) {
        system("cls");
        drawMenu({ " Изменить каталог", "\n\n Изменить пароль","\n\n Удалить аккаунт",
                   "\n\n Назначить нового администратора", "\n\n Удалить пользователя",
                   "\n\n Добавить нового пользователя", "\n\n Назад" }, row);

        char a = getCharCode();

        if (a == VK_UP) row = (row + colNum - 1) % colNum;
        else if (a == VK_DOWN) row = (row + 1) % colNum;
        else if (a == 13) {
            if (row == 0) catalog.changeCatalog();
            else if (row == 1) changePassword();
            else if (row == 2) { deleteAccount(); break; }
            else if (row == 3) addAdmin();
            else if (row == 4) deleteUser();
            else if (row == 5) addUser();
            else if (row == 6) {
                system("cls");
                break;
            }
        }
    }
}

void Admin::adminHaveAccount() {
    int8_t row = 0;
    Admin admin;
    bool isUserFileEmpty = admin.isUserFileEmpty();

    while (true) {
        system("cls");

        if (isUserFileEmpty)
            drawMenu({ "  Создать аккаунт", "\n\n  Назад" }, row);
        else
            drawMenu({ "  Войти в аккаунт", "\n\n  Назад" }, row);

        char a = getCharCode();

        if (a == VK_UP || a == VK_DOWN) row = !row;
        else if (a == 13) {
            if (isUserFileEmpty) {
                if (row == 0) admin.createAccount();
                else if (row == 1) {
                    system("cls");
                    break;
                }

            } else {
                if (row == 0) admin.enterAccount();
                else if (row == 1) {
                    system("cls");
                    break;
                }
            }
            return;
        }
    }
}

bool Admin::deleteUserThenAddAdmin(string &login) {
    ifstream file(Constants::pathToUserFile);
    json j;
    try {
        file >> j;
    } catch (detail::parse_error &error) {
        return false;
    }
    for (int i = 0; i < j.size(); ++i) {
        if (j[i]["login"] == login) {
            string password = j["password"];
            j.erase(j.begin() + i);
            ofstream output(Constants::pathToUserFile, ios::trunc);
            output << j;
            file.close();
            output.close();
            (new Admin(login, password))->addUserToFile(Constants::pathToAdminFile);
            return true;
        }
    }
    return false;
}

bool Admin::deleteUser(string &login) {
    ifstream file(Constants::pathToUserFile);
    json j;
    try {
        file >> j;
    } catch (detail::parse_error &error) {
        return false;
    }
    for (int i = 0; i < j.size(); ++i) {
        if (j[i]["login"] == login) {
            j.erase(j.begin() + i);
            ofstream output(Constants::pathToUserFile, ios::trunc);
            output << j;
            file.close();
            output.close();
            return true;
        }
    }
    return false;
}
