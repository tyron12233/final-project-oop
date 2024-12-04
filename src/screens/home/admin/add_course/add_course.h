#pragma once
#include "base/view/base_view.h"
#include "service/course/course_service.h"
#include "service/course/course_store.h"
#include "utils/dialog_view.h"
#include "utils/student_selector.h"

using namespace ftxui;

class AddCourseView final : public BaseView {
public:
    explicit AddCourseView(Navigator &navigator)
        : BaseView(navigator) {
    }

    User *selectedTeacher = nullptr;
    Component selectTeacherComponent;


    std::string courseName;
    Component courseNameComponent;

    std::string description;
    Component courseDescriptionComponent;

    std::string password;
    Component passwordComponent;


    Component createButtonComponent;


    void init() override {
        selectTeacherComponent = Button({
            .label = "Select Teacher",
            .on_click = [&] {
                StudentSelectorView studentSelectorView(navigator);
                studentSelectorView.setUserTypeFilter("teacher");
                studentSelectorView.init();
                studentSelectorView.render();

                selectedTeacher = studentSelectorView.getSelecedUser();
            },
            .transform = [&](const EntryState &params) {
                auto element = text(params.label) | border;
                if (selectedTeacher != nullptr) {
                    element = text("Selected: " + selectedTeacher->getName()) | border;
                }

                if (params.focused) {
                    element |= inverted;
                }
                return element;
            }
        });

        courseNameComponent = Input({
            .content = &courseName,
            .placeholder = "Course Name",
            .multiline = false,
        });

        courseDescriptionComponent = Input({
            .content = &description,
            .placeholder = "Course description",
            .multiline = false
        });

        createButtonComponent = Button({
            .label = "Create",
            .on_click = [this] {
                if (!isValid()) {
                    return;
                }

                bool positive = false;

                DialogView dialogView(navigator, "Are you sure", "Are you sure you want to create a new course?", [this, &positive] {
                    const auto courseStore = CourseStore::getInstance().get();
                    courseStore->createCourse(courseName, description, selectedTeacher);
                    positive = true;
                });
                dialogView.render();

                if (positive) {
                    screen.Post(Task([this] {
                        DialogView(navigator, "Success", "Course created successfully").render();

                        screen.Post(Task([this] {
                            screen.Exit();
                        }));
                    }));
                }


            },
            .transform = [&](const EntryState &params) {
                auto element = text(params.label) | border;
                if (!isValid()) {
                    element |= dim;
                }
                if (params.focused) {
                    element |= inverted;
                }
                return element;
            }
        });
    }

    bool isValid() {
        if (courseName.empty()) {
            return false;
        }

        if (description.empty()) {
            return false;
        }

        return true;
    }

    Component getLayout() override {
        return Container::Vertical({
            selectTeacherComponent,
            courseNameComponent,
            courseDescriptionComponent,

            createButtonComponent,
        });
    }

    Element getElement(Component layout) override {
        return vbox({
                   text("Add course") | hcenter,
                   separator(),

                   selectTeacherComponent->Render(),

                   separator(),

                   courseNameComponent->Render() | border,
                   courseDescriptionComponent->Render() | border,

                   separator(),

                   hbox({
                        createButtonComponent -> Render() | hcenter
                   })
               }) | border;
    }
};
