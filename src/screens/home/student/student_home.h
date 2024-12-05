#pragma once

#include "student_home_model.h"
#include "base/view/base_view.h"
#include "screens/course/course_view.h"

using namespace ftxui;

class StudentHomeView final : public BaseView {

    StudentHomeModel *model = nullptr;

public:
    explicit StudentHomeView(Navigator &navigator)
        : BaseView(navigator) {
    }

    ~StudentHomeView() override {
        delete model;
    }

    Component courses;

    Component backButton;

    void init() override {
        model = new StudentHomeModel();

        Components components;
        for (const auto &course : model->getCourses()) {
            components.push_back(Button({
                .label = course->getName(),
                .on_click = [&, course] {
                    CourseView courseView(navigator, course->getId());
                    courseView.init();
                    courseView.render();
                },
            }));
        }
        courses = Container::Vertical(components);

        backButton = Button({
            .label = "Logout",
            .on_click = [&] {
                AuthService::getInstance()->logout();
                navigator.navigate("/");
            }
        });
    }

    Component getLayout() override {
        Components horizontals;



        horizontals.push_back(courses);

        horizontals.push_back(backButton);

        return Container::Vertical(horizontals);
    }

    Element getElement(Component layout) override {
        return vbox({
            text("Welcome, " + model->getCurrentUser()->getName()),
            separator(),

            text("Courses") | bold,
            separator(),

            courses->Render() | yframe | yflex,

            separator(),
            hbox({
                backButton->Render()
            })
        }) | border;
    }
};
