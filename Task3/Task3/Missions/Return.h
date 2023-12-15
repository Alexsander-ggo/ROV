#ifndef RETURN_H
#define RETURN_H

#include "Missions/Mission.h"

class Return : public Mission
{
public:
    using Mission::Mission;
    Return(MissionType type = RETURN,
           const std::string& name = "возвращение");
};

#endif // RETURN_H
