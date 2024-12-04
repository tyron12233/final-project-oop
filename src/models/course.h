#pragma once
#include "user.h"
#include "service/auth/auth_service.h"

/**
 * Represents a learning module
 */
class Course {
    string teacherId;
    std::string id;
    std::string name;
    std::string description;

public:
    Course(const std::string &teacherId, const std::string &id, const std::string &name, const std::string &description) {
        this->teacherId = teacherId;
        this->id = id;
        this->name = name;
        this->description = description;
    }

    string getTeacherId() {
        return teacherId;
    }

    std::string getId() {
        return id;
    }

    std::string getName() {
        return name;
    }

    std::string getDescription() {
        return description;
    }

    [[nodiscard]] std::string getLineString() const {
        return teacherId + "," + id + "," + name + "," + description;
    }

    static Course fromLine(const std::string &line) {
        std::string teacherId, id, name, description;
        std::istringstream iss(line);
        std::getline(iss, teacherId, ',');
        std::getline(iss, id, ',');
        std::getline(iss, name, ',');
        std::getline(iss, description, ',');
        return {teacherId, id, name, description};
    }
};
