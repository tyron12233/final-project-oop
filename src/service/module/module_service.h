#pragma once
#include <fstream>
#include <map>
#include <memory>
#include <vector>

#include "models/module.h"


/**
 * Handles
 */
class ModuleService {
public:
    ModuleService(ModuleService const &) = delete;

    void operator=(ModuleService const &) = delete;


    static std::shared_ptr<ModuleService> getInstance() {
        static std::shared_ptr<ModuleService> instance(new ModuleService());
        return instance;
    }


    std::vector<Module> getModules(const std::string &courseId) {
        return moduleIdToModules[courseId];
    }

    void addModule(const string &courseId, Module *module) {
        moduleIdToModules[courseId].push_back(*module);
        save();
    }

private:
    std::map<std::string, std::vector<Module> > moduleIdToModules{};

    static constexpr auto DATABASE = "modules.db";

    void init() {
        ifstream file(DATABASE);
        std::string line;
        while (getline(file, line)) {
            // separator is [#
            auto pos = line.find("[#");
            auto courseId = line.substr(0, pos);
            auto moduleDataString = line.substr(pos + 2);

            // moduleDataString is an array of modules separated by [$
            auto moduleStrings = split(moduleDataString, "[$");
            std::vector<Module> modules;
            for (const auto &moduleString: moduleStrings) {
                modules.push_back(Module::fromLine(moduleString));
            }

            moduleIdToModules[courseId] = modules;
        }
    }

    void save() {
        ofstream file(DATABASE);
        for (const auto &[courseId, modules]: moduleIdToModules) {
            file << courseId << "[#";
            for (auto &module: modules) {
                file << module.getLineString() << "[$";
            }
            file << "\n";
        }
    }

    static std::vector<std::string> split(std::string s, const std::string &delimiter) {
        size_t pos_start = 0, pos_end, delim_len = delimiter.length();
        std::string token;
        std::vector<std::string> res;

        while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
            token = s.substr(pos_start, pos_end - pos_start);
            pos_start = pos_end + delim_len;
            res.push_back(token);
        }

        res.push_back(s.substr(pos_start));
        return res;
    }

    ModuleService() {
        init();
    }
};
