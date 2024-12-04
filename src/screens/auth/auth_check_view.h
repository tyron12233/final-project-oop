#pragma once
#include <utility>

#include "base/view/base_view.h"
#include "service/auth/auth_service.h"

using namespace ftxui;

class AuthCheckView final : public BaseView {
public:
    explicit AuthCheckView(Navigator &navigator)
        : BaseView(navigator) {
    }

    void init() override {
        // DEBUG PURPOSES
        // if (true) {
        //     AuthService::getInstance()->setActiveUser(
        //         *AuthService::getInstance()->getUserById("0")
        //     );
        // }
    }

    Component getLayout() override {
        return Container::Vertical({

        });
    }

    Element getElement(Component layout) override {
        return vbox({
            text("AuthCheckView"),
        });
    }

    void onLoop() override {
        const auto activeUser = AuthService::getInstance()->getActiveUser();
        if (activeUser != nullptr) {
            const auto userType = activeUser->getUserType();
            if (userType == "student") {
                navigator.navigate("/student_home");
            } else if (userType == "admin") {
                navigator.navigate("/admin_home");
            } else if (userType == "teacher") {
                navigator.navigate("/teacher_home");
            }
        } else {
            navigator.navigate("/auth");
        }
    }
};
