#pragma once
#include "base/view/base_view.h"


template<typename T>
struct ItemAdapter {
    std::function<void(T item)> onClick;
    std::function<Element(const EntryState &params, T item)> render;
};

using namespace ftxui;

template<typename T>
class ListView : public BaseView {
    std::vector<T> items;
    std::shared_ptr<ItemAdapter<T> > itemAdapter;
    bool showBackButton = false;

    Component list;
    Component backButton;

public:
    explicit ListView(Navigator &navigator)
        : BaseView(navigator) {
    }

    void setShouldShowBackButton(bool showBackButton) {
        this->showBackButton = showBackButton;
    }

    void setItems(std::vector<T> items) {
        this->items = items;
    }

    void setItemRenderer(std::shared_ptr<ItemAdapter<T> > itemAdapter) {
        this->itemAdapter = itemAdapter;
    }

    void init() override {
        Components components;

        for (auto &item: items) {
            components.push_back(Button({
                .label = "",
                .on_click = [&] {
                    itemAdapter->onClick(item);
                },
                .transform = [&](const EntryState &params) {
                    return itemAdapter->render(params, item);
                }
            }));
        }
        list = Container::Vertical(components);
    }

    ScreenInteractive* getScreen() {
        return &screen;
    }

    Component getLayout() override {
        Components components;

        components.push_back(list);

        if (showBackButton) {
            backButton = Button("Back", [&] {
                screen.Exit();
            });
            components.push_back(backButton);
        }

        return Container::Vertical(components);
    }

    Element getElement(Component layout) override {
        Elements elements;

        elements.push_back({
            vbox({
                list->Render()
            }) | yframe | yflex
        });

        if (showBackButton) {
            elements.push_back(separator());
            elements.push_back(backButton->Render());
        }


        return vbox(elements) | border;
    }
};
