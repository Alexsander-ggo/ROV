#include "Missions/Mission.h"

#include "JSON/JSON_builder.h"
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

Mission* createMission(const json::Node& node)
{
    static const std::map<std::string, MissionType> missionType = {
        {"погружение", DIVE}, {"всплытие", LIFT}, {"движение", MOVE},
        {"галс", TACK}, {"возвращение", RETURN}
    };
    json::Array array = node.asArray();
    Mission* mission = createMission(missionType.at(array.front().asDict().at("Миссия").asString()));
    mission->load(node);
    return mission;
}

Mission::Mission(MissionType type,
                 const std::string& name)
    : mType(type)
    , mName(name)
{

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

void Mission::load(const json::Node& node)
{
    json::Array array = node.asArray();
    if (array.front().asDict().at("Миссия") == mName) {
        for (uint64_t i = 1; i < array.size(); ++i) {
            mSettings.at(i - 1).load(array.at(i));
        }
    }
}

json::Node Mission::save() const
{
    json::Builder builder;
    builder.startArray()
           .startDict()
           .key("Миссия")
           .value(mName)
           .endDict();
    for (const Setting& setting : mSettings) {
        builder.value(setting.save().getValue());
    }
    builder.endArray();
    return builder.build();
}
