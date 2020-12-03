#ifndef COURSEWORK_ABSTRACTUSER_H
#define COURSEWORK_ABSTRACTUSER_H

class AbstractUser;

#include <string>
#include <vector>
#include <utility>
#include "../Header.h"
#include "../../lib/json.hpp"

class AbstractUser {
private:
    std::string login;
    std::string password;
    inline static std::string currentUser;

protected:
    enum class Error {
        AU_NO_ERROR,
        AU_ERROR,
        AU_INTERRUPTION_ERROR
    };

    Error checkPasswords();
    bool isUserInFile();

    static std::string enterLogin(int8_t, bool &);
    static std::string inputPassword(const std::string &);
    static std::string inputPassword();
    void enterPassword(bool &);
    virtual std::string pathToData() = 0;
    virtual std::vector<AbstractUser *> *getUsersFromFile();
    virtual void addUserToFile();
    virtual void deleteUserFromFile();

public:
    AbstractUser() = default;
    AbstractUser(std::string login, std::string password);

    virtual void userMenu() = 0;
    void enterAccount();
    void changePassword();
    void deleteAccount();
    static const std::string &getCurrentUser();
    static void setCurrentUser(const std::string &currentUser);
};

#endif //COURSEWORK_ABSTRACTUSER_H
