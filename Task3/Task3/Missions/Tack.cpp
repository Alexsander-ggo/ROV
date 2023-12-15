#include "Missions/Tack.h"

#include "Utilities/Setting.h"

Tack::Tack(MissionType type,
           const std::string& name)
    : Mission(type, name)
{
    Parameter x("x", "м", 2, 10000, 0, 0, 0);
    Parameter y("y", "м", 2, 10000, 0, 0, 0);
    Parameter distance("distance", "м", 2, 10000, 1, 1, 0);

    addSetting(Setting("Начало", "begin", "Задание начальной точки", {x, y}));

    addSetting(Setting("Конец", "end", "Задание конечной точки", {x, y}));

    addSetting(Setting("Интервал", "space", "Задание расстояния между галсами", {distance}));
}

double Tack::getBeginX() const
{
    return mSettings.at(0).getValue(0);
}

double Tack::getBeginY() const
{
    return mSettings.at(0).getValue(1);
}

double Tack::getEndX() const
{
    return mSettings.at(1).getValue(0);
}

double Tack::getEndY() const
{
    return mSettings.at(1).getValue(1);
}
