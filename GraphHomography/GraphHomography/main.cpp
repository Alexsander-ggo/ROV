#include "GraphHomography.h"

#include <QApplication>

int main(int argc, char* argv[])
{
    QApplication application(argc, argv);
    GraphHomography window;
    window.show();
    return application.exec();
}
