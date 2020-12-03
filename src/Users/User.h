#ifndef COURSEWORK_USER_H
#define COURSEWORK_USER_H

#include "AbstractUser.h"

class User : public AbstractUser {
private:
    User(): AbstractUser(){};
protected:
    std::string pathToData() override;

public:
    void userMenu() override;

    static void userHaveAccount();
};

#endif //COURSEWORK_USER_H
