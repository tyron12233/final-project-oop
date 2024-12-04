#pragma once
#include "base/view/base_view.h"
#include "models/user.h"
#include "service/auth/auth_service.h"

using namespace ftxui;

struct UserHolder {
    User *student;
    bool shown;
};

class StudentSelectorView : public BaseView {
public:
    explicit StudentSelectorView(Navigator &navigator)
        : BaseView(navigator) {
    }

    User *selectedUser = nullptr;
    std::string userTypeFilter = "student";

    std::vector<UserHolder *> users;


    Component inputQuery;
    std::string currentQuery;

    Component resultComponent;

    Component backButton;


    void setUserTypeFilter(const std::string &type) {
        userTypeFilter = type;
    }

    User *getSelecedUser() {
        return selectedUser;
    }

    vector<User *> getAllUsers() {
        if (userTypeFilter == "student") {
            return AuthService::getInstance()->getStudents();
        }
        return AuthService::getInstance()->getTeachers();
    }

    void init() override {
        const auto allUsers = getAllUsers();

        for (const auto &student: allUsers) {
            users.push_back(new UserHolder{
                .student = student,
                .shown = true,
            });
        }
    }

    void filter(const std::string &nameQuery) {
        const auto allUsers = getAllUsers();

        if (nameQuery.empty()) {
            users = std::vector<UserHolder *>();
            for (const auto &student: allUsers) {
                users.push_back(new UserHolder{
                    .student = student,
                    .shown = true,
                });
            }
            return;
        }


        // reset all students to not shown
        for (const auto &student: users) {
            student->shown = false;
        }

        for (const auto &student: users) {
            // by exact name
            if (student->student->getName() == nameQuery) {
                student->shown = true;
                continue;
            }

            // by partial name
            if (student->student->getName().find(nameQuery) != std::string::npos) {
                student->shown = true;
            }
        }

        // sort by name
        std::sort(users.begin(), users.end(), [](UserHolder *a, UserHolder *b) {
            return a->student->getName() < b->student->getName();
        });
    }

    Component getLayout() override {
        Components components;

        components.push_back(inputQuery = Input({
                                 .content = &currentQuery,
                                 .placeholder = "Student Name",
                                 .multiline = false,
                                 .on_change = [&] {
                                     filter(currentQuery);
                                 },
                             }));


        Components resultContainer;

        // copy students array


        for (const auto &student: users) {
            const auto button = Maybe(
                Button({
                    .label = student->student->getName(),
                    .on_click = [&] {
                        selectedUser = student->student;
                        screen.Exit();
                    }
                }),
                &student->shown
            );
            resultContainer.push_back(button);
        }

        resultComponent = Container::Vertical({
            resultContainer
        });

        components.push_back(resultComponent);

        backButton = Button({
            .label = "Back",
            .on_click = [&] {
                screen.Exit();
            }
        });
        components.push_back(backButton);

        return Container::Vertical(components);
    }

    Element getElement(Component layout) override {
        Elements elements;


        elements.push_back(text("Student Selector") | bold | hcenter);
        elements.push_back(separator());
        elements.push_back(inputQuery->Render() | xflex);
        elements.push_back(separator());

        if (users.empty()) {
            elements.push_back(text("No users found") | hcenter);
        } else {
            const auto test = vbox({
                                  resultComponent->Render()
                              }) | yframe | yflex;
            elements.push_back(test);
        }

        elements.push_back(separator());
        elements.push_back(backButton->Render());
        return vbox(elements) | border;
    }
};
