#pragma once

#include <map>
#include <utility>

#include "base/view/base_view.h"
#include "base/view/navigator.h"
#include "base/view/navigator.h"
#include "screens/auth/auth_view.h"

class Navigator;
using namespace ftxui;

class RouterView {

    std::string path = "/";
    std::map<std::string, std::shared_ptr<BaseView>> routes;
    Navigator navigator;

public:

    explicit RouterView(Navigator &navigator) : navigator(navigator) {
        navigator.setOnNavigate([&](const std::string &path) {
            return onNavigate(path);
        });
    }

    bool onNavigate(const std::string &path) {
        if (routes.find(path) == routes.end()) {
            return false;
        }

        this->path = path;

        ScreenInteractive::Active()->Exit();
        return true;
    }


    void setRoutes(std::map<std::string, std::shared_ptr<BaseView>> routes) {
        this->routes = std::move(routes);
    }

   void render() {
        while (true) {
            const auto &view = routes[path];
            if (view == nullptr) {
                throw std::invalid_argument("Invalid path, no route found: " + path);
            }

            view->init();
            view->render();
        }
    }
};
