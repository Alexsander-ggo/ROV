#include "Main/Application.h"

Application::Application(int argc, char* argv[],
                         std::istream& in, std::ostream& out)
    : mIn(in)
    , mOut(out)
{
    mError = 0;
     if (argc != 2) {
         mOut << "Выберете файл с расширением .json\n";
         mError = 1;
     } else {
         mConsol = new Consol(125, 50, mIn, mOut);
         if (!mConsol->open(argv[1])) {
             mOut << "Файл с именем " << argv[1] << " не удалось открыть\n";
             mOut << "Выберете файл с расширением .json\n";
             mError = 2;
         }
     }
}

Application::~Application()
{
    delete mConsol;
}

int Application::run()
{
    if (mError != 0) {
        return mError;
    }
    mConsol->run();
    return mError;
}
