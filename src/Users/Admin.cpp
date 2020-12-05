#include "Admin.h"

using namespace std;
using namespace nlohmann;

string Admin::pathToData() {
    return Constants::pathToAdminFile;
}

void Admin::addAdmin() {
    string login = getString("������� ��� ������������. ������� exit ��� ������ ��� ������� ������� Esc");
    if (login == "exit") return;
    if (deleteUserThenAddAdmin(login)) {
        getCharacter("������������ �������. ��� ����������� � ���� ������� ����� �������");
    }
    else getCharacter("������������ �� ������. ��� ����������� � ���� ������� ����� �������");
}

void Admin::deleteUser() {
    string login = getString("������� ��� ������������. ������� exit ��� ������ ��� ������� ������� Esc");
    if (login == "exit") return;

    if (deleteUser(login)) {
        getCharacter("������������ ������. ��� ����������� � ���� ������� ����� �������");
        return;
    }
    else getCharacter("������������ �� ������. ��� ����������� � ���� ������� ����� �������");
}

void Admin::addUser() {
    system("cls");

    bool leave = false;
    core::AbstractUserImpl newUser;
    newUser.enterCredentials(leave);
    if (leave) return;

    if (newUser.isUserLoginInFile(Constants::pathToAdminFile) || newUser.isUserLoginInFile(Constants::pathToUserFile)){
        getCharacter("������������ � ����� ������� ��� ����������. ��� ����������� � ���� ������� ����� �������");
    } else {
        newUser.addUserToFile(Constants::pathToUserFile);
        getCharacter("������������ ������� ��������. ��� ����������� ������� ����� �������");
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
        getCharacter("������������ � ����� ������� ��� ����������. ��� ����������� � ���� ������� ����� �������");
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
        drawMenu({ " �������� �������", "\n\n �������� ������","\n\n ������� �������",
                   "\n\n ��������� ������ ��������������", "\n\n ������� ������������",
                   "\n\n �������� ������ ������������", "\n\n �����" }, row);

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
            drawMenu({ "  ������� �������", "\n\n  �����" }, row);
        else
            drawMenu({ "  ����� � �������", "\n\n  �����" }, row);

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
    ifstream fIn(Constants::pathToUserFile);
    json j;
    try {
        fIn >> j;
    } catch (detail::parse_error &error) {
        return false;
    }
    for (int i = 0; i < j.size(); ++i) {
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
    j.push_back(j);

    ofstream output(path, ios::trunc);
    output << j;

    file.close();
    output.close();
}

