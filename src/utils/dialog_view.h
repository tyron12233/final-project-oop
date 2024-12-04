#pragma once
#include <utility>

#include "base/view/base_view.h"

class DialogView final : public BaseView {
    std::string title;
    std::string message;
    std::function<void()> on_positive;
    std::function<void()> on_negative;

public:
    DialogView(Navigator &navigator, std::string title, std::string message)
        : DialogView(
            navigator,
            std::move(title),
            std::move(message), [this] {

            }, [this] {

            }
        ) {
    }

    DialogView(
        Navigator &navigator,
        std::string title,
        std::string message,
        std::function<void()> onPositive
    ): DialogView(
        navigator,
        std::move(title),
        std::move(message),
        std::move(onPositive),
        [this] {

        }) {
    }

    DialogView(
        Navigator &navigator,
        std::string title,
        std::string message,
        std::function<void()> onPositive,
        std::function<void()> onNegative
    ): BaseView(navigator),
       title(std::move(title)), message(std::move(message)), on_positive(std::move(onPositive)),
       on_negative(std::move(onNegative)) {
        init();
    }

    Component positive_button;
    Component negative_button;

    void init() override {
        positive_button = Button("OK", [&] {
            on_positive();
            screen.Exit();
        });

        negative_button = Button("Cancel", [&] {
            on_negative();
            screen.Exit();
        });
    }

    Component getLayout() override {
        return Container::Horizontal({
            positive_button,
            negative_button,
        });
    }

    Element getElement(Component layout) override {
        return vbox({
                   text(title),
                   separator(),
                   text(""),
                   text(message),
                   text(""),
                   hbox({
                       positive_button->Render(),
                       negative_button->Render(),
                   }),

               }) | border | center;
    }

};
