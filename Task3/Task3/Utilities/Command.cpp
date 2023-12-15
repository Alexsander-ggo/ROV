#include "Utilities/Command.h"

bool Command::setCommand(const std::string& command,
                         uint64_t from, uint64_t to)
{
    const Args& args = mParser.split(command);
    for (uint64_t i = from; i <= to && i < mSettings.size(); ++i) {
        if (mSettings.at(i).setData(args)) {
            return true;
        }
    }
    return false;
}

void Command::print(std::ostream& out,
                    const std::string& separator,
                    uint64_t from, uint64_t to) const
{
    for (uint64_t i = from; i <= to && i < mSettings.size(); ++i) {
        out << separator;
        mSettings.at(i).print(out);
    }
    out << '\n';
}

void Command::info(std::ostream& out,
                   uint64_t from, uint64_t to) const
{
    for (uint64_t i = from; i <= to && i < mSettings.size(); ++i) {
        out << '\n';
        mSettings.at(i).info(out);
    }
    out << '\n';
}

void Command::addSetting(const Setting& setting,
                         const Table& valueText)
{
    mSettings.push_back(setting);
    for (const auto& [value, text] : valueText) {
        mSettings.back().setText(value, text);
    }
}

