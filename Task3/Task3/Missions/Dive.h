#ifndef DIVE_H
#define DIVE_H

#include "Missions/Mission.h"

class Dive : public Mission
{
public:
    using Mission::Mission;
    Dive(MissionType type = DIVE,
         const std::string& name = "погружение");

    double getDepth() const;
};

#endif // DIVE_H
