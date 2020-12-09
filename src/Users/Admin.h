#ifndef COURSEWORK_ADMIN_H
#define COURSEWORK_ADMIN_H

#include "AbstractUser.h"
#include "AbstractUserImpl.h"
#include "../Data/Catalog.h"
#include "../Menu.h"

class Admin : public AbstractUser {
private:
    Admin(): AbstractUser(){};
    Admin(std::string &login, std::string&password): AbstractUser(login, password){};

    static bool deleteUserThenAddAdmin(std::string&);
    static bool deleteUser(std::string&);
    static void addUserToFileWithoutPasswordEncoding(const std::string&, const std::string&, const std::string&);
    void changeCatalog();
protected:
    std::string pathToData() override;
    void addAdmin();
    void deleteUser();
    void addUser();
    void createAccount();

public:
    friend class Menu<Admin>;
    static void adminHaveAccount();

    void userMenu() override;
};

#endif //COURSEWORK_ADMIN_H
