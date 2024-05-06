#include "SensorTemprature.h"

#include <iomanip>
#include <iostream>

int main()
{
    clb::SensorTemperature sensorEstimator, sensorRANSAC;
    sensorEstimator.calibrateEstimator("Data.txt");
    sensorRANSAC.calibrateRANSAC("Data.txt");
    for (int z = 1; z < 1024; ++z) {
        std::cout << "Z = " << z << "   " << std::fixed << std::setprecision(1)
                  << "\tEstimator: T = " << sensorEstimator.getTemp(z)
                  << "\tRANSAC: T = " << sensorRANSAC.getTemp(z) << std::endl;
    }
    return 0;
}
