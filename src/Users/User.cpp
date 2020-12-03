#include "User.h"

std::string User::pathToData() {
    return Constants::pathToUserFile;
}

void User::userMenu() {
    int8_t row = 0;
    int8_t colNum = 4;
    Catalog catalog;
    setCurrentUser();
    while (true) {
        system("cls");
        drawMenu({ " Изменить каталог", "\n\n Изменить пароль","\n\n Удалить аккаунт" ,"\n\n Назад" }, row);

        char a = getCharCode();

        if (a == VK_UP) row = (row + colNum - 1) % colNum;
        else if (a == VK_DOWN) row = (row + 1) % colNum;
        else if (a == 13) {
            if (row == 0) catalog.changeCatalog();
            else if (row == 1) changePassword();
            else if (row == 2) { deleteAccount(); break; }
            else if (row == 3) {
                system("cls");
                break;
            }
        }
    }
}

void User::userHaveAccount() {
    int8_t row = 0;
    User user;
    bool isUserFileEmpty = user.isUserFileEmpty();

    while (true) {
        system("cls");

        if (isUserFileEmpty) {
            getCharacter("Список пользователей пуст. Для продолжения нажмите любую кнопку");
            return;
        }
        else {
            drawMenu({"Войти в аккаунт   ", "\n\nНазад   "}, row);

            char a = getCharCode();

            if (a == VK_UP || a == VK_DOWN) row = (row + 1) % 2;
            else if (a == 13) {
                if (row == 0) user.enterAccount();
                else if (row == 1) {
                    system("cls");
                    break;
                }
                return;
            }
        }
    }
}
