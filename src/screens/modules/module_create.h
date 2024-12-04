#pragma once
#include "base/view/navigator.h"
#include "ftxui/component/component.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "models/module.h"

using namespace ftxui;

// used to create a new module
// displays a form to input module name and content
static Module* ModuleCreate() {
    std::string name;
    std::string content;
    Module *module = nullptr;

    auto screen = ScreenInteractive::Fullscreen();

    auto backButton = Button({
        .label = "Back",
        .on_click = [&] {
            screen.Exit();
        }
    });
    auto nameInput = Input({
        .content = &name,
        .placeholder = "Name",
    });
    auto contentInput = Input({
        .content = &content,
        .placeholder = "Content",
        .multiline = true,
    });
    auto createButton = Button({
        .label = "Create",
        .on_click = [&] {
            screen.Exit();
            module = new Module(name, content);
        }
    });

    auto component = Container::Vertical({
        nameInput,
        contentInput,
        backButton,
        createButton
    });
    auto renderer = Renderer(component, [&] {
        return vbox({
                   text("Create Module") | hcenter,
                   separator(),

                   nameInput->Render() | border,
                   vbox(contentInput->Render() | flex) | yflex | border,

                    hbox({
                        backButton->Render(),
                        filler(),
                        createButton->Render(),
                    }),


               }) | border;
    });

    screen.Loop(renderer);

    return module;
}
