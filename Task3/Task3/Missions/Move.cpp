#include "Missions/Move.h"

#include "Utilities/Setting.h"

Move::Move(MissionType type,
           const std::string& name)
    : Mission(type, name)
{
    Parameter x("x", "м", 2, 10000, 0, 0, 0);
    Parameter y("y", "м", 2, 10000, 0, 0, 0);
    Parameter flag("flag", "", 0, 1, 0, 0, 1);
    Parameter radius("radius", "м", 2, 10, 1, 1, 0);

    addSetting(Setting("Координаты", "point", "Задание точки назначения", {x, y}));

    addSetting(Setting("Режим", "mode", "Задание режима движения", {flag}),
               {{0, "в точку"}, {1, "вдоль прямой"}});

    addSetting(Setting("Тип", "type", "Задание типа движения", {flag}),
               {{0, "постоянная глубина"}, {1, "постоянное отстояние"}});

    addSetting(Setting("Точность", "precision", "Задание радиуса окрестности попадания в точку назначения", {radius}));
}

double Move::getX() const
{
    return mSettings.front().getValue(0);
}

double Move::getY() const
{
    return mSettings.front().getValue(1);
}
