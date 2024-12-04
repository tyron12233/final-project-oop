#pragma once

#include <sstream>

using namespace std;

/**
 * The basic class stored in the database
 */
class User {
private:
    string email;
    string name;
    string id;
    string passwordHash;
    string userType;

public:

    User(const string &name, const string&email, const string &id, const string &passwordHash, const string &userType) {
        this->name = name;
        this->email = email;
        this->id = id;
        this->passwordHash = passwordHash;
        this->userType = userType;
    }





    string getName() {
        return name;
    }

    string getEmail() {
        return email;
    }

    string getId() {
        return id;
    }

    string getPasswordHash() {
        return passwordHash;
    }

    string getUserType() {
        return userType;
    }

    [[nodiscard]] string getLineString() const {
        return name + "," + email + "," + id + "," + passwordHash + "," + userType;
    }

    static User fromLine(const string &line) {
        string email, name, id, passwordHash, userType;
        istringstream iss(line);
        getline(iss, name, ',');
        getline(iss, email, ',');
        getline(iss, id, ',');
        getline(iss, passwordHash, ',');
        getline(iss, userType, ',');
        return {name, email, id, passwordHash, userType};
    }
};