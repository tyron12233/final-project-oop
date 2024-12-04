#pragma once

#include "list_all_courses.h"
#include "add_course/add_course.h"
#include "base/view/base_view.h"
#include "service/course/course_store.h"
#include "utils/list_view.h"
#include "utils/student_selector.h"

using namespace ftxui;

class AdminHomeView final : public BaseView {
public:
    explicit AdminHomeView(Navigator &navigator)
        : BaseView(navigator) {
    }

    Components actions = {
        Button("Logout", [&] {
            navigator.navigate("/auth");
        }),

        Button("List all courses", [&] {
            ListAllCourses(navigator);
        }),

        Button("Add Course", [&] {
            AddCourseView view(navigator);
            view.init();
            view.render();
        }),

        Button("Add Student", [&] {
            navigator.navigate("/add_student");
        }),

        Button("Enroll student", [&] {
            StudentSelectorView studentSelectorView(navigator);
            studentSelectorView.init();
            studentSelectorView.render();

            auto selectedStudent = studentSelectorView.getSelecedUser();
        }),

        Button("View Students", [&] {
            navigator.navigate("/view_students");
        }),


    };

    Component getLayout() override {
        return Container::Vertical(actions);
    }

    Element getElement(Component layout) override {
        Elements elements;

        for (auto &action: actions) {
            elements.push_back(action->Render());
        }

        return vbox(elements) | border;
    }
};
