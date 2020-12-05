#include "AbstractUser.h"
#include "AbstractUserImpl.h"

using namespace std;
using namespace nlohmann;

AbstractUser::AbstractUser(std::string &login,
                           std::string &password) :
        login(std::move(login)), password(std::move(password)) {}

const std::string &AbstractUser::getCurrentUser() {
    return currentUser;
}

void AbstractUser::setCurrentUser() {
    AbstractUser::currentUser = login;
}

AbstractUser::Error AbstractUser::checkPasswords() {
    string pass1 = inputPassword("������� ������ �� ��������."
                                 " ��� ������ ������� exit ��� ������� ������� Esc");
    if (pass1 == "exit") return Error::AU_INTERRUPTION_ERROR;

    if (pass1 != this->password) {
        getCharacter("������ �� ���������. ��� ����������� ������� ����� �������");
        return Error::AU_ERROR;
    }
    return Error::AU_NO_ERROR;
}

std::string AbstractUser::inputPassword(const std::string &msg) {
    system("cls");
    cout << msg << '\n';

    return inputPassword();
}

std::string AbstractUser::inputPassword() {
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
            cout << '*';
        }
    }
    return pass;
}

void AbstractUser::enterAccount() {
    system("cls");
    bool leave = false;

    this->login = enterLogin(0, leave);
    if (leave) return;
    this->enterPassword(leave);
    if (leave) return;
    if (this->isUserInFile()) {
        userMenu();
        return;
    }

    do {
        this->login = enterLogin(1, leave);
        if (leave) return;
        this->enterPassword(leave);
        if (leave) return;
    } while (!this->isUserInFile());

    userMenu();
}

string AbstractUser::enterLogin(int8_t mode, bool &leave) {
    string login;
    int8_t error;
    switch (mode) {
        default:
        case 0:
            login = getString("������� �����. ��� ������ ������� exit ��� ������� ������� Esc");
            break;
        case 1:
            login = getString("������������ ����� �/��� ������. ��������� ���� ������ � ������. "
                              "��� ������ ������� exit ��� ������� ������� Esc");
            break;
        case 2:
            login = getString(
                    "������� � ����� ������� ��� ����������. ��������� ���� ������ � ������. "
                    "��� ������ ������� exit ��� ������� ������� Esc");
            break;
    }

    do {
        error = 0;

        if (login == "exit") {
            leave = true;
            break;
        }

        if (login[0] == ' ') {
            error = 1;
            login = getString("����� �� ����� ���������� � �������. ��������� ���� ������. "
                              "��� ������ ������� exit ��� ������� ������� Esc");
            continue;
        }

        if (login.size() < 4) {
            error = 1;
            if (!login.empty())
                login = getString(
                        "����� ������ ������ ���� ������ 3 ��������. ��������� ���� ������. "
                        "��� ������ ������� exit ��� ������� ������� Esc");
            else
                getline(cin, login);
            continue;
        }

        if (login.size() > 16) {
            error = 1;
            login = getString(
                    "����� ������ ������ ���� ������ 17 ��������. "
                    "��������� ���� ������. ��� ������ ������� exit ��� ������� ������� Esc");
            continue;
        }

        for (auto a : login) {
            if (!isCharacterValid(a)) {
                error = 2;
                login = getString("����� �������� ������������ �������. ��������� ���� ������."
                                  " ��� ������ ������� exit ��� ������� ������� Esc");
                break;
            }
        }

    } while (error);

    return login;
}

void AbstractUser::changePassword() {
    do {
        AbstractUser::Error error = this->checkPasswords();

        if (error == Error::AU_NO_ERROR) {
            string path = this->pathToData();
            ifstream file(path);
            json j;
            try {
                file >> j;
            } catch (detail::parse_error &error) {
                j = json::array();
            }

            for (auto &i : j) {
                if (i["login"] == login && i["password"] == sha256(password)) {
                    bool leave = false;
                    this->enterPassword(leave);
                    if (leave) return;
                    i["password"] = sha256(password);
                    ofstream output(path, ios::trunc);
                    output << j;

                    file.close();
                    output.close();
                    return;
                }
            }
        } else if (error == Error::AU_INTERRUPTION_ERROR) break;

    } while (true);

}

void AbstractUser::deleteAccount() {
    do {
        AbstractUser::Error error = this->checkPasswords();
        if (error == Error::AU_NO_ERROR) {
            this->deleteUserFromFile();
            getCharacter("������������ ������. ��� ����������� ������� ����� �������");
            return;
        } else if (error == Error::AU_INTERRUPTION_ERROR) break;

    } while (true);
}

void AbstractUser::addUserToFile(const string &path) {
    ifstream file(path);
    json j;
    try {
        file >> j;
    } catch (detail::parse_error &error) {
        j = json::array();
    }
    json user = userToJson();
    j.push_back(user);

    ofstream output(path, ios::trunc);
    output << j;

    file.close();
    output.close();
}

void AbstractUser::deleteUserFromFile() {
    string path = this->pathToData();
    ifstream file(path);
    json j;
    try {
        file >> j;
    } catch (detail::parse_error &error) {
        j = json::array();
    }
    for (int i = 0; i < j.size(); ++i) {
        if (j[i]["login"] == login && j[i]["password"] == sha256(password)) {
            j.erase(j.begin() + i);
            ofstream output(path, ios::trunc);
            output << j;
            file.close();
            output.close();
            return;
        }
    }
}

bool AbstractUser::isUserInFile() {
    string path = this->pathToData();
    ifstream file(path);
    json j;
    try {
        file >> j;
    } catch (detail::parse_error &error) {
        return false;
    }
    for (auto &i : j) {
        if (i["login"] == login && i["password"] == sha256(password)) {
            return true;
        }
    }
    return false;
}

void AbstractUser::enterPassword(bool &leave) {
    int8_t error;
    string password;

    password = this->inputPassword("������� ������."
                                   " ��� ������ ������� exit ��� ������� ������� Esc");

    do {
        error = 0;

        if (password == "exit") {
            leave = true;
            return;
        }

        if (password.size() < 8) {
            error = 2;
            if (!password.empty())
                password = this->inputPassword("������ ������ ��������� ������� 8 ��������."
                                               " ��������� ���� ������."
                                               " ��� ������ ������� exit ��� ������� ������� Esc");
            else
                password = this->inputPassword();
            continue;
        }

        for (auto a : password) {
            if (!isCharacterValid(a)) {
                error = 1;
                password = this->inputPassword("������ �������� ������������ �������."
                                               " ��������� ���� ������."
                                               " ��� ������ ������� exit ��� ������� ������� Esc");
                break;
            }

        }
    } while (error);

    this->password = password;
}

bool AbstractUser::isUserFileEmpty() {
    string path = this->pathToData();
    ifstream file(path);
    json j;
    try {
        file >> j;
        file.close();
        return false;
    } catch (detail::parse_error &error) {
        file.close();
        return true;
    }
}

nlohmann::json AbstractUser::userToJson() {
    json j;
    j["login"] = login;
    j["password"] = sha256(password);
    return j;
}

bool AbstractUser::isUserLoginInFile(const string &path) {
    ifstream file(path);
    json j;
    try {
        file >> j;
    } catch (detail::parse_error &error) {
        return false;
    }
    for (auto &i : j) {
        if (i["login"] == login) {
            return true;
        }
    }
    return false;
}
