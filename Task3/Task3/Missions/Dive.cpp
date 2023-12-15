#include "Missions/Dive.h"

#include "Utilities/Setting.h"

Dive::Dive(MissionType type,
           const std::string& name)
    : Mission(type, name)
{
    Parameter mode("type", "", 0, 1, 0, 0, 1);
    Parameter measure("measure", "м", 2, 100, 0, 0, 0);
    Parameter flag("flag", "", 0, 1, 0, 0, 1);

    addSetting(Setting("Измеритель", "sensor", "Задание типа устройства измерения", {mode}),
               {{0, "датчик глубины"}, {1, "эхолот"}});

    addSetting(Setting("Величина", "value", "Задание величины устройства измерения", {measure}));

    addSetting(Setting("Режим", "mode", "Задание режима движения подводного аппарата", {flag}),
               {{0, "вертикально"}, {1, "спиралью"}});
}

double Dive::getDepth() const
{
    const Parameter& parameterSensor = mSettings.at(0).getParameter();
    const Parameter& parameterValue = mSettings.at(1).getParameter();
    if (parameterSensor.getData() == 0) {
        return parameterValue.getValue();
    }
    return parameterValue.getMax() - parameterValue.getValue();
}
