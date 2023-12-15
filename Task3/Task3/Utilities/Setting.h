#ifndef SETTING_H
#define SETTING_H

#include <Utilities/Parameter.h>
#include <Utilities/Parser.h>

#include <functional>
#include <iostream>
#include <map>
#include <string>
#include <vector>

using Parameters = std::vector<Parameter>;
using Action = std::function<bool(const Parameters&)>;

class Setting
{
public:
    Setting(const std::string& name,
            const std::string& command,
            const std::string& description = "",
            const Parameters& parameters = {},
            const Action& action = {});

    void setText(uint64_t value,
                 const std::string& text);

    bool setData(const Args& args);

    double getValue(uint64_t index = 0) const;

    const Parameter& getParameter(uint64_t index = 0) const;

    Parameters& getParameters();

    const Parameters& getParameters() const;

    void print(std::ostream& out = std::cout) const;

    void info(std::ostream& out = std::cout) const;

private:
    std::string mName;
    std::string mCommand;
    std::string mDescription;
    Parameters mParameters;
    Action mAction;
    Table mTable;
};

#endif // PARAMETER_H
