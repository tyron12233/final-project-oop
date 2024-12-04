#pragma once
#include <fstream>
#include <memory>
#include <vector>

#include "models/course.h"

/**
 * Responsible for storing and managing courses
 */
class CourseStore {

public:
    CourseStore(CourseStore const &) = delete;
    void operator=(CourseStore const &) = delete;

    static std::shared_ptr<CourseStore> getInstance() {
        static std::shared_ptr<CourseStore> instance{new CourseStore()};
        return instance;
    }



    vector<Course> getCoursesForTeacher(const std::string &teacherId) {
        vector<Course> teacherCourses;
        for (auto &course : courses) {
            if (course.getTeacherId() == teacherId) {
                teacherCourses.push_back(course);
            }
        }

        return teacherCourses;
    }

    vector<Course> getAllCourses() {
        return courses;
    }

    Course getCourse(const std::string &courseId) {
        for (auto &course : courses) {
            if (course.getId() == courseId) {
                return course;
            }
        }

        throw std::runtime_error("Course not found");
    }

    void createCourse(const std::string &name, const std::string &description, User *teacher) {
        Course course(teacher->getId(), std::to_string(courses.size()), name, description);
        courses.emplace_back(course);
        save();
    }

private:

    static constexpr auto DATABASE = "courses.db";
    std::vector<Course> courses{};

    void init() {
        // load courses from database
        if (std::ifstream file(DATABASE); file.is_open()) {
            std::string line;
            while (getline(file, line)) {
                courses.push_back(Course::fromLine(line));
            }
        }
    }

    void save() {
        std::ofstream file(DATABASE);
        for (const auto &course : courses) {
            file << course.getLineString() << std::endl;
        }
    }


    CourseStore() {
        init();
    }
};