#ifndef TACK_H
#define TACK_H

#include "Missions/Mission.h"

class Tack : public Mission
{
public:
    using Mission::Mission;
    Tack(MissionType type = TACK,
         const std::string& name = "галс");

    double getBeginX() const;

    double getBeginY() const;

    double getEndX() const;

    double getEndY() const;
};

#endif // TACK_H
