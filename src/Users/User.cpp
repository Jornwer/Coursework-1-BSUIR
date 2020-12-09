#include "User.h"

std::string User::pathToData() {
    return Constants::pathToUserFile;
}

void User::userMenu() {
    setCurrentUser();
    (new Menu<User>(" Изменить каталог", &User::changeCatalog))
    ->add("\n\n Изменить пароль", &User::changePassword)
    ->add("\n\n Удалить аккаунт", &User::deleteAccount)
    ->add("\n\n Назад")
    ->addClass(*this)
    ->exit(2)
    ->exit(3)
    ->whileTrue();
}

void User::userHaveAccount() {
    User user;
    if (user.isUserFileEmpty()) {
        getCharacter("Список пользователей пуст. Для продолжения нажмите любую кнопку");
        return;
    }
    (new Menu<User>("Войти в аккаунт   ", &User::enterAccount))
    ->add("\n\nНазад   ")
    ->exit(0)
    ->exit(1)
    ->addClass(user)
    ->whileTrue();
}

void User::changeCatalog() {
    Catalog catalog;
    catalog.changeCatalog();
}
