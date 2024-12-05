#pragma once
#include <iomanip>

#include "add_grade.h"
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
    for (const auto &student: allStudents) {
        auto studentCourses = CourseService::getInstance()->getCoursesForStudent(student->getId());

        // if student is enrolled in the course, add to list
        for (auto &studentCourse: studentCourses) {
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
    auto grades = GradesService::getInstance()->getGrades(studentId, courseId);

    auto screen = ScreenInteractive::Fullscreen();

    Components gradeComponents;
    Components bottomComponents;

    for (auto &grade: grades) {
        auto gradeComponent = Button({
            .on_click = [&] {
                // is teacher, show edit grade dialog
                if (AuthService::getInstance()->getActiveUser()->getUserType() == "teacher") {
                    auto newGrade = AddGrade(true, new Grade(grade));
                    if (newGrade != nullptr) {
                        GradesService::getInstance()->editGrade(studentId, courseId, grade, newGrade);
                        ShowGradeForStudent(courseId, studentId);
                        screen.Exit();
                    }
                }
            },
            .transform = [&](const EntryState &params) {
                auto e = hbox({
                             text(grade.getTitle()),
                             filler(),
                             text(to_string(grade.getScore()) + "/" + to_string(grade.getTotal())),
                         }) | border;
                if (params.focused) {
                    e = e | inverted;
                }
                return e;
            }
        });
        gradeComponents.push_back(gradeComponent);
    }

    auto backButton = Button({
        .label = "Back",
        .on_click = [&] {
            screen.Exit();
        }
    });
    bottomComponents.push_back(backButton);

    const bool isTeacher = AuthService::getInstance()->getActiveUser()->getUserType() == "teacher";
    if (isTeacher) {
        Component addGradeButton = Button({
            .label = "Add Grade",
            .on_click = [&] {
                const auto grade = AddGrade();
                if (grade != nullptr) {
                    GradesService::getInstance()->addGrade(studentId, courseId, grade);
                    ShowGradeForStudent(courseId, studentId);

                    screen.Exit();
                }
            }
        });
        bottomComponents.push_back(addGradeButton);
    }

    const auto bottomComponent = Container::Horizontal(bottomComponents);
    const auto gradeComponent = Container::Vertical(gradeComponents);

    const auto component = Container::Vertical({
        gradeComponent,
        bottomComponent
    });


    const auto renderer = Renderer(component, [&] {
        int total = 0;
        int score = 0;

        string message = "No grades";
        if (!grades.empty()) {
            for (auto &grade: grades) {
                total += grade.getTotal();
                score += grade.getScore();
            }

            double percentage = score / static_cast<double>(total) * 100;
            stringstream ss;
            ss << fixed << setprecision(2) << percentage;
            ss >> percentage;

            message = "Percentage: " + to_string(percentage) + "%";
        }

        return vbox({
                   text("Showing grades for student") | hcenter,
                   separator(),

                   gradeComponent->Render() | yframe | flex,
                   separator(),

                   hbox({
                       filler(),
                       text(message),
                   }),
                   separator(),

                   bottomComponent->Render(),

               }) | border;
    });
    screen.Loop(renderer);
}

static void ShowAllGrades(const std::string &courseId) {
    auto students = getStudents(courseId);

    auto screen = ScreenInteractive::Fullscreen();

    Components studentComponents;
    for (auto &student: students) {
        auto studentComponent = Button({
            .on_click = [&] {
                ShowGradeForStudent(courseId, student.getId());
            },
            .transform = [&, &student](const EntryState &params) {
                string msg = "No grades";
                auto grades = GradesService::getInstance()->getGrades(student.getId(), courseId);

                if (!grades.empty()) {
                    int total = 0;
                    for (auto &grade: grades) {
                        total += grade.getTotal();
                    }

                    int score = 0;
                    for (auto &grade: grades) {
                        score += grade.getScore();
                    }

                    double percentage = score / static_cast<double>(total) * 100;
                    stringstream ss;
                    ss << fixed << setprecision(2) << percentage;
                    ss >> percentage;

                    msg = ss.str() + "%";
                }

                auto e = hbox({
                             text(student.getName()),
                             filler(),
                             text(msg),
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


    const auto sutdentComponent = Container::Vertical(studentComponents);
    const auto component = Container::Vertical({
        sutdentComponent,
        backButton
    });
    const auto renderer = Renderer(component, [&] {
        return vbox({
                   text("Showing grades") | hcenter,
                   separator(),
                   sutdentComponent->Render() | yframe,
                   separator(),
                   backButton->Render(),
               }) | border;
    });
    screen.Loop(renderer);
}
