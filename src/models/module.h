#pragma once

#include <iostream>
#include <sstream>

using namespace std;


class Module {

    string title;
    string content;

public:
    Module(const string &title, const string &content) {
        this->title = title;
        this->content = content;
    }

    string getTitle() const {
        return title;
    }

    string getContent() const {
        return content;
    }


    // encode as a string
    string getLineString() const {
        // remove newlines from content, as they are not allowed in csv
        // replace them with \n
        string contentCopy = content;
        size_t pos = 0;
        while ((pos = contentCopy.find('\n', pos)) != string::npos) {
            contentCopy.replace(pos, 1, "$n");
            pos += 2;
        }


        return title + "," + contentCopy;
    }

    static Module fromLine(const string &line) {
        string title, content;
        istringstream iss(line);
        getline(iss, title, ',');
        getline(iss, content, ',');

        // replace $n with newline
        size_t pos = 0;
        while ((pos = content.find("$n", pos)) != string::npos) {
            content.replace(pos, 2, "\n");
            pos += 1;
        }
        return {title, content};
    }
};