#ifndef MOVE_H
#define MOVE_H

#include "Missions/Mission.h"

class Move : public Mission
{
public:
    using Mission::Mission;
    Move(MissionType type = MOVE,
         const std::string& name = "движение");

    double getX() const;

    double getY() const;
};

#endif // MOVE_H
