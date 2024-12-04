#pragma once

#include <sstream>

#include "auth_controller.h"
#include "auth_model.h"
#include "ftxui/dom/elements.hpp"
#include "base/view/base_view.h"
#include "utils/dialog_view.h"

using namespace ftxui;

class AuthView final : public BaseView {

    AuthModel model;
    AuthController controller;

public:
    explicit AuthView(Navigator &navigator)
        : BaseView(navigator) {
    }

private:
    void init() override {
        model.input_email = Input(&model.email, "Email Address");

        InputOption password_option;
        password_option.password = true;
        model.input_password = Input(&model.password, "Password", password_option);

        model.button_login = Button("Login", [&] {
            const bool result = controller.login(model.email, model.password);
            if (!result) {
                const std::function callback = [&] {
                    ScreenInteractive::Active()->Exit();
                };
                DialogView(navigator, "Error", "Invalid email or password", callback, callback).render();
                return;
            }

            navigator.navigate("/");
        });
    }


    Component getLayout() override {
        return Container::Vertical({
            model.input_email,
            model.input_password,
            model.button_login,
        });
    }


    Element getElement(Component layout) override {

        hash<string> hash;

        return login();
    }

    Element login() {
        return vbox({
                   text("Authentication"),
                   separator(),

                   text(""),
                   hbox({
                       text("Welcome to OOP LMS")
                   }) | center,
                   text(""),

                   hbox({
                       text("Email Address: "), model.input_email->Render()
                   }) | border,

                   hbox({
                       text("Password: "), model.input_password->Render()
                   }) | border,

                    text(""),

                     model.button_login->Render() | center,
               }) |
               border | flex;
    }
};
