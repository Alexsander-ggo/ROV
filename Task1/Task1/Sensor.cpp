#include "Sensor.h"

uint8_t CharToNum(char c) {
    if (c >= 'A' && c <= 'F') {
        return c - 'A' + 10;
    }
    if (c >= 'a' && c <= 'f') {
        return c - 'a' + 10;
    }
    if (c >= '0' && c <= '9') {
        return c - '0';
    }
}

uint8_t StrToByte(const std::string& str) {
    int byte = 0;
    for (char c : str) {
        byte *= 16;
        byte += CharToNum(c);
    }
    return byte;
}

std::ostream& operator<<(std::ostream& out, const Sensor& sensor)
{
    sensor.print(out);
    return out;
}

Sensor::~Sensor()
{
    for (Field* field : mFields) {
        if (field != nullptr) {
            delete field;
        }
    }
}

void Sensor::setData(const std::string& data)
{
    size_t charInByte = 2;
    size_t countByte = (mBufferText.size() + data.size()) / charInByte;
    if (countByte == 0) {
        mBufferText.insert(mBufferText.end(), data.begin(), data.end());
        return;
    }
    std::vector<uint8_t> bytes;
    for (char c : data) {
        mBufferText.push_back(c);
        if (mBufferText.size() >= charInByte) {
            bytes.push_back(StrToByte(mBufferText));
            mBufferText.clear();
        }
    }
    setData(bytes.data(), bytes.size());
}

void Sensor::setData(uint8_t* data, size_t size)
{
    mBufferByte.insert(mBufferByte.end(), data, data + size);
    for (size_t i = 0; i < mBufferByte.size(); ++i) {
        uint8_t id = mBufferByte[i];
        if (id < mIdFields.size()) {
            Field* field = mIdFields[id];
            if (field != nullptr) {
                size_t sizeData = field->getSizeData();
                size_t pos = i + 1;
                if (pos < mBufferByte.size()
                    && field->setData(mBufferByte.data() + pos, mBufferByte.size() - pos)) {
                    i += sizeData;
                } else {
                    mBufferByte.erase(mBufferByte.begin(), mBufferByte.begin() + i);
                    return;
                }
            }
        }
    }
    mBufferByte.clear();
}

Sensor& Sensor::add(Field* field)
{
    size_t id = field->getID();
    if (id >= mIdFields.size()) {
        mIdFields.resize(id + 1, nullptr);
    }
    mIdFields[id] = field;
    mFields.push_back(field);
    return *this;
}

const Field* Sensor::getField(size_t id) const
{
    if (id < mIdFields.size()) {
        return mIdFields[id];
    }
    return nullptr;
}

void Sensor::print(std::ostream& out) const
{
    for (Field* field : mFields) {
        out << *field << "\n";
    }
}
