#pragma once

#include <fstream>
#include <map>
#include <memory>
#include <vector>

#include "models/grade.h"
#include "models/module.h"
#include "utils/string_split.h"


/**
 * Handles
 */
class GradesService {
public:
    GradesService(GradesService const &) = delete;

    void operator=(GradesService const &) = delete;


    static std::shared_ptr<GradesService> getInstance() {
        static std::shared_ptr<GradesService> instance(new GradesService());
        return instance;
    }




    std::vector<Grade> getGrades(const std::string &studentId) {
        return studentIdToGrades[studentIdToGrades];
    }



    void addGrade(const string &studentId, const Grade *grade) {
        studentIdToGrades[studentId].push_back(*grade);
        save();
    }

private:
    std::map<std::string, std::vector<Grade>> studentIdToGrades{};

    static constexpr auto DATABASE = "grades.db";

    void init() {
        ifstream file(DATABASE);
        std::string line;
        while (getline(file, line)) {
            if (line.empty()) {
                continue;
            }
            // separator is [#
            auto pos = line.find("[#");
            auto studentId = line.substr(0, pos);
            auto moduleDataString = line.substr(pos + 2);

            if (moduleDataString == "[$" || moduleDataString.empty()) {
                studentIdToGrades[studentId] = {};
                continue;
            }

            // moduleDataString is an array of modules separated by [$
            auto gradesString = split(moduleDataString, "[$");
            std::vector<Grade> grades;
            for (const auto &gradeString: gradesString) {
                grades.push_back(Grade::fromLineString(gradeString));
            }

            studentIdToGrades[studentId] = grades;
        }
    }

    void save() {
        ofstream file(DATABASE);
        for (const auto &[studentId, grades]: studentIdToGrades) {
            file << studentId << "[#";
            for (auto &grade: grades) {
                file << grade.getLineString() << "[$";
            }
            file << "\n";
        }
    }



    GradesService() {
        init();
    }
};
