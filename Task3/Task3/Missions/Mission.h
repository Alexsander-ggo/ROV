#ifndef MISSION_H
#define MISSION_H

#include "Utilities/Command.h"

#include <deque>
#include <iostream>
#include <map>
#include <string>
#include <vector>

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

class Mission : public Command
{
public:
    using Command::Command;
    Mission(MissionType type = EMPTY,
            const std::string& name = "");

    std::vector<uint8_t> upload() const;

    void load(const std::vector<uint8_t>& bytes);

    MissionType getType() const;

    void print(std::ostream& out,
               const std::string& separator = "\n") const;

protected:
    MissionType mType;
    std::string mName;
};

#endif // MISSION_H
