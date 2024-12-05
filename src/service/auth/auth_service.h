#pragma once
#include <fstream>
#include <memory>
#include <string>
#include <vector>

#include "models/user.h"


class AuthService {
public:
    AuthService(AuthService const &) = delete;
    void operator=(AuthService const &) = delete;

    static std::shared_ptr<AuthService> getInstance() {
        static std::shared_ptr<AuthService> instance{new AuthService()};
        return instance;
    }

    void setActiveUser(const User &user) {
        activeUser = new User(user);
    }

    [[nodiscard]] std::string getNextId() const {
        return std::to_string(users.size());
    }

    void logout() {
        delete activeUser;
        activeUser = nullptr;
    }


    [[nodiscard]] User *getActiveUser() const {
        return activeUser;
    }

    void addUser(User user) {
        users.push_back(user);
        saveUsers();
    }

    /**
     * Tries to login the user with the given email and password
     * @return the user if the login was successful, nullptr otherwise
     */
    User* tryLogin(const std::string &email, const std::string &password) {
        // hash password
        constexpr std::hash<std::string> hash;
        const auto hashedPassword = std::to_string(hash(password));


        for (auto &user : users) {
            if (user.getEmail() == email && user.getPasswordHash() == hashedPassword) {
                return &user;
            }
        }

        return nullptr;
    }

    User * getUserById(const std::string &id) {
        for (auto &user : users) {
            if (user.getId() == id) {
                return &user;
            }
        }

        return nullptr;
    }

    vector<User*> getStudents() {
        vector<User*> students;
        for (auto &user : users) {
            if (user.getUserType() == "student") {
                students.push_back(&user);
            }
        }
        return students;
    }

    vector<User*> getTeachers() {
        vector<User*> teachers;
        for (auto &user : users) {
            if (user.getUserType() == "teacher") {
                teachers.push_back(&user);
            }
        }
        return teachers;
    }

private:
    std::vector<User> users{};
    User *activeUser = nullptr;

    void readUsers() {
        ifstream file("users.db");
        std::string line;
        while (getline(file, line)) {
            users.push_back(User::fromLine(line));
        }
    }

    void saveUsers() const {
        // if file does not exist, it will be created
        ofstream file("users.db");
        for (const auto &user: users) {
            file << user.getLineString() << endl;
        }
        file.close();
    }


    AuthService() {
        readUsers();
    }
};
