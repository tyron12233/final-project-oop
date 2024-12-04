#pragma once
#include <utility>

#include "base/view/base_view.h"
#include "screens/modules/module_create.h"
#include "screens/modules/show_modules.h"
#include "service/course/course_store.h"

class CourseView final : public BaseView {
    const std::string courseId;
    Course *course = nullptr;

public:
    explicit CourseView(Navigator &navigator, std::string courseId)
        : BaseView(navigator), courseId(std::move(courseId)) {
    }

    ~CourseView() override {
        delete course;
    }


    Component modulesButton;
    Component addModuleButton;

    Component backButton;


    void init() override {
        const auto c = CourseStore::getInstance()->getCourse(courseId);
        course = new Course(c);

        modulesButton = Button({
            .label = "View Modules",
            .on_click = [&] {
                ShowModules(navigator, courseId);
            }
        });

        if (AuthService::getInstance()->getActiveUser()->getUserType() == "teacher") {
            addModuleButton = Button({
                .label = "Add Module",
                .on_click = [&] {
                  auto module = ModuleCreate();
                    if (module != nullptr) {
                        ModuleService::getInstance()->addModule(courseId, module);
                        DialogView(navigator, "Success", "Module added successfully!").render();
                    }
                }
            });
        }


        backButton = Button({
            .label = "Back",
            .on_click = [&] {
                screen.Exit();
            }
        });
    }

    Component getLayout() override {
        Components c;

        c.push_back(modulesButton);
        if (AuthService::getInstance()->getActiveUser()->getUserType() == "teacher") {
            c.push_back(addModuleButton);
        }
        c.push_back(backButton);

        return Container::Vertical(c);
    }

    Element getElement(Component layout) override {
        Elements vboxElements;
        vboxElements.push_back(modulesButton->Render());
        if (AuthService::getInstance()->getActiveUser()->getUserType() == "teacher") {
            vboxElements.push_back(addModuleButton->Render());
        }

        return vbox({
                   text(course->getName()) | hcenter,
                   separator(),
                   text("Course description:"),
                   text(course->getDescription()),
                   separator(),

                   vbox(vboxElements) | flex,

                   hbox({
                       backButton->Render()
                   })
               }) | border;
    }
};
