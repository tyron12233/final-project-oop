cmake_minimum_required(VERSION 3.28)

project(LayoutEngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# --- Fetch FTXUI --------------------------------------------------------------
include(FetchContent)

set(FETCHCONTENT_UPDATES_DISCONNECTED ON)
set(FETCHCONTENT_QUIET OFF)

FetchContent_Declare(ftxui
        GIT_REPOSITORY https://github.com/arthursonzogni/ftxui.git
        GIT_TAG v5.0.0
        GIT_PROGRESS TRUE
        GIT_SHALLOW TRUE
        EXCLUDE_FROM_ALL
)
FetchContent_MakeAvailable(ftxui)
# ------------------------------------------------------------------------------


add_executable(LayoutEngine src/main.cpp
        src/screens/auth/auth_view.h
        src/base/view/base_view.h
        src/base/model/base_model.h
        src/screens/auth/auth_model.h
        src/screens/auth/auth_controller.h
        src/base/controller/base_controller.h
        src/models/user.h
        src/utils/dialog_view.h
        src/screens/router/router_view.h
        src/base/view/navigator.h
        src/screens/auth/auth_check_view.h
        src/service/auth/auth_service.h
        src/screens/home/admin/admin_home.h
        src/screens/home/student/student_home.h
        src/screens/home/student/student_home_model.h
        src/models/course.h
        src/service/course/course_service.h
        src/utils/student_selector.h
        src/service/course/course_store.h
        src/screens/home/admin/course_select.h
        src/screens/home/admin/add_course/add_course.h
        src/utils/list_view.h
        src/screens/home/admin/list_all_courses.h
        src/screens/home/admin/add_student/add_student.h
        src/screens/course/course_view.h
        src/service/module/module_service.h
        src/models/module.h
        src/screens/modules/show_modules.h
        src/screens/home/teacher/teacher_home.h
        src/screens/home/teacher/teacher_home_model.h
        src/screens/modules/module_create.h
        src/screens/home/admin/add_teacher/add_teacher.h
        src/screens/course/show_grades.h
        src/models/grade.h
        src/utils/string_split.h
        src/service/grades/GradesService.h
        src/screens/course/add_grade.h
        src/utils/integer_input.h
)
target_include_directories(LayoutEngine PRIVATE src)

target_link_libraries(LayoutEngine
        PRIVATE ftxui::screen
        PRIVATE ftxui::dom
        PRIVATE ftxui::component  # Not needed for this example.
)

