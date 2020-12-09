#ifndef COURSEWORK_USER_H
#define COURSEWORK_USER_H

#include "AbstractUser.h"
#include "../Data/Catalog.h"
#include "../Menu.h"

class User : public AbstractUser {
private:
    User(): AbstractUser(){};
    void changeCatalog();

protected:
    std::string pathToData() override;

public:
    friend class Menu<User>;
    void userMenu() override;
    static void userHaveAccount();
};

#endif //COURSEWORK_USER_H
