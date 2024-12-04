#include "base/view/navigator.h"
#include "ftxui/component/screen_interactive.hpp"
#include "screens/auth/auth_check_view.h"
#include "screens/home/admin/admin_home.h"
#include "screens/home/admin/add_student/add_student.h"
#include "screens/home/student/student_home.h"
#include "screens/home/teacher/teacher_home.h"
#include "screens/router/router_view.h"

using namespace ftxui;

int main() {
    Navigator navigator;
    RouterView routerView(navigator);


    routerView.setRoutes({
        {"/", std::make_shared<AuthCheckView>(navigator)},
        {"/auth", std::make_shared<AuthView>(navigator)},
        {"/home", std::make_shared<AdminHomeView>(navigator)},
        {"/teacher_home", std::make_shared<TeacherHomeView>(navigator)},
        {"/student_home", std::make_shared<StudentHomeView>(navigator)},
        {"/admin_home", std::make_shared<AdminHomeView>(navigator)},
        {"/add_student", std::make_shared<AddStudentView>(navigator)}
    });


    routerView.render();


    return 0;
}
