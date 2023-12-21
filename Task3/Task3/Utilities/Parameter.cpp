#include "Utilities/Parameter.h"

#include <cmath>

Parameter::Parameter(const std::string& name,
                     const std::string& unit,
                     uint8_t precision,
                     double max,
                     double min,
                     double value,
                     bool hasText)
    : mName(name)
    , mUnit(unit)
    , mPrecision(precision)
    , mHasText(hasText)
{
    setMax(max);
    setMin(min);
    setValue(value);
}

std::string Parameter::getName() const
{
    return mName;
}

uint64_t Parameter::getData() const
{
    return mValue;
}

void Parameter::setData(uint64_t data)
{
    mValue = data;
}

double Parameter::getValue() const
{
    return convertValue(mValue);
}

void Parameter::setValue(double value)
{
    if (value >= getMin() && value <= getMax()) {
        mValue = value * getPrecision();
    }
}

double Parameter::getMax() const
{
    return convertValue(mMax);
}

void Parameter::setMax(double max)
{
    mMax = max * getPrecision();
}

double Parameter::getMin() const
{
    return convertValue(mMin);
}

void Parameter::setMin(double min)
{
    mMin = min * getPrecision();
}

void Parameter::print(std::ostream& out,
                      const Table& table,
                      bool isVisibleName) const
{
    if (isVisibleName) {
        out << mName;
        if (!mName.empty()) {
            out << " = ";
        }
    }
    double value = getValue();
    if (mHasText && table.count(value)) {
        out << table.at(value);
    } else {
        out << value;
    }
    if (!mUnit.empty()) {
        out << ' ' << mUnit;
    }
}

void Parameter::info(std::ostream& out,
                     const Table& table) const
{
    if (!mUnit.empty()) {
        out << '(' << mUnit << ')';
    }
    if (!mName.empty()) {
        out << ' ' << mName;
    }
    double precision = getPrecision();
    out << ": диапазон от " << getMin() << " до " << getMax();
    out << " c точностью до ";
    if (precision == 1)  {
        out << "целых\n";
    } else {
        out << 1.0 / precision << '\n';
    }
    if (mHasText && !table.empty()) {
        out << "  (";
        for (const auto& [value, text] : table) {
            out << value << " - " << text << ", ";
        }
        out << "\b\b";
        out << ")\n";
    }
}

double Parameter::textToValue(const std::string& text) const
{
    double value;
    if (mPrecision == 0) {
        if (isInteger(text)) {
            value = std::stoi(text);
        } else {
            throw 0;
        }
    } else {
        if (isDouble(text)) {
            value = std::stod(text);
        } else {
            throw 0;
        }
    }
    if (value < getMin() || value > getMax()) {
        throw 0;
    }
    return value;
}

bool Parameter::hasText() const
{
    return mHasText;
}

double Parameter::getPrecision() const
{
    return std::pow(10, mPrecision);
}

double Parameter::convertValue(uint64_t value) const
{
    return value / getPrecision();
}

bool Parameter::isSymbolInteger(char c) const
{
    return (c >= '0' && c <= '9') || c == '-';
}

bool Parameter::isSymbolDouble(char c) const
{
    return isSymbolInteger(c) || c == '.';
}

bool Parameter::isInteger(const std::string& text) const
{
    for (char c : text) {
        if (!isSymbolInteger(c)) {
            return false;
        }
    }
    return true;
}

bool Parameter::isDouble(const std::string& text) const
{
    for (char c : text) {
        if (!isSymbolDouble(c)) {
            return false;
        }
    }
    return true;
}
