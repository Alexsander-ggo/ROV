#ifndef PARAMETER_H
#define PARAMETER_H

#include <iostream>
#include <map>
#include <string>

using Table = std::map<uint64_t, std::string>;
using InvTable = std::map<std::string, uint64_t>;

class Parameter
{
public:
    Parameter(const std::string& name,
              const std::string& unit,
              uint8_t precision,
              double max,
              double min,
              double value,
              bool hasText);

    std::string getName() const;

    uint64_t getData() const;

    void setData(uint64_t data);

    double getValue() const;

    void setValue(double value);

    double getMax() const;

    void setMax(double max);

    double getMin() const;

    void setMin(double min);

    void print(std::ostream& out = std::cout,
               const Table& table = {},
               bool isVisibleName = false) const;

    void info(std::ostream& out = std::cout,
              const Table& table = {}) const;

    double textToValue(const std::string& text) const;

    bool hasText() const;

private:
    double getPrecision() const;

    double convertValue(uint64_t mValue) const;

    bool isSymbolInteger(char c) const;

    bool isSymbolDouble(char c) const;

    bool isInteger(const std::string& text) const;

    bool isDouble(const std::string& text) const;

private:
    std::string mName;
    std::string mUnit;
    uint8_t mPrecision;
    uint64_t mMax;
    uint64_t mMin;
    uint64_t mValue;
    bool mHasText;
};

#endif // PARAMETER_H
