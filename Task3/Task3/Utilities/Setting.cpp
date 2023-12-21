#include "Utilities/Setting.h"

#include "JSON/JSON_builder.h"

Setting::Setting(const std::string& name,
                 const std::string& command,
                 const std::string& description,
                 const Parameters& parameters,
                 const Action& action)
    : mName(name)
    , mCommand(command)
    , mDescription(description)
    , mParameters(parameters)
    , mAction(action)
{

}

void Setting::setText(uint64_t value,
                      const std::string& text)
{
    mTable[value] = text;
    mInvTable[text] = value;
}

bool Setting::setData(const Args& args)
{
    bool result = false;
    if (args.size() == mParameters.size() + 1) {
        if (args.front() == mCommand) {
            std::vector<double> values;
            bool flag = true;
            try {
                for (uint64_t i = 1; i < args.size(); ++i) {
                    values.push_back(mParameters.at(i - 1).textToValue(args.at(i)));
                }
            } catch (...) {
                flag = false;
            }
            if (flag) {
                for (uint64_t i = 0; i < values.size(); ++i) {
                    mParameters[i].setValue(values.at(i));
                }
                if (mAction) {
                    flag = mAction(mParameters);
                }
            }
            result = flag;
        }
    }
    return result;
}

double Setting::getValue(uint64_t index) const
{
    return mParameters.at(index).getValue();
}

const Parameter& Setting::getParameter(uint64_t index) const
{
    return mParameters.at(index);
}

Parameters& Setting::getParameters()
{
    return mParameters;
}

const Parameters& Setting::getParameters() const
{
    return mParameters;
}

void Setting::print(std::ostream& out) const
{
    out << mName << ": ";
    bool isVisibleName = mParameters.size() > 1;
    for (uint64_t i = 0; i < mParameters.size(); ++i) {
        if (i > 0) {
            out << ", ";
        }
        mParameters.at(i).print(out, mTable, isVisibleName);
    }
}

void Setting::info(std::ostream& out) const
{
    out << mDescription << '\n';
    out << "Команда: " << mCommand;
    for (const Parameter& parameter : mParameters) {
        out << " [" <<parameter.getName() << ']';
    }
    out << '\n';
    for (int i = 0; i < mParameters.size(); ++i) {
        out << i + 1 << ") Аргумент";
        mParameters.at(i).info(out, mTable);
    }
}

void Setting::load(const json::Node& node)
{
    json::Dict dict = node.asDict();
    std::string key = dict.begin()->first;
    if (key == mName) {
        json::Array array = dict.begin()->second.asArray();
        for (uint64_t i = 0; i < array.size(); ++i) {
            Parameter& parameter = mParameters[i];
            if (parameter.hasText()) {
                parameter.setValue(mInvTable.at(array.at(i).asString()));
            } else {
                parameter.setValue(array.at(i).asDouble());
            }
        }
    }
}

json::Node Setting::save() const
{
    json::Builder builder;
    builder.startDict()
           .key(mName)
           .startArray();
    for (const Parameter& parameter : mParameters) {
        if (parameter.hasText()) {
            builder.value(mTable.at(parameter.getValue()));
        } else {
            builder.value(parameter.getValue());
        }
    }
    builder.endArray()
           .endDict();
    return builder.build();
}
