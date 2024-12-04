#pragma once

#include "base/model/base_model.h"
#include "ftxui/component/component.hpp"

using namespace ftxui;

class AuthModel : public BaseModel {
public:
    std::string email;
    Component input_email;

    std::string password;
    Component input_password;

    Component button_login;
};