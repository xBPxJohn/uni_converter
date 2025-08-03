#pragma once

#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>

class FlagParser {
private:
    std::unordered_map<char, bool> shortFlags;
    std::unordered_map<std::string, bool> longFlags;
    std::unordered_map<std::string, std::string> values;
    std::unordered_map<char, std::string> shortDescriptions;
    std::unordered_map<std::string, std::string> longDescriptions;
    std::vector<std::string> positionalArgs;
    size_t numFlags{ 0 };


public:

    size_t printNumFlags() const { return numFlags; }

    void addShortFlag(char flag, const std::string& description = "") {
        shortFlags[flag] = false;
        shortDescriptions[flag] = description;
    }

    void addLongFlag(const std::string& flag, const std::string& description = "") {
        longFlags[flag] = false;
        longDescriptions[flag] = description;
    }

    void addValueFlag(const std::string& flag, const std::string& description = "") {
        values[flag] = "";
        longDescriptions[flag] = description;
    }

    void parse(int argc, char* argv[]) {
        for (int i = 1; i < argc; ++i) {
            std::string arg = argv[i];

            if (arg.rfind("--", 0) == 0) {
                // Long flag
                size_t eqPos = arg.find('=');
                if (eqPos != std::string::npos) {
                    std::string key = arg.substr(2, eqPos - 2);
                    std::string val = arg.substr(eqPos + 1);
                    if (values.count(key)) {
                        values[key] = val;
                        numFlags++;
                    }
                    else {
                        std::cerr << "Unknown long value flag: --" << key << "\n";
                    }
                }
                else {
                    std::string key = arg.substr(2);
                    if (values.count(key)) {
                        if (i + 1 < argc && argv[i + 1][0] != '-') {
                            values[key] = argv[++i];
                            numFlags++;
                        }
                        else {
                            std::cerr << "Missing value for flag: --" << key << "\n";
                        }
                    }
                    else if (longFlags.count(key)) {
                        longFlags[key] = true;
                        numFlags++;
                    }
                    else {
                        std::cerr << "Unknown long flag: --" << key << "\n";
                    }
                }
            }
            else if (arg[0] == '-' && arg.length() > 1) {
                for (size_t j = 1; j < arg.length(); ++j) {
                    char flag = arg[j];

                    if (!std::isdigit(flag))
                    {

                        if (shortFlags.count(flag)) {
                            shortFlags[flag] = true;
                            numFlags++;
                        }

                        else 
                        {
                            std::cerr << "Unknown short flag: -" << flag << "\n";
                        }
                    }
                    else
                    {
                        numFlags++;
                    }
                    
                }
            }
            else {
                positionalArgs.push_back(arg); // Anything else is a positional argument (e.g. filename)
            }
        }
    }

    bool hasShortFlag(char flag) const {
        auto it = shortFlags.find(flag);
        return it != shortFlags.end() && it->second;
    }

    bool hasLongFlag(const std::string& flag) const {
        auto it = longFlags.find(flag);
        return it != longFlags.end() && it->second;
    }

    bool hasValue(const std::string& flag) const {
        return values.find(flag) != values.end() && !values.at(flag).empty();
       
    }

    std::string getValue(const std::string& flag) const {
        auto it = values.find(flag);
        return it != values.end() ? it->second : "";
    }

    const std::vector<std::string>& getPositionalArgs() const {
        return positionalArgs;
    }

    void printUsage(const std::string& programName) const {
        std::cout << "Usage: " << programName << "  [Input...] [flags]\nFlags:\n";
        
        for (const auto& [flag, desc] : shortDescriptions) {
            std::cout << "  -" << flag << "  " << desc << '\n';
        }
        for (const auto& [flag, desc] : longDescriptions) {
            std::cout << "  --" << flag << "  " << desc << '\n';
        }
        std::cout << "\nMust only have 1 type of input flag" << std::endl;
        std::cout << "If you are passing multiple values, put quotes and seperate with spaces\nEx. \"0xFF 0xFF\"" << std::endl;
    }
};
