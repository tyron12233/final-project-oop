#pragma once
#include "ftxui/component/component.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "service/course/course_service.h"
#include "service/course/course_store.h"
#include "service/grades/GradesService.h"


static std::vector<User> getStudents(const std::string &courseId) {
    auto course = CourseStore::getInstance()->getCourse(courseId);
    auto allStudents = AuthService::getInstance()->getStudents();

    vector<User> students;
    // for each student, check if they are enrolled in the course
    for (const auto &student : allStudents) {
        auto studentCourses = CourseService::getInstance()->getCoursesForStudent(student->getId());

        // if student is enrolled in the course, add to list
        for (auto &studentCourse : studentCourses) {
            if (studentCourse == courseId) {
                students.push_back(*student);
                break;
            }
        }
    }
    return students;
}

using namespace ftxui;

static void ShowGradeForStudent(const std::string &courseId, const std::string &studentId) {
    auto grades = GradesService::getInstance()->getGrades(studentId);

    auto screen = ScreenInteractive::Fullscreen();

    Components gradeComponents;

    auto backButton = Button({
        .label = "Back",
        .on_click = [&] {
            screen.Exit();
        }
    });

    const auto component = Container::Vertical(gradeComponents);
    const auto renderer = Renderer(component, [&] {
        return vbox({
            text("Showing grades for student") | hcenter,
            separator(),
            component->Render(),
            backButton->Render(),
        }) | border;
    });
    screen.Loop(renderer);
}

static void ShowAllGrades(const std::string &courseId) {
    auto students = getStudents(courseId);

    auto screen = ScreenInteractive::Fullscreen();

    Components studentComponents;
    for (auto &student : students) {
        auto studentComponent = Button({
            .on_click = [&] {
                ShowGradeForStudent(courseId, student.getId());
            },
            .transform = [&, student](const EntryState &params) {

                auto grades = GradesService::getInstance()->getGrades(student.getId());
                int total = 0;
                for (auto &grade : grades) {
                    total += grade.getTotal();
                }

                int score = 0;
                for (auto &grade : grades) {
                    score += grade.getScore();
                }

                double percentage = score / static_cast<double>(total) * 100;

                auto e = hbox({
                    text(student.getName()),
                    filler(),
                    text(to_string(percentage) + "%"),
                }) | border;
                if (params.active) {
                    e = e | inverted;
                }
                return e;
            }
        });
        studentComponents.push_back(studentComponent);
    }

    auto backButton = Button({
        .label = "Back",
        .on_click = [&] {
            screen.Exit();
        }
    });



    const auto component = Container::Vertical(studentComponents);
    const auto renderer = Renderer(component, [&] {
        return vbox({
            text("Showing grades") | hcenter,
            separator(),
            component->Render(),
        }) | border;
    });
    screen.Loop(renderer);
}
