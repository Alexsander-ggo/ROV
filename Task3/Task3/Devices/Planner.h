#ifndef PLANNER_H
#define PLANNER_H

#include "Missions/Mission.h"
#include "Utilities/Command.h"

class Planner : public Command
{
public:
    using Command::Command;
    Planner();

    ~Planner();

    bool isEdit() const;

    Mission* getCurrentMission() const;

    Missions& getMissions();

    const Missions& getMissions() const;

    bool erase(uint64_t index);

    bool erase(uint64_t from, uint64_t to);

    bool clear();

    bool insert(uint64_t index, MissionType type);

    bool add(MissionType type);

    bool swap(uint64_t first, uint64_t second);

    bool edit(uint64_t index);

    bool exit();

private:
    bool isCorrectIndex(uint64_t index) const;

    template<typename It>
    It find(It begin, uint64_t index);

private:
    Missions mMissions;
    Mission* mCurrentMission;
};

template<typename It>
It Planner::find(It begin, uint64_t index)
{
    uint64_t size = mMissions.size();
    auto it = mMissions.end();
    index--;
    if (index < size) {
        return begin + index;
    }
    return it;
}

#endif // PLANNER_H
