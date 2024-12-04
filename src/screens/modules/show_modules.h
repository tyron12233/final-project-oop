#pragma once
#include "base/view/navigator.h"
#include "models/course.h"
#include "models/module.h"
#include "service/module/module_service.h"
#include "utils/list_view.h"

using namespace ftxui;

// used to show a module
// displays the module title and content
static void ShowModule(Navigator &navigator, const std::string &courseId, const Module &module) {
    auto screen = ScreenInteractive::Fullscreen();

    auto backButton = Button({
        .label = "Back",
        .on_click = [&] {
            screen.Exit();
        }
    });

    Components bottomComponents;
    bottomComponents.push_back(backButton);


    if (AuthService::getInstance()->getActiveUser()->getUserType() == "teacher") {
        auto deleteButton = Button({
            .label = "Delete",
            .on_click = [&] {
                bool yes = false;
                DialogView(navigator, "Delete Module", "Are you sure you want to delete this module?", [&, &yes] {
                    yes = true;
                    ModuleService::getInstance()->deleteModule(courseId, module.getTitle());
                }).render();


                if (yes) {
                    screen.Post(Task{
                        [&] {
                            screen.Exit();
                        }
                    });
                }
            }
        });
        bottomComponents.push_back(deleteButton);
    }

    const auto component = Container::Horizontal(bottomComponents);


    const auto renderer = Renderer(component, [&] {
        return vbox({
                   text(module.getTitle()) | bold,
                   separator(),
                   vbox({
                       paragraph(module.getContent())
                   }) | flex | yframe,
                   separator(),
                   hbox({
                       component->Render()
                   })
               }) | border;
    });
    screen.Loop(renderer);
}

static void ShowModules(Navigator &navigator, const std::string &courseId) {
    auto modules = ModuleService::getInstance()->getModules(courseId);

    ListView<Module> listView(navigator);
    listView.setShouldShowBackButton(true);
    listView.setItems(modules);

    ItemAdapter<Module> adapter{
        .onClick = [&, &listView](const Module &module) {
            ShowModule(navigator, courseId, module);
            ShowModules(navigator, courseId);
            listView.getScreen()->Exit();
        },
        .render = [&](const EntryState &params, const Module &module) {
            auto e = vbox({
                         text(module.getTitle()) | bold,
                     }) | border;

            if (params.focused) {
                e = e | inverted;
            }

            return e;
        }
    };
    listView.setItemRenderer(std::make_shared<ItemAdapter<Module> >(adapter));
    listView.init();
    listView.render();
}
