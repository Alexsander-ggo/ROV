#ifndef SENSOR_H
#define SENSOR_H

#include "Field.h"

#include <iostream>
#include <string>
#include <vector>

class Sensor;

std::ostream& operator<<(std::ostream& out, const Sensor& sensor);

class Sensor
{
public:
    Sensor() = default;

    Sensor(const Sensor&) = delete;

    Sensor& operator=(const Sensor&) = delete;

    ~Sensor();

    void setData(const std::string& data);

    void setData(uint8_t* data, size_t size);

    Sensor& add(Field* field);

    const Field* getField(size_t id) const;

    void print(std::ostream& out = std::cout) const;

private:
    std::string mBufferText;
    std::vector<uint8_t> mBufferByte;
    std::vector<Field*> mFields;
    std::vector<Field*> mIdFields;
};

#endif // SENSOR_H
