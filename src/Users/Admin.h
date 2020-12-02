#ifndef COURSEWORK_ADMIN_H
#define COURSEWORK_ADMIN_H

#include "AbstractUser.h"

class Admin : public AbstractUser {
private:
    void addAdmin();

    void deleteUser();

    void addUser();

public:
    void createAccount();
};

#endif //COURSEWORK_ADMIN_H
