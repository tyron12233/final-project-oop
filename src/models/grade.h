#pragma once

#include <iostream>
#include <utility>

#include "utils/string_split.h"
using namespace std;

class Grade {

    string title;
    int score;
    int total;

public:
    Grade(string title, const int score, const int total) : title(std::move(title)), score(score), total(total) {}

    [[nodiscard]] string getTitle() const {
        return title;
    }

    [[nodiscard]] int getScore() const {
        return score;
    }

    [[nodiscard]] int getTotal() const {
        return total;
    }

    [[nodiscard]] double getPercentage() const {
        return score / static_cast<double>(total) * 100;
    }

    [[nodiscard]] string getLineString() const {
        return title + ", " + to_string(score) + ", " + to_string(total);
    }

    static Grade fromLineString(const string &line) {
        const auto parts = split(line, ",");
        return {parts[0], stoi(parts[1]), stoi(parts[2])};
    }
};