#include "Sensor.h"

#include <iostream>
#include <string>

using namespace std;

void SettingSensor(Sensor& sensor) {
    sensor.add(new Field(5, "kHeading", FLOAT, sizeof(float), "град."))
          .add(new Field(24, "kPitch", FLOAT, sizeof(float), "град."))
          .add(new Field(25, "kRoll", FLOAT, sizeof(float), "град."))
          .add(new Field(79, "kHeadingStatus", CHAR, sizeof(char)))
          .add(new Field(77, "kQuaternion", FLOAT, sizeof(float), "", 4))
          .add(new Field(7, "kTemperature", FLOAT, sizeof(float), "град. C"))
          .add(new Field(8, "kDistortion", BOOL, sizeof(bool)))
          .add(new Field(9, "kCalStatus", BOOL, sizeof(bool)))
          .add(new Field(21, "kAccelX", FLOAT, sizeof(float), "g"))
          .add(new Field(22, "kAccelY", FLOAT, sizeof(float), "g"))
          .add(new Field(23, "kAccelZ", FLOAT, sizeof(float), "g"))
          .add(new Field(27, "kMagX", FLOAT, sizeof(float), "mT"))
          .add(new Field(28, "kMagY", FLOAT, sizeof(float), "mT"))
          .add(new Field(29, "kMagZ", FLOAT, sizeof(float), "mT"))
          .add(new Field(74, "kGyroX", FLOAT, sizeof(float), "рад/сек"))
          .add(new Field(75, "kGyroY", FLOAT, sizeof(float), "рад/сек"))
          .add(new Field(76, "kGyroZ", FLOAT, sizeof(float), "рад/сек"));
}

int main()
{
    setlocale(LC_ALL, "ru");
    Sensor sensor;
    SettingSensor(sensor);
    while (true) {
        std::string test;
        getline(std::cin, test, '\n');
        if (test == "ok") {
            break;
        }
        for (char c : test) {
            sensor.setData(std::string(1, c));
        }
        std::cout << sensor;
    }
    return 0;
}
