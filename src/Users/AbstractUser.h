#ifndef COURSEWORK_ABSTRACTUSER_H
#define COURSEWORK_ABSTRACTUSER_H

class AbstractUser;

#include <string>
#include <vector>
#include <utility>
#include "../Header.h"
#include "../../lib/json.hpp"

class AbstractUser {
protected:
    std::string login;
private:
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
    void enterPassword(bool &);
    void setCurrentUser();
    void enterAccount();
    void changePassword();
    void deleteAccount();

    static std::string enterLogin(int8_t, bool &);
    static std::string inputPassword(const std::string &);
    static std::string inputPassword();

    virtual std::string pathToData() = 0;
    void deleteUserFromFile();
    nlohmann::json userToJson();

    virtual void addUserToFile(const std::string &);

public:
    AbstractUser() = default;
    AbstractUser(std::string &login, std::string &password);

    virtual void userMenu() = 0;
    bool isUserFileEmpty();
    bool isUserLoginInFile(const std::string &);
    static const std::string &getCurrentUser();
};

#endif //COURSEWORK_ABSTRACTUSER_H
