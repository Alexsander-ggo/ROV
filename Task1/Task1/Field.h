#ifndef FIELD_H
#define FIELD_H

#include <iostream>
#include <string>

enum TypeField
{
    FLOAT,
    BOOL,
    CHAR
};

class Field;

std::ostream& operator<<(std::ostream& out, const Field& field);

class Field
{
public:
    Field(size_t id,
          const std::string& name,
          TypeField type,
          size_t sizeType,
          const std::string& units = "",
          size_t count = 1,
          bool isBigEndian = true);

    Field(const Field& other) = delete;

    Field& operator=(const Field& other) = delete;

    ~Field();

    bool setData(const uint8_t* data, size_t size);

    template<typename T>
    T getData(size_t pos = 0) const;

    size_t getID() const;

    std::string getName() const;

    size_t getSizeData() const;

    void print(std::ostream& out = std::cout) const;

private:
    void printType(std::ostream& out, size_t pos) const;

private:
    size_t mID;
    std::string mName;
    std::string mUnits;
    TypeField mType;
    size_t mSizeType;
    size_t mCount;
    uint8_t* mData;
    size_t mSizeData;
    bool mHasData;
    bool mIsBigEndian;
};

template<typename T>
T Field::getData(size_t pos) const
{
    size_t size = sizeof(T);
    size_t index = (pos < mCount) ? pos * size : 0;
    if (index + size > mSizeData) {
        return T();
    }
    return *(T*)(mData + index);
}

#endif // FIELD_H
