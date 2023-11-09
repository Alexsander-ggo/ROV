#include "Field.h"

#include <limits>

std::ostream& operator<<(std::ostream& out, const Field& field)
{
    field.print(out);
    return out;
}

Field::Field(size_t id,
             const std::string& name,
             TypeField type,
             size_t sizeType,
             const std::string& units,
             size_t count,
             bool isBigEndian)
    : mID(id)
    , mName(name)
    , mUnits(units)
    , mType(type)
    , mSizeType(sizeType)
    , mHasData(false)
    , mIsBigEndian(isBigEndian)
{
    mCount = (count > 0) ? count : 1;
    mSizeData = mCount * mSizeType;
    mData = new uint8_t(mSizeData);
}

Field::~Field()
{
    delete[] mData;
}

bool Field::setData(const uint8_t* data, size_t size)
{
    if (mSizeData > size) {
        return false;
    }
    for (size_t i = 0; i < mCount; ++i) {
        for (size_t j = 0; j < mSizeType; ++j) {
            size_t offset = i * mSizeType;
            size_t index = (mIsBigEndian) ? offset + mSizeType - j - 1 : j;
            mData[index] = data[offset + j];
        }
    }
    mHasData = true;
    return true;
}

size_t Field::getID() const
{
    return mID;
}

std::string Field::getName() const
{
    return mName;
}

size_t Field::getSizeData() const
{
    return mSizeData;
}

void Field::print(std::ostream& out) const
{
    out << mName << ": ";
    if (mHasData) {
        out << std::boolalpha;
        out.precision(8);
        size_t count = mCount;
        if (count > 1) {
            out << "[";
        }
        for (size_t i = 0; i < count; ++i) {
            printType(out, i);
            if (i + 1 != count) {
                out << ", ";
            }
        }
        if (count > 1) {
            out << "]";
        }
        out << " " << mUnits << std::noboolalpha;
    }
}

void Field::printType(std::ostream& out, size_t pos) const
{
    switch(mType)
    {
        case TypeField::FLOAT: {
            out << getData<float>(pos);
            break;
        }
        case TypeField::BOOL: {
            out << getData<bool>(pos);
            break;
        }
        case TypeField::CHAR: {
            out << (int)getData<char>(pos);
            break;
        }
        default:
        break;
    }
}
