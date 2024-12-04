#pragma once
#include "base/view/navigator.h"
#include "service/course/course_store.h"
#include "utils/list_view.h"

static void ListAllCourses(Navigator &navigator) {
    const auto courses = CourseStore::getInstance()->getAllCourses();
    ListView<Course> listView(navigator);
    listView.setShouldShowBackButton(true);
    listView.setItems(courses);
    listView.setItemRenderer(std::make_shared<ItemAdapter<Course> >(ItemAdapter<Course>{
        .onClick = [&](Course course) {
            std::cout << "Course clicked: " << course.getName() << std::endl;
        },
        .render = [&](const EntryState &params, Course course) {
            const auto teacher = AuthService::getInstance()->getUserById(course.getTeacherId());

            auto e = vbox({
                         text(course.getName()) | bold,
                         text("Description: " + course.getDescription()),
                         text("Professor: " + teacher->getName()),
                     }) | border;
            if (params.focused) {
                e = e | inverted;
            }
            return e;
        }
    }));
    listView.init();
    listView.render();
}
