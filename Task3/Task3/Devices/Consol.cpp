#include "Devices/Consol.h"

#include <vector>

Consol::Consol(int width, int height,
               std::istream& in,
               std::ostream& out)
    : mIn(in)
    , mOut(out)
    , mIsSuccessful(true)
    , mIsHasHelp(true)
{
    mPlanner = new Planner();
    mFileManager = new FileManager(mPlanner->getMissions());
    mLine = std::string(width, '=') + '\n';

    setlocale(LC_ALL, "Russian");
    mOut << "\e[8;" << height << ";" << width << ";t";

    Parameter mode("mode", "", 0, 1, 0, 1, 1);
    addSetting(Setting("Помощь", "help", "Вкл/выкл справочника команд", {mode},
    [this] (const Parameters& parameters)->bool {
        return help(parameters.front().getValue());
    }), {{0, "выкл"}, {1, "вкл"}});

    addSetting(Setting("Сохранить", "save", "Сохранение миссий в текущий файл", {},
    [this] (const Parameters&)->bool {
        return save();
    }));

    addSetting(Setting("Очистить", "close", "Закрытие текущего файла", {},
    [this] (const Parameters&)->bool {
        return close();
    }));
}

Consol::~Consol()
{
    delete mFileManager;
    delete mPlanner;
}

bool Consol::save()
{
    return mFileManager->save();
}

bool Consol::open(const std::string& filename)
{
    return mFileManager->open(filename);
}

void Consol::run()
{
    while (mFileManager->isOpen()) {
        update();
    }
}

bool Consol::close()
{
    std::string command;
    do {
        mOut << "Вы точно хотите закрыть файл без сохранения изменений?(y/n)\n";
        std::getline(mIn, command);
        Parser parser;
        Args args = parser.split(command);
        if (args.size() == 1) {
            if (args.front() == "y" || args.front() == "n") {
                command = args.front();
            }
        }
    } while (command != "y" && command != "n");
    if (command == "y") {
        mFileManager->close();
    }
    return !mFileManager->isOpen();
}

bool Consol::help(bool flag)
{
    mIsHasHelp = flag;
    return true;
}

void Consol::update()
{
    system("clear");
    view();
    command();
}

void Consol::view()
{
    viewHelp();
    viewData();
    viewResult();
}

void Consol::viewHelp()
{
    Mission* mCurrentMission = mPlanner->getCurrentMission();
    if (mIsHasHelp) {
        mOut << mLine;
        if (mPlanner->isEdit()) {
            info(mOut);
            mPlanner->info(mOut, 0, 0);
            mOut << mLine;
            mCurrentMission->info(mOut);
        } else {
            info(mOut);
            mOut << mLine;
            mPlanner->info(mOut, 1);
        }
    }
    mOut << mLine;
}

void Consol::viewData()
{
    Mission* mCurrentMission = mPlanner->getCurrentMission();
    if (mPlanner->isEdit()) {
        mCurrentMission->print(mOut);
    } else {
        print();
    }
    mOut << mLine;
}

void Consol::viewResult()
{
    mOut << mFileManager->getSaveInfo() << '\n';
    mOut << mLine;
    if (!mCommand.empty()) {
        if (mIsSuccessful) {
            mOut << "Успех: ";
        } else {
            mOut << "Провал: ";
        }
        mOut << mCommand << '\n';
    }
    mOut << mLine;
}

void Consol::command()
{
    mIsSuccessful = false;
    Mission* mCurrentMission = mPlanner->getCurrentMission();
    getline(mIn, mCommand);
    if (mPlanner->isEdit()) {
        mIsSuccessful |= mCurrentMission->setCommand(mCommand);
        mIsSuccessful |= mPlanner->setCommand(mCommand, 0, 0);
    } else {
        mIsSuccessful |= mPlanner->setCommand(mCommand, 1);
    }
    mIsSuccessful |= setCommand(mCommand);
}

void Consol::print() const
{
    uint64_t i = 0;
    for (Mission* mission : mPlanner->getMissions()) {
        mOut << ++i << ") ";
        mission->print(mOut, "; ");
    }
}
