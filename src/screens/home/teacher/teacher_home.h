#pragma once
#include "teacher_home_model.h"
#include "base/view/base_view.h"
#include "screens/course/course_view.h"

using namespace ftxui;

class TeacherHomeView : public BaseView {
    TeacherHomeModel *model = nullptr;

public:
    explicit TeacherHomeView(Navigator &navigator)
        : BaseView(navigator) {
    }

    ~TeacherHomeView() override {
        delete model;
    }

    Component courses;
    Component backButton;
    Component logoutButton;


    void init() override {
        model = new TeacherHomeModel();


        Components components;
        for (auto &course : model->getCourses()) {
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
            .label = "Back",
            .on_click = [&] {
                screen.Exit();
            }
        });

        logoutButton = Button({
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
        horizontals.push_back(logoutButton);

        return Container::Vertical(horizontals);
    }

    Element getElement(Component layout) override {
        return vbox({
            text("Teacher View"),
            separator(),

            text("Courses") | bold,
            separator(),

            courses->Render() | yframe | yflex,

            separator(),
            hbox({
                logoutButton->Render(),
            })
        }) | border;
    }
};