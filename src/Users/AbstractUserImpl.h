#ifndef COURSEWORK_ABSTRACTUSERIMPL_H
#define COURSEWORK_ABSTRACTUSERIMPL_H

#include <utility>
#include "AbstractUser.h"

namespace core {
    class AbstractUserImpl : public AbstractUser {
    public:
        void userMenu() override {}

        void enterCredentials(bool &leave) {
            login = enterLogin(0, leave);
            if (leave) return;
            this->enterPassword(leave);
            if (leave) return;
            if (this->isUserInFile()) userMenu();
        }
        void addUserToFile(const std::string& path){
            AbstractUser::addUserToFile(path);
        }
    protected:
        std::string pathToData() override {
            return "";
        }
    };
}
#endif //COURSEWORK_ABSTRACTUSERIMPL_H
