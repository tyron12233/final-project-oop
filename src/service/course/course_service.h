#pragma once
#include <fstream>
#include <map>
#include <memory>
#include <vector>

#include "models/user.h"

using namespace std;

class CourseService {
public:
    CourseService(CourseService const &) = delete;

    void operator=(CourseService const &) = delete;

    static shared_ptr<CourseService> getInstance() {
        static std::shared_ptr<CourseService> instance{new CourseService()};
        return instance;
    }

    std::vector<std::string> getCoursesForStudent(const std::string &studentId) {
        return studentToCoursesMap[studentId];
    }

    void enrollStudent(User *user, const std::string &courseId) {
        studentToCoursesMap[user->getId()].push_back(courseId);
        save();
    }

    void unenrollStudent(User *user, const std::string &courseId) {
        auto &studentCourses = studentToCoursesMap[user->getId()];
        auto it = std::find(studentCourses.begin(), studentCourses.end(), courseId);
        if (it != studentCourses.end()) {
            studentCourses.erase(it);
        }

        save();
    }



private:
    // map of student id -> list of course id;s
    std::map<std::string, vector<std::string> > studentToCoursesMap{};

    static constexpr auto DATABASE = "student_courses.db";

    CourseService() {
        init();
    }

    void init() {
        // load student courses from database (txt file, saved line by line)
        if (ifstream file(DATABASE); file.is_open()) {
            string line;
            while (getline(file, line)) {
                // split by comma, first element is student id, rest are course ids
                auto pos = line.find(',');
                auto studentId = line.substr(0, pos);
                auto courseIds = line.substr(pos + 1);

                studentToCoursesMap.emplace(studentId, vector<string>{});
                auto &courses = studentToCoursesMap[studentId];

                // split course ids by comma
                pos = 0;
                while ((pos = courseIds.find(',')) != string::npos) {
                    auto courseId = courseIds.substr(0, pos);
                    courses.push_back(courseId);
                    courseIds.erase(0, pos + 1);
                }
            }
            file.close();
        }
    }

    void save() {
        ofstream file(DATABASE);
        for (const auto &[studentId, courses]: studentToCoursesMap) {
            file << studentId << ',';
            for (const auto &courseId: courses) {
                file << courseId << ',';
            }
            file << '\n';
        }
        file.close();
    }
};
