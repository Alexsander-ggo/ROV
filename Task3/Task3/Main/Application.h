#ifndef APPLICATION_H
#define APPLICATION_H

#include "Devices/Consol.h"

#include <iostream>

class Application
{
public:
    Application(int argc, char* argv[],
                std::istream& in = std::cin,
                std::ostream& out = std::cout);

    ~Application();

    int run();

private:
    std::istream& mIn;
    std::ostream& mOut;
    int mError;
    Consol* mConsol;
};

#endif // APPLICATION_H
