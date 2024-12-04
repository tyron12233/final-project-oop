#pragma once

#include <utility>

#include "navigator.h"
#include "ftxui/component/component.hpp"
#include "ftxui/component/loop.hpp"
#include "ftxui/dom/elements.hpp"
#include "ftxui/component/screen_interactive.hpp"

using namespace ftxui;

class BaseView {
protected:
    ScreenInteractive screen;
    Navigator &navigator;

public:
    explicit BaseView(Navigator &navigator)
        : screen(ScreenInteractive::Fullscreen()), navigator(navigator) {
    }

    virtual ~BaseView() = default;


    /**
     * Used for initializing the view, setting up components, etc.
     */
    virtual void init() {
    }


    /**
     * The layout is the structure of the view, it is used to define the direction of input
     */
    virtual Component getLayout() = 0;

    /**
     * This is what actually gets rendered to the screen
     */
    virtual Element getElement(Component layout) = 0;


    /**
     * Renders the view, handles ftxui screen logic
     */
    virtual void render() {
        const auto layout = getLayout();
        const auto renderer = Renderer(layout, [&] {
            return getElement(layout);
        });

        Loop loop(&screen, renderer);

            while (!loop.HasQuitted()) {
            onLoop();
            loop.RunOnce();
        }
    }

    virtual void onLoop() {}
};
