#pragma once
#include "base/view/navigator.h"
#include "models/course.h"
#include "models/module.h"
#include "service/module/module_service.h"
#include "utils/list_view.h"

using namespace ftxui;

static void ShowModule(const Module &module) {
    auto screen = ScreenInteractive::Fullscreen();

    auto backButton = Button({
        .label = "Back",
        .on_click = [&] {
            screen.Exit();
        }
    });

    auto component = Container::Vertical({
        backButton
    });

    const auto renderer = Renderer(component, [&] {
        return vbox({
                   text(module.getTitle()) | bold,
                   separator(),
                   vbox({
                       text(module.getContent())
                   }) | flex,
                   separator(),
                   hbox({
                       backButton->Render()
                   })
               }) | border;
    });
    screen.Loop(renderer);
}

static void ShowModules(Navigator &navigator, const std::string &courseId) {
    const auto modules = ModuleService::getInstance()->getModules(courseId);

    ListView<Module> listView(navigator);
    listView.setShouldShowBackButton(true);
    listView.setItems(modules);

    ItemAdapter<Module> adapter{
        .onClick = [&](const Module &module) {
            ShowModule(module);
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



