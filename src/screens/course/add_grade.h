#pragma once
#include "ftxui/component/component.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "models/grade.h"
#include "utils/integer_input.h"

using namespace ftxui;

static Grade* AddGrade(const bool edit = false, Grade *grade = nullptr) {
    auto screen = ScreenInteractive::Fullscreen();


    std::string titleInput;
    int scoreInput = 0;
    int totalInput = 0;

    auto isValid = [&] {
        if (titleInput.empty()) {
            return false;
        }

        // if score is greater than total, return false
        if (scoreInput > totalInput) {
            return false;
        }

        // if score is less than 0, return false
        if (scoreInput < 0) {
            return false;
        }

        // if total is less than 0, return false
        if (totalInput <= 0) {
            return false;
        }

        return true;
    };

    if (edit) {
        titleInput = grade->getTitle();
        scoreInput = grade->getScore();
        totalInput = grade->getTotal();
    }


    Component titleInputComponent = Input(&titleInput, "Title");
    Component scoreInputComponent = IntegerInput(&scoreInput, edit ? grade->getScore() : 0);
    Component totalInputComponent = IntegerInput(&totalInput, edit ? grade->getTotal() : 0);


    Component backButtonComponent = Button("Back", [&] {
        grade = nullptr;
        screen.Exit();
    });
    Component addButton = Button(edit ? "Save" : "Add", [&] {
        grade = new Grade(titleInput, scoreInput, totalInput);
        screen.Exit();
    }, {
        .transform = [&](const EntryState &param) {
            auto e = text(param.label) | border;
            if (!isValid()) {
                return e |= dim;
            }

            if (param.active) {
                e |= inverted;
            }
            return e;
        }
    });


    auto component = Container::Vertical({
        titleInputComponent,
        scoreInputComponent,
        totalInputComponent,
        backButtonComponent,
        addButton,
    });

    const auto renderer = Renderer(component, [&] {
        return vbox({
            text(edit ? "Edit grade" : "Input grade") | hcenter,
            separator(),

            titleInputComponent->Render() | border,

            hbox({
                text("Score: "),
                scoreInputComponent->Render() | flex,
            }) | border,

            hbox({
                text("Total: "),
                totalInputComponent->Render() | flex,
            }) | border,

            vbox({}) | yflex,

            hbox({
                backButtonComponent->Render(),
                addButton->Render(),
            })
        }) | border;
    });

    screen.Loop(renderer);
    return grade;
}
