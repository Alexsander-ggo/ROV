#ifndef LIFT_H
#define LIFT_H

#include "Missions/Dive.h"
#include "Missions/Mission.h"

class Lift : public Dive
{
public:
    using Dive::Dive;
    Lift(MissionType type = LIFT,
         const std::string& name = "всплытие");
};

#endif // LIFT_H
