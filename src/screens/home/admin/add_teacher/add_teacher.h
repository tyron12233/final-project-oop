#pragma once
#include "base/view/base_view.h"
#include "service/auth/auth_service.h"
#include "service/course/course_service.h"
#include "utils/dialog_view.h"

class Course;
using namespace ftxui;

class AddTeacherView : public BaseView {
public:
    explicit AddTeacherView(Navigator &navigator)
        : BaseView(navigator) {
    }


    std::string email;
    Component emailInput;

    std::string name;
    Component nameInput;

    std::string studentId;


    Component createStudentButton;
    Component backButton = Button({
        .label = "Back",
        .on_click = [&] {
            navigator.navigate("/admin_home");
        }
    });

    void init() override {
        const auto service = AuthService::getInstance().get();
        studentId = service->getNextId();

        emailInput = Input({
            .content = &email,
            .placeholder = "Email",
        });

        nameInput = Input({
            .content = &name,
            .placeholder = "Name",
        });

        createStudentButton = Button({
            .label = "Create",
            .on_click = [&, this] {
                if (!isValid()) return;
                const std::hash<std::string> hash;
                const auto password = std::to_string(hash("12345678"));

                const auto user = User(name, email, studentId, password, "teacher");
                AuthService::getInstance()->addUser(user);

                DialogView(navigator, "Success", "Teacher account created successfully").render();

                navigator.navigate("/admin_home");
            },
            .transform = [&](const EntryState &state) {
                auto e = text("Create") | border;
                if (!isValid()) {
                    return e | dim;
                }

                if (state.focused) {
                    e = e | inverted;
                }

                return e;
            }

        });
    }

    bool isValid() {
        if (email.empty()) {
            return false;
        }

        if (name.empty()) {
            return false;
        }

        return true;
    }

    Component getLayout() override {
        return Container::Vertical({
            emailInput,
            nameInput,
            backButton,
            createStudentButton,
        });
    }

    Element getElement(Component layout) override {
        return vbox({
                   text("Create Teacher Account"),
                   separator(),

                   emailInput->Render() | border,
                   nameInput->Render() | border,

                   text("Generated id: " + studentId) | border,

                   text("Default password: 12345678") | border,

                   vbox() | flex,
                   hbox({
                       backButton->Render(),
                       createStudentButton->Render(),
                   })

               }) | border;
    }
};
