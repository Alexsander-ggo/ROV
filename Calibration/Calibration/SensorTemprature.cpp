#include "SensorTemprature.h"

#include <cmath>
#include <fstream>

namespace clb {

    clb::SensorTemperature::SensorTemperature(uint8_t bitness)
    {
        mMaxValue = (1 << bitness);
    }

    void SensorTemperature::calibrateEstimator(const std::string& fileName, float deltaMax)
    {
        loadData(fileName);
        static const float ErrorMax = 0.0001;
        static const int MaxIter = 100;
        int i = 0;
        int n = mData.size();
        Eigen::MatrixXf solve(2, 1);
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                mWeight(i, j) = (i == j);
            }
        }
        while (i < MaxIter) {
            solve = (mSystem.transpose() * mWeight * mSystem).lu().solve(mSystem.transpose() * mWeight * mRight);
            mResidual = mRight - mSystem * solve;
            bool flag = true;
            for (int j = 0; j < n; ++j) {
                float weight = 1 / std::max(deltaMax, std::abs(mResidual(j, 0)));
                if (std::abs(weight - mWeight(j, j)) > ErrorMax) {
                    mWeight(j, j) = weight;
                    flag = false;
                }
            }
            if (flag) {
                break;
            }
            ++i;
        }
        mGain = solve(0, 0);
        mOffset = solve(1, 0);
    }

    void SensorTemperature::calibrateRANSAC(const std::string& fileName, float deltaMax)
    {
        loadData(fileName);
        Eigen::MatrixXf tempSystem(2, 2);
        tempSystem(0, 1) = 1;
        tempSystem(1, 1) = 1;
        Eigen::MatrixXf tempRight(2, 1);
        Eigen::MatrixXf hypothesisSolve(2, 1);
        Eigen::MatrixXf solve(2, 1);
        float deltaMin = -1;
        float norm = mRight.norm();
        int n = mData.size();
        for (int i = 0; i < n; ++i) {
            tempSystem(0, 0) = mData.at(i).value;
            tempRight(0, 0) = mData.at(i).temp;
            for (int j = i + 1; j < n; ++j) {
                tempSystem(1, 0) = mData.at(j).value;
                tempRight(1, 0) = mData.at(j).temp;
                hypothesisSolve = tempSystem.lu().solve(tempRight);
                mResidual = mRight - mSystem * hypothesisSolve;
                float delta = mResidual.norm() / norm;
                if (delta < deltaMin || deltaMin < 0) {
                    deltaMin = delta;
                    solve = hypothesisSolve;
                }
            }
        }
        int size = 0;
        mResidual = mRight - mSystem * solve;
        for (int i = 0; i < n; ++i) {
            size += (std::abs(mResidual(i, 0)) < deltaMax * norm);
        }
        tempSystem.resize(size, 2);
        tempRight.resize(size, 1);
        std::vector<Data> filterData;
        int index = 0;
        for (int i = 0; i < n; ++i) {
            if (std::abs(mResidual(i, 0)) < deltaMax * norm) {
                tempSystem(index, 0) = mData.at(i).value;
                tempSystem(index, 1) = 1;
                tempRight(index, 0) = mData.at(i).temp;
                filterData.push_back(mData.at(i));
                ++index;
            }
        }
        solve = (tempSystem.transpose() * tempSystem).lu().solve(tempSystem.transpose() * tempRight);
        mGain = solve(0, 0);
        mOffset = solve(1, 0);
    }

    void SensorTemperature::loadData(const std::string& fileName)
    {
        std::fstream file;
        file.open(fileName, std::ios::in);
        if (file.is_open()) {
            while (!file.eof()) {
                float rawValue, temp;
                file >> rawValue >> temp;
                mData.push_back(Data{rawValue, std::log10(rawValue / mMaxValue), temp});
            }
        }
        file.close();
        mData.pop_back();
        int n = mData.size();
        mSystem.resize(n, 2);
        mRight.resize(n, 1);
        mResidual.resize(n, 1);
        mWeight.resize(n, n);
        for (int i = 0; i < n; ++i) {
            mSystem(i, 0) = mData.at(i).value;
            mSystem(i, 1) = 1;
            mRight(i, 0) = mData.at(i).temp;
        }
    }

    float SensorTemperature::getTemp(float value)
    {
        return mGain * std::log10(value / mMaxValue) + mOffset;
    }

}
