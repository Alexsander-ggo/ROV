#ifndef CONSOL_H
#define CONSOL_H

#include "Devices/FileManager.h"
#include "Devices/Planner.h"
#include "Missions/Mission.h"
#include "Utilities/Command.h"

#include <iostream>
#include <string>

class Consol : public Command
{
public:
    using Command::Command;
    Consol(int width = 80, int height = 25,
           std::istream& in = std::cin,
           std::ostream& out = std::cout);

   ~Consol();

    void run();

public:
    bool save();

    bool close();

    bool open(const std::string& filename);

    bool help(bool flag);

private:
    void update();

    void view();

    void viewHelp();

    void viewData();

    void viewResult();

    void command();

    void print() const;

private:
    std::istream& mIn;
    std::ostream& mOut;
    bool mIsSuccessful;
    bool mIsHasHelp;
    std::string mCommand;
    std::string mLine;
    FileManager* mFileManager;
    Planner* mPlanner;
};

#endif // CONSOL_H
