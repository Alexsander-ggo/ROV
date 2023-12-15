#include "Missions/Mission.h"

#include "Missions/Dive.h"
#include "Missions/Lift.h"
#include "Missions/Move.h"
#include "Missions/Tack.h"
#include "Missions/Return.h"

Mission* createMission(MissionType type)
{
    switch(type)
    {
        case DIVE: return new Dive();
        case LIFT: return new Lift();
        case MOVE: return new Move();
        case TACK: return new Tack();
        case RETURN: return new Return();
        default: return nullptr;
    }
}

Mission::Mission(MissionType type,
                 const std::string& name)
    : mType(type)
    , mName(name)
{

}

std::vector<uint8_t> Mission::upload() const
{
    std::vector<uint8_t> bytes;
    for (const Setting& setting : mSettings) {
        for (const Parameter& parameter : setting.getParameters()) {
            uint64_t value = parameter.getData();
            uint8_t size = sizeof(value);
            uint8_t* buffer = (uint8_t*)&value;
            for (uint8_t i = 0; i < size; ++i) {
                bytes.push_back(buffer[i]);
            }
        }
    }
    return bytes;
}

void Mission::load(const std::vector<uint8_t>& bytes)
{
    uint8_t i = 0;
    for (Setting& setting : mSettings) {
        for (Parameter& parameter : setting.getParameters()) {
            parameter.setData(*(uint64_t*)(bytes.data() + i));
            i += sizeof(parameter.getData());
        }
    }
}

MissionType Mission::getType() const
{
    return mType;
}

void Mission::print(std::ostream& out,
                    const std::string& separator) const
{
    out << "Миссия: " << mName;
    Command::print(out, separator);
}
