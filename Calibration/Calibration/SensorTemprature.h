#ifndef SENSORTEMPRATURE_H
#define SENSORTEMPRATURE_H

#include <Eigen/Dense>

#include <string>
#include <vector>

namespace clb {

    struct Data
    {
        float rawValue;
        float value;
        float temp;
    };

    class SensorTemperature
    {
    public:
        SensorTemperature(uint8_t bitness = 10);

        void calibrateEstimator(const std::string& fileName,
                                float deltaMax = 0.01);

        void calibrateRANSAC(const std::string& fileName,
                             float deltaMax = 0.01);

        float getTemp(float value);

    private:
        void loadData(const std::string& fileName);

    private:
        float mMaxValue;
        float mGain;
        float mOffset;
        std::vector<Data> mData;
        Eigen::MatrixXf mSystem;
        Eigen::MatrixXf mRight;
        Eigen::MatrixXf mResidual;
        Eigen::MatrixXf mWeight;
    };

}

#endif // SENSORTEMPRATURE_H
