#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include "Missions/Dive.h"
#include "Missions/Lift.h"
#include "Missions/Mission.h"
#include "Missions/Tack.h"

#include <string>

class FileManager
{
public:
    FileManager(Missions& missions);

    bool isOpen() const;

    std::string getSaveInfo() const;

    bool open(const std::string& filename);

    bool close();

    bool load();

    bool save();

private:
    bool saveData();

//    bool writeData();

    bool checkCorrectionMission();

    bool checkDive(Dive* dive, double& curDepth);

    bool checkLift(Lift* lift, double& curDepth);

    bool checkTack(Tack* tack, double& curX, double& curY);

private:
    bool mIsOpened;
    std::string mFileName;
    std::string mSaveInfo;
    Missions& mMissions;
};

#endif // FILEMANAGER_H
