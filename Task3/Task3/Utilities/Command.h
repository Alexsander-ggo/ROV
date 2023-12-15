#ifndef COMMAND_H
#define COMMAND_H

#include "Utilities/Parser.h"
#include "Utilities/Setting.h"

#include <iostream>
#include <map>
#include <string>
#include <vector>

using Settings = std::vector<Setting>;

class Command
{
public:
    Command() = default;

    bool setCommand(const std::string& command,
                    uint64_t from = 0, uint64_t to = -1);

    void print(std::ostream& out = std::cout,
               const std::string& separator = "\n",
               uint64_t from = 0, uint64_t to = -1) const;

    void info(std::ostream& out = std::cout,
              uint64_t from = 0, uint64_t to = -1) const;

protected:
    void addSetting(const Setting& setting,
                    const Table& valueText = {});

protected:
    Settings mSettings;
    Parser mParser;
};

#endif // COMMAND_H
