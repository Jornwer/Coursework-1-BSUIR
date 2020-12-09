#include "Admin.h"

using namespace std;
using namespace nlohmann;

string Admin::pathToData() {
    return Constants::pathToAdminFile;
}

void Admin::addAdmin() {
    string login = getString("Введите имя пользователя. Введите exit для выхода или нажмите клавишу Esc");
    if (login == "exit") return;
    if (deleteUserThenAddAdmin(login)) {
        getCharacter("Пользователь повышен. Для возвращения в меню нажмите любую клавишу");
    }
    else getCharacter("Пользователь не найден. Для возвращения в меню нажмите любую клавишу");
}

void Admin::deleteUser() {
    string login = getString("Введите имя пользователя. Введите exit для выхода или нажмите клавишу Esc");
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
        newUser.addUserToFile(Constants::pathToUserFile);
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
    setCurrentUser();
    (new Menu<Admin>(" Изменить каталог", &Admin::changeCatalog))
    ->add("\n\n Изменить пароль", &Admin::changePassword)
    ->add("\n\n Удалить аккаунт", &Admin::deleteAccount)
    ->add("\n\n Назначить нового администратора", &Admin::addAdmin)
    ->add("\n\n Удалить пользователя", &Admin::deleteUser)
    ->add("\n\n Добавить нового пользователя", &Admin::addUser)
    ->add("\n\n Назад")
    ->exit(2)
    ->exit(6)
    ->addClass(*this)
    ->whileTrue();
}

void Admin::adminHaveAccount() {
    Admin admin;
    Menu<Admin>* menu;
    if (admin.isUserFileEmpty()) {
        menu = new Menu<Admin>("  Создать аккаунт", &Admin::createAccount);
    } else {
        menu = new Menu<Admin>("  Войти в аккаунт", &Admin::enterAccount);
    }
    menu
    ->add("\n\n  Назад")
    ->exit(0)
    ->exit(1)
    ->addClass(admin)
    ->whileTrue();
}

bool Admin::deleteUserThenAddAdmin(string &login) {
    ifstream fIn(Constants::pathToUserFile);
    json j;
    try {
        fIn >> j;
    } catch (detail::parse_error &error) {
        return false;
    }
    for (uint32_t i = 0; i < j.size(); ++i) {
        if (j[i]["login"] == login) {
            string password = j[i]["password"];
            j.erase(j.begin() + i);
            ofstream fOut(Constants::pathToUserFile, ios::trunc);
            fOut << j;
            fIn.close();
            fOut.close();
            addUserToFileWithoutPasswordEncoding(login, password, Constants::pathToAdminFile);
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
    for (uint32_t i = 0; i < j.size(); ++i) {
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

void Admin::addUserToFileWithoutPasswordEncoding(const std::string &login,
                                                 const std::string &password,
                                                 const std::string &path) {
    ifstream file(path);
    json j;
    try {
        file >> j;
    } catch (detail::parse_error &error) {
        j = json::array();
    }
    json user;
    user["login"] = login;
    user["password"] = password;
    j.push_back(user);

    ofstream output(path, ios::trunc);
    output << j;

    file.close();
    output.close();
}

void Admin::changeCatalog() {
    Catalog catalog;
    catalog.changeCatalog();
}

