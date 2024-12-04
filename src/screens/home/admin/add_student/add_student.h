#pragma once
#include "base/view/base_view.h"

using namespace ftxui;

class AddStudentView : public BaseView {
public:
    explicit AddStudentView(Navigator &navigator)
        : BaseView(navigator) {
    }


    std::string email;
    Component emailInput;

    std::string name;
    Component nameInput;

    std::string studentId;

    std::vector<Course> selectedCourses;
    Component selectedCoursesComponent;

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

        selectedCoursesComponent = Button({
            .label = "",
            .on_click = [&] {
                vector<Course> checkedCourses{};


                auto courses = CourseStore::getInstance()->getAllCourses();
                ListView<Course> listView(navigator);
                listView.setItems(courses);
                listView.setShouldShowBackButton(true);

                ItemAdapter<Course> adapter{
                    .onClick = [&](Course item) {
                        bool selected = false;
                        for (auto &course: selectedCourses) {
                            if (course.getId() == item.getId()) {
                                selected = true;
                                break;
                            }
                        }

                        // toggle, if selected then remove else add it to vector
                        if (selected) {
                            selectedCourses.erase(std::remove_if(selectedCourses.begin(), selectedCourses.end(),
                                                                 [&](Course &course) {
                                                                     return course.getId() == item.getId();
                                                                 }), selectedCourses.end());
                        } else {
                            selectedCourses.push_back(item);
                        }
                    },
                    .render = [&](const EntryState &params, Course course) {
                        bool selected = false;
                        for (auto &c: selectedCourses) {
                            if (c.getId() == course.getId()) {
                                selected = true;
                                break;
                            }
                        }

                        auto e = hbox({
                                     text(selected ? " / " : "  ") | border,
                                     text(" "),
                                     text(course.getName()) | vcenter,
                                 }) | vcenter;
                        if (params.focused) {
                            e = e | inverted;
                        }

                        return e | border;
                    }
                };
                listView.setItemRenderer(std::make_shared<ItemAdapter<Course> >(adapter));
                listView.init();
                listView.render();
            },
            .transform = [&](const EntryState &state) {
                std::string msg = "Select courses";
                if (!selectedCourses.empty()) {
                    msg = "Select courses (" + std::to_string(selectedCourses.size()) + ") selected";
                }
                auto e = text(msg) | border;
                if (state.focused) {
                    e = e | inverted;
                }
                return e;
            }
        });

        createStudentButton = Button({
            .label = "Create",
            .on_click = [&, this] {
                if (!isValid()) return;
                const std::hash<std::string> hash;
                const auto password = std::to_string(hash("12345678"));

                auto user = User(name, email, studentId, password, "student");
                AuthService::getInstance()->addUser(user);


                // enroll user to the selected courses
                for (auto &course: selectedCourses) {
                    CourseService::getInstance()->enrollStudent(&user, course.getId());
                }


                DialogView(navigator, "Success", "Student account created successfully").render();

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

        if (selectedCourses.empty()) {
            return false;
        }

        return true;
    }

    Component getLayout() override {
        return Container::Vertical({
            emailInput,
            nameInput,
            selectedCoursesComponent,
            backButton,
            createStudentButton,
        });
    }

    Element getElement(Component layout) override {
        return vbox({
                   text("Create Student Account"),
                   separator(),

                   emailInput->Render() | border,
                   nameInput->Render() | border,

                   text("Generated id: " + studentId) | border,

                   text("Default password: 12345678") | border,

                   selectedCoursesComponent->Render(),

                   vbox() | flex,
                   hbox({
                       backButton->Render(),
                       createStudentButton->Render(),
                   })

               }) | border;
    }
};
