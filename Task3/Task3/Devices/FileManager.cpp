#include "Devices/FileManager.h"

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
    uint64_t pos = filename.find(".data");
    if (pos == -1 || filename.size() != pos + 5) {
        return false;
    }
    mFileName = filename;
    std::fstream file;
    file.open(mFileName, std::ifstream::in);
    if (file.is_open()) {
        mIsOpened = true;
        while (!file.eof()) {
            char begin = file.get();
            if (begin == '@') {
                uint8_t count = file.get();
                std::vector<uint8_t> bytes;
                for (uint8_t i = 0; i < count; ++i) {
                    bytes.push_back(file.get());
                }
                char end = file.get();
                if (end == '\n') {
                    uint8_t crc = 0;
                    for (uint8_t i = 0; i < bytes.size() - 1; ++i) {
                        crc += bytes[i];
                    }
                    if (crc == bytes.back()) {
                        mMissions.push_back(createMission((MissionType)bytes.front()));
                        mMissions.back()->load({bytes.begin() + 1, bytes.end() - 1});
                    }
                }
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
        flag = saveData() && writeData();
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
    uint8_t expansion = std::string(".data").size();
    std::string filename = mFileName.substr(0, mFileName.size() - expansion) + ".txt";
    file.open(filename, std::ofstream::out);
    if (file.is_open()) {
        for (uint64_t i = 0; i < mMissions.size(); ++i) {
            Mission* mission = mMissions.at(i);
            file << i + 1 << ") ";
            mission->print(file, "; ");
            file << '\n';
        }
        file.close();
        return true;
    }
    return false;
}

bool FileManager::writeData()
{
    std::fstream file;
    file.open(mFileName, std::ofstream::out);
    if (file.is_open()) {
        for (const Mission* mission : mMissions) {
            std::vector<uint8_t> bytes = mission->upload();
            uint8_t count = bytes.size() + 2;
            uint8_t type = mission->getType();
            uint8_t crc = type;
            for (uint8_t byte : bytes) {
                crc += byte;
            }
            file << '@';
            file << count;
            file << type;
            for (uint8_t byte : bytes) {
                file << byte;
            }
            file << crc;
            file << '\n';
        }
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
