#pragma once
#include <vector>

#include "base/model/base_model.h"
#include "models/course.h"
#include "service/auth/auth_service.h"
#include "service/course/course_service.h"
#include "service/course/course_store.h"

class StudentHomeModel : BaseModel {

    User *currentUser;
    std::vector<Course*> courses{};
public:

    StudentHomeModel() {
        currentUser = AuthService::getInstance()->getActiveUser();
        auto result = CourseService::getInstance()->getCoursesForStudent(currentUser->getId());
        for (auto &courseID : result) {
            auto courseResult = CourseStore::getInstance()->getCourse(courseID);
            courses.emplace_back(new Course(courseResult));
        }
    }

    ~StudentHomeModel() {
        delete currentUser;
        for (const auto course : courses) {
            delete course;
        }
    }

    vector<Course*> getCourses() {
        return courses;
    }

    [[nodiscard]] User* getCurrentUser() const {
        return currentUser;
    }
};
