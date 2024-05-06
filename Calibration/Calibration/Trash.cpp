/*#include <Eigen/Dense>

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <string>
#include <vector>

struct SensorData
{
    float rawValue;
    float value;
    float temp;
};

int main()
{
    std::vector<SensorData> data;
    std::string fileName = "Data.txt";
    std::fstream file;
    file.open(fileName, std::ios::in);
    if (file.is_open()) {
        while (!file.eof()) {
            float rawValue, temp;
            file >> rawValue >> temp;
            data.push_back(SensorData{rawValue, std::log10(rawValue / 1024), temp});
        }
    }
    data.pop_back();
    int n = data.size();
    Eigen::MatrixXf A(n, 2);
    Eigen::MatrixXf B(n, 1);
    Eigen::MatrixXf X(2, 1);
    Eigen::MatrixXf R(n, 1);
    Eigen::MatrixXf W(n, n);

    for (int i = 0; i < n; ++i) {
        A(i, 0) = data.at(i).value;
        A(i, 1) = 1;
        B(i, 0) = data.at(i).temp;
        W(i, i) = 1;
    }

    X = (A.transpose() * A).lu().solve(A.transpose() * B);
    float Kmnk = X(0, 0);
    float Bmnk = X(1, 0);

    float deltaMax = 0.01;
    float errorMax = 0.0001;
    int i = 0;
    while (i < 100) {
        X = (A.transpose() * W * A).lu().solve(A.transpose() * W * B);
        R = B - A * X;
        bool flag = true;
        for (int j = 0; j < n; ++j) {
            float weight = 1 / std::max(deltaMax, std::abs(R(j, 0)));
            if (abs(weight - W(j, j)) > errorMax) {
                W(j, j) = weight;
                flag = false;
            }
        }
        if (flag) {
            break;
        }
        ++i;
    }
    float Kmnm = X(0, 0);
    float Bmnm = X(1, 0);

    Eigen::MatrixXf Ah(2, 2);
    Ah(0, 1) = 1;
    Ah(1, 1) = 1;
    Eigen::MatrixXf Bh(2, 1);
    Eigen::MatrixXf H(2, 1);

    float deltaMin = -1;
    float norm = B.norm();
    for (int i = 0; i < n; ++i) {
        Ah(0, 0) = data.at(i).value;
        Bh(0, 0) = data.at(i).temp;
        for (int j = i + 1; j < n; ++j) {
            Ah(1, 0) = data.at(j).value;
            Bh(1, 0) = data.at(j).temp;
            H = Ah.lu().solve(Bh);
            R = B - A * H;
            float delta = R.norm() / norm;
            if (delta < deltaMin || deltaMin < 0) {
                deltaMin = delta;
                X = H;
            }
        }
    }
    int size = 0;
    R = B - A * X;
    for (int i = 0; i < n; ++i) {
        size += (std::abs(R(i, 0)) < deltaMax * norm);
    }
    A.resize(size, 2);
    B.resize(size, 1);
    std::vector<SensorData> filterData;
    int index = 0;
    for (int i = 0; i < n; ++i) {
        if (std::abs(R(i, 0)) < deltaMax * norm) {
            A(index, 0) = data.at(i).value;
            A(index, 1) = 1;
            B(index, 0) = data.at(i).temp;
            filterData.push_back(data.at(i));
            ++index;
        }
    }
    X = (A.transpose() * A).lu().solve(A.transpose() * B);
    float Kran = X(0, 0);
    float Bran = X(1, 0);

    std::cout << "Kмнм = " << Kmnk << " Bмнк = " << Bmnk << std::endl;
    std::cout << "Kмнм = " << Kmnm << " Bмнм = " << Bmnm;
    std::cout << "Kran = " << Kran << " Bran = " << Bran;

    float sumErrorMNK = 0;
    float sumErrorMNM = 0;
    float sumErrorRAN = 0;

    for (const SensorData& info : filterData) {
        float tempMNK = Kmnk * info.value + Bmnk;
        float tempMNM = Kmnm * info.value + Bmnm;
        float tempRAN = Kran * info.value + Bran;
        float errorMNK = info.temp - tempMNK;
        float errorMNM = info.temp - tempMNM;
        float errorRAN = info.temp - tempRAN;
        sumErrorMNK += std::abs(errorMNK);
        sumErrorMNM += std::abs(errorMNM);
        sumErrorRAN += std::abs(errorRAN);
        std::cout << "Z = " << info.rawValue << std::fixed << std::setprecision(2)
                  << "\t\tL = " << info.value << "\tT = " << info.temp
                  << "\tTмнк = " << tempMNK << "\tTмнм = " << tempMNM << "\tTran = " << tempRAN
                  << "\tEмнк = " << errorMNK << "\tEмнм = " << errorMNM << "\tEran = " << errorRAN
                  << std::setprecision(0) << std::endl;
    }
    std::cout << std::setprecision(2);
    std::cout << "MNK = " << sumErrorMNK << std::endl;
    std::cout << "MNM = " << sumErrorMNM << std::endl;
    std::cout << "RAN = " << sumErrorRAN << std::endl;
    for (int z = 1; z < 1024; ++z) {
        float v = std::log10(z / 1024.f);
        std::cout << std::setprecision(0) << "Z = " << z << std::setprecision(1)
                   << "\tTмнк = " << Kmnk * v + Bmnk
                   << "\tTмнм = " << Kmnm * v + Bmnm
                   << "\tTran = " << Kran * v + Bran << std::endl;

    }
    return 0;
}*/

