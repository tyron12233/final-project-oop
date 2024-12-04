#pragma once
#include "base/view/base_view.h"
#include "models/course.h"
#include "service/course/course_store.h"

struct CourseHolder {
    Course course;
    bool selected;
};

class CourseSelectView final : public BaseView {

    vector<CourseHolder> list;

public:
    explicit CourseSelectView(Navigator &navigator)
        : BaseView(navigator) {
    }

    void init() override {
        const auto allModules = CourseStore::getInstance()->getAllCourses();

        for (const auto &module: allModules) {
            list.push_back(CourseHolder{
                .course = module,
                .selected = false,
            });
        }
    }

    Component getLayout() override {
        return Container::Vertical({});
    }

    Element getElement(Component layout) override {
        return vbox({});
    }
};