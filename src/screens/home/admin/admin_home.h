#pragma once

#include "list_all_courses.h"
#include "add_course/add_course.h"
#include "add_teacher/add_teacher.h"
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

        Button("Add Teacher", [&] {
            AddTeacherView view(navigator);
            view.init();
            view.render();
        }),

        Button("Enroll student", [&] {
            StudentSelectorView studentSelectorView(navigator);
            studentSelectorView.init();
            studentSelectorView.render();

            auto selectedStudent = studentSelectorView.getSelecedUser();
            if (selectedStudent == nullptr) {
                return;
            }


            vector<Course> selectedCourses{};
            vector<Course> checkedCourses{};

            auto existingCourses =CourseService::getInstance()->getCoursesForStudent(selectedStudent->getId());
            auto courses = CourseStore::getInstance()->getAllCourses();
            for (auto &course: courses) {
                bool selected = false;
                for (auto &c: existingCourses) {
                    if (c == course.getId()) {
                        selected = true;
                        break;
                    }
                }

                if (selected) {
                    selectedCourses.push_back(course);
                } else {
                    checkedCourses.push_back(course);
                }
            }



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


        }),

        Button("View Students", [&] {
            StudentSelectorView studentSelectorView(navigator);
            studentSelectorView.init();
            studentSelectorView.render();
        }),


    };

    Component getLayout() override {
        return Container::Vertical(actions);
    }

    Element getElement(Component layout) override {
        Elements elements;

        elements.push_back(text("Admin Home") | hcenter);
        elements.push_back(separator());

        for (auto &action: actions) {
            elements.push_back(action->Render());
        }

        return vbox(elements) | yframe | border;
    }
};
