#pragma once
#include <functional>
#include <sstream>
#include <stack>
#include <stdexcept>
#include <string>


class Navigator {
    std::function<bool(const std::string &)> onNavigate;
    std::stack<std::string> history;
public:

    Navigator() {
        history.emplace("/");
    }

    void setOnNavigate(const std::function<bool(const std::string &)> &onNavigate) {
        this->onNavigate = onNavigate;
    }



    void navigate(const std::string &path) {
        if (onNavigate == nullptr) {
            throw std::invalid_argument("BUG: Navigate is called before onNavigate is set.");
        }

        history.push(path);


        const bool result = onNavigate(path);
        if (!result) {
            throw std::invalid_argument("Invalid path, no route found: " + path);
        }
    }




    [[nodiscard]] std::string getPath() const {
        std::stringstream ss;

        // copy stack, so we can iterate over it
        std::stack<std::string> copy = history;
        while (!copy.empty()) {
            ss << copy.top() << "/";
            copy.pop();
        }

        return ss.str();
    }

    void pop() {
        if (history.empty()) {
            throw std::invalid_argument("Cannot pop from root.");
        }

        history.pop();
        navigate(getPath());
    }


};
