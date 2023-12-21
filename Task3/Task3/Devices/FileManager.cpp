#include "Devices/FileManager.h"

#include "JSON/JSON.h"
#include "JSON/JSON_builder.h"
#include "Missions/Move.h"
#include "Missions/Return.h"

#include <fstream>

std::string ToString(double value) {
    std::string text = std::to_string(value);
    return text.substr(0, text.size() - 4);
}

FileManager::FileManager(Missions& missions)
    : mMissions(missions)
{

}

bool FileManager::isOpen() const
{
    return mIsOpened;
}

std::string FileManager::getSaveInfo() const
{
    return mSaveInfo;
}

bool FileManager::open(const std::string& filename)
{
    uint64_t pos = filename.find(".json");
    if (pos == -1 || filename.size() != pos + 5) {
        return false;
    }
    mFileName = filename;
    std::fstream file;
    file.open(mFileName, std::ifstream::in);
    if (file.is_open()) {
        mIsOpened = true;
        mMissions.clear();
        json::Document doc = json::Load(file);
        json::Node node = doc.getRoot();
        if (!node.isNull()) {
            json::Array array = node.asArray();
            for (const json::Node& unit : array) {
                mMissions.push_back(createMission(unit));
            }
        }
        file.close();
        return true;
    }
    return false;
}

bool FileManager::close()
{
    mIsOpened = false;
    return true;
}

bool FileManager::save()
{
    bool flag = checkCorrectionMission();
    if (flag) {
        flag = saveData();
    }
    if (flag) {
        mSaveInfo = "Миссия сохранена в файл " + mFileName;
    } else {
        mSaveInfo.insert(0, "Не удалось сохранить миссию в файл " + mFileName + ": ");
    }
    return flag;
}

bool FileManager::saveData()
{
    std::fstream file;
    file.open(mFileName, std::ofstream::out);
    if (file.is_open()) {
        json::Builder builder;
        builder.startArray();
        for (const Mission* mission : mMissions) {
            builder.value(mission->save().getValue());
        }
        builder.endArray();
        json::Document doc(builder.build());
        json::Print(doc, file);
        file.close();
        return true;
    }
    return false;
}

bool FileManager::checkCorrectionMission()
{
    mSaveInfo.clear();
    if (mMissions.empty()) {
        return true;
    }
    if (mMissions.back()->getType() != RETURN) {
        mSaveInfo = "Миссия должна заканчиваться возвращением";
        return false;
    }
    double depth = 0;
    double x = 0;
    double y = 0;
    for (uint64_t i = 0; i < mMissions.size(); ++i) {
        Mission* mission = mMissions.at(i);
        bool flag = true;
        switch(mission->getType()) {
            case DIVE:
            {
                flag = checkDive((Dive*)mission, depth);
                break;
            }
            case LIFT:
            {
                flag = checkLift((Lift*)mission, depth);
                break;
            }
            case MOVE:
            {
                Move* move = (Move*)mission;
                x = move->getX();
                y = move->getY();
                break;
            }
            case TACK:
            {
                flag = checkTack((Tack*)mission, x, y);
                break;
            }
            case RETURN:
            {
                depth = 0;
                break;
            }
            default:
            break;
        }
        if (!flag) {
            mSaveInfo.insert(0, "Миссия №" + std::to_string(i + 1) + ":");
            return false;
        }
    }
    return true;
}

bool FileManager::checkDive(Dive* dive, double& curDepth)
{
    double newDepth = dive->getDepth();
    if (newDepth < curDepth) {
        mSaveInfo = " нельзя с глубины " + ToString(curDepth) + " м"
                  + " погрузиться на глубину " + ToString(newDepth) + " м";
        return false;
    }
    curDepth = newDepth;
    return true;
}

bool FileManager::checkLift(Lift* lift, double& curDepth)
{
    double newDepth = lift->getDepth();
    if (newDepth > curDepth) {
        mSaveInfo = " нельзя с глубины " + ToString(curDepth) + " м"
                  + " всплыть на глубину " + ToString(newDepth) + " м";
        return false;
    }
    curDepth = newDepth;
    return true;
}

bool FileManager::checkTack(Tack* tack, double& curX, double& curY)
{
    double newX = tack->getBeginX();
    double newY = tack->getBeginY();
    if (curX != newX || curY != newY) {
        mSaveInfo = " нельзя начать движение галсом из точки с координатами ("
                  + ToString(newX) + ", " + ToString(newY) + ") м,"
                  + " находясь в точке с координатами ("
                  + ToString(curX) + ", " + ToString(curY) + ") м";
        return false;
    }
    curX = tack->getEndX();
    curY = tack->getEndY();
    return true;
}
