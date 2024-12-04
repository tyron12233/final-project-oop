#pragma once
#include <fstream>
#include <map>
#include <memory>
#include <vector>

#include "models/module.h"
#include "utils/string_split.h"


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

    void editModule(const std::string &courseId, const std::string &moduleId, const Module *module) {
        auto &modules = moduleIdToModules[courseId];
        for (auto &m: modules) {
            if (m.getTitle() == moduleId) {
                m = *module;
                break;
            }
        }
        save();
    }

    void deleteModule(const std::string &courseId, const std::string &moduleId) {
        auto &modules = moduleIdToModules[courseId];
        modules.erase(std::remove_if(modules.begin(), modules.end(), [&](const Module &module) {
            return module.getTitle() == moduleId;
        }), modules.end());
        save();
    }

    void addModule(const string &courseId, const Module *module) {
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
            if (line.empty()) {
                continue;
            }
            // separator is [#
            auto pos = line.find("[#");
            auto courseId = line.substr(0, pos);
            auto moduleDataString = line.substr(pos + 2);

            if (moduleDataString == "[$" || moduleDataString.empty()) {
                moduleIdToModules[courseId] = {};
                continue;
            }

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



    ModuleService() {
        init();
    }
};
