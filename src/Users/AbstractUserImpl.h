#ifndef COURSEWORK_ABSTRACTUSERIMPL_H
#define COURSEWORK_ABSTRACTUSERIMPL_H

#include <utility>
#include "AbstractUser.h"

namespace core {
    class AbstractUserImpl : public AbstractUser {
    public:
        AbstractUserImpl(std::string login, std::string password) : AbstractUser(std::move(login),
                                                                                 std::move(password)) {
        }

        void userMenu() override {
        }

    protected:
        std::string pathToData() override {
            return "";
        }
    };
}
#endif //COURSEWORK_ABSTRACTUSERIMPL_H
