#include "Devices/Planner.h"

Planner::Planner()
{
    Parameter index("index", "", 0, 10000, 0, 0, 0);
    Parameter type("type", "", 0, 5, 1, 0, 1);
    Parameter begin("begin", "", 0, 10000, 0, 0, 0);
    Parameter end("end", "", 0, 10000, 0, 0, 0);
    Parameter first("first", "", 0, 10000, 0, 0, 0);
    Parameter second("second", "", 0, 10000, 0, 0, 0);
    std::map<uint64_t, std::string> types = {
        {1, "погружение"}, {2, "всплытие"}, {3, "движение"},
        {4, "галс"}, {5, "возвращение"}
    };

    addSetting(Setting("Выйти", "exit", "Выход из режима редактирования миссии", {},
    [this] (const Parameters&)->bool {
        return exit();
    }));

    addSetting(Setting("Удалить", "erase", "Удаления миссии с индексом index", {index},
    [this] (const Parameters& parameters)->bool {
        return erase(parameters.front().getValue());
    }));

    addSetting(Setting("Удалить", "erase", "Удаление миссий с индексами от from до to включительно", {begin, end},
    [this] (const Parameters& parameters)->bool {
        return erase(parameters.front().getValue(), parameters.back().getValue());
    }));

    addSetting(Setting("Очистить", "clear", "Удаления всех миссий", {},
    [this] (const Parameters&)->bool {
        return clear();
    }));

    addSetting(Setting("Вставить", "insert", "Вставка миссии типа type на позицию с индексом index", {index, type},
    [this] (const Parameters& parameters)->bool {
        return insert(parameters.front().getValue(), (MissionType)parameters.back().getValue());
    }), types);

    addSetting(Setting("Добавить", "add", "Добавление в конец миссии типа type", {type},
    [this] (const Parameters& parameters)->bool {
        return add((MissionType)parameters.front().getValue());
    }), types);

    addSetting(Setting("Поменять", "swap", "Замена местами миссий с индесами first и second", {first, second},
    [this] (const Parameters& parameters)->bool {
        return swap(parameters.front().getValue(), parameters.back().getValue());
    }));

    addSetting(Setting("Редактировать", "edit", "Редактирование миссии с индексом index", {index},
    [this] (const Parameters& parameters)->bool {
        return edit(parameters.front().getValue());
    }));
}

Planner::~Planner()
{
    for (Mission* mission : mMissions) {
        delete mission;
    }
}

bool Planner::isEdit() const
{
    return mCurrentMission;
}

Mission* Planner::getCurrentMission() const
{
    return mCurrentMission;
}

Missions& Planner::getMissions()
{
    return mMissions;
}

const Missions& Planner::getMissions() const
{
    return mMissions;
}

bool Planner::erase(uint64_t index)
{
    if (!isCorrectIndex(index)) {
        return false;
    }
    auto it = find(mMissions.begin(), index);
    mMissions.erase(it);
    return true;
}

bool Planner::erase(uint64_t from, uint64_t to)
{
    if (!isCorrectIndex(from) || !isCorrectIndex(to)) {
        return false;
    }
    auto begin = mMissions.begin();
    auto fromIt = find(begin, from);
    auto toIt = find(begin, to + 1);
    mMissions.erase(fromIt, toIt);
    return true;
}

bool Planner::clear()
{
    mMissions.clear();
    return true;
}

bool Planner::insert(uint64_t index, MissionType type)
{
    if (!isCorrectIndex(index)) {
        return false;
    }
    mMissions.insert(find(mMissions.begin(), index), createMission(type));
    return true;
}

bool Planner::add(MissionType type)
{
    mMissions.push_back(createMission(type));
    return true;
}

bool Planner::swap(uint64_t first, uint64_t second)
{
    if (!isCorrectIndex(first) || !isCorrectIndex(second)) {
        return false;
    }
    auto begin = mMissions.begin();
    auto firstIt = find(begin, first);
    auto secondIt = find(begin, second);
    std::swap(*firstIt, *secondIt);
    return true;
}

bool Planner::edit(uint64_t index)
{
    if (!isCorrectIndex(index)) {
        return false;
    }
    auto it = find(mMissions.begin(), index);
    if (it != mMissions.end()) {
        mCurrentMission = *it;
        return true;
    }
    return false;
}

bool Planner::exit()
{
    mCurrentMission = nullptr;
    return true;
}

bool Planner::isCorrectIndex(uint64_t index) const
{
    return index > 0 && index <= mMissions.size();
}
