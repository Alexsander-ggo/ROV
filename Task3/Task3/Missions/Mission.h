#ifndef MISSION_H
#define MISSION_H

#include "JSON/JSON.h"
#include "Utilities/Command.h"

#include <deque>
#include <iostream>
#include <map>
#include <string>

enum MissionType
{
    EMPTY,
    DIVE,
    LIFT,
    MOVE,
    TACK,
    RETURN
};

class Mission;

using Missions = std::deque<Mission*>;

Mission* createMission(MissionType type);

Mission* createMission(const json::Node& node);

class Mission : public Command
{
public:
    using Command::Command;
    Mission(MissionType type = EMPTY,
            const std::string& name = "");

    MissionType getType() const;

    void print(std::ostream& out,
               const std::string& separator = "\n") const;

    void load(const json::Node& node);

    json::Node save() const;

protected:
    MissionType mType;
    std::string mName;
};

#endif // MISSION_H
