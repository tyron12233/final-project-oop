#pragma once
#include <vector>

#include "base/model/base_model.h"
#include "models/course.h"
#include "service/course/course_store.h"

class TeacherHomeModel : public BaseModel {
    std::vector<Course*> courses{};
public:
    TeacherHomeModel() {
        auto activeUser = AuthService::getInstance()->getActiveUser();
        auto coursesa = CourseStore::getInstance()->getCoursesForTeacher(activeUser->getId());
        for (auto &course : coursesa) {
            courses.emplace_back(new Course(course));
        }
    }

    std::vector<Course*> getCourses() {
        return courses;
    }
};
