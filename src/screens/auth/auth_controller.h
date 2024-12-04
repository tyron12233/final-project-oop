#pragma once

#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "base/controller/base_controller.h"
#include "models/user.h"
#include "service/auth/auth_service.h"

using namespace std;

class AuthController: public BaseController {

public:

    bool login(const string &email, const string &password) {
        const User* user = AuthService::getInstance()->tryLogin(email, password);
        if (user != nullptr) {
            AuthService::getInstance()->setActiveUser(*user);
            return true;
        }
        return false;
    }


};