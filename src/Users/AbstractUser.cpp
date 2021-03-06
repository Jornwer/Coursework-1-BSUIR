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
    string pass1 = inputPassword("Введите пароль от аккаунта."
                                 " Для выхода введите exit или нажмите клавишу Esc");
    if (pass1 == "exit") return Error::AU_INTERRUPTION_ERROR;

    if (pass1 != this->password) {
        getCharacter("Пароли не совпадают. Для продолжения нажмите любую клавишу");
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
            login = getString("Введите логин. Для выхода введите exit или нажмите клавишу Esc");
            break;
        case 1:
            login = getString("Неправильный логин и/или пароль. Повторите ввод логина и пароля. "
                              "Для выхода введите exit или нажмите клавишу Esc");
            break;
        case 2:
            login = getString(
                    "Аккаунт с таким логином уже существует. Повторите ввод логина и пароля. "
                    "Для выхода введите exit или нажмите клавишу Esc");
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
            login = getString("Логин не может начинаться с пробела. Повторите ввод логина. "
                              "Для выхода введите exit или нажмите клавишу Esc");
            continue;
        }

        if (login.size() < 4) {
            error = 1;
            if (!login.empty())
                login = getString(
                        "Длина логина должна быть больше 3 символов. Повторите ввод логина. "
                        "Для выхода введите exit или нажмите клавишу Esc");
            else
                getline(cin, login);
            continue;
        }

        if (login.size() > 16) {
            error = 1;
            login = getString(
                    "Длина логина должна быть меньше 17 символов. "
                    "Повторите ввод логина. Для выхода введите exit или нажмите клавишу Esc");
            continue;
        }

        for (auto a : login) {
            if (!isCharacterValid(a)) {
                error = 2;
                login = getString("Логин содержит недопустимые символы. Повторите ввод логина."
                                  " Для выхода введите exit или нажмите клавишу Esc");
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
            getCharacter("Пользователь удален. Для продолжения нажмите любую клавишу");
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
    for (uint32_t i = 0; i < j.size(); ++i) {
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

    password = this->inputPassword("Введите пароль."
                                   " Для выхода введите exit или нажмите клавишу Esc");

    do {
        error = 0;

        if (password == "exit") {
            leave = true;
            return;
        }

        if (password.size() < 8) {
            error = 2;
            if (!password.empty())
                password = this->inputPassword("Пароль должен содержать минимум 8 символов."
                                               " Повторите ввод пароля."
                                               " Для выхода введите exit или нажмите клавишу Esc");
            else
                password = this->inputPassword();
            continue;
        }

        for (auto a : password) {
            if (!isCharacterValid(a)) {
                error = 1;
                password = this->inputPassword("Пароль содержит недопустимые символы."
                                               " Повторите ввод пароля."
                                               " Для выхода введите exit или нажмите клавишу Esc");
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
