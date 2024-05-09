#include "SolverDP.h"

namespace ode {

    void SolverDP::initButcherTable()
    {
        mSteps = {0, 0.2, 0.3, 0.8, 8 / 9.0, 1, 1};
        mWeights.push_back({0, 0, 0, 0, 0, 0, 0});
        mWeights.push_back({0.2, 0, 0, 0, 0, 0, 0});
        mWeights.push_back({3 / 40.0, 9 / 40.0, 0, 0, 0, 0, 0});
        mWeights.push_back({44 / 45.0, -56 / 15.0, 32 / 9.0, 0, 0, 0, 0});
        mWeights.push_back({19372 / 6561.0, -25360 / 2187.0, 64448 / 6561.0, -212 / 729.0, 0, 0, 0});
        mWeights.push_back({9017 / 3168.0, -355 / 33.0, 46732 / 5247.0, 49 / 176.0, -5103 / 18656.0, 0, 0});
        mWeights.push_back({35 / 384.0, 0, 500 / 1113.0, 125 / 192.0, -2187 / 6784.0, 11 / 84.0, 0});
        mOutWeights.push_back({35 / 384.0, 0, 500 / 1113.0, 125 / 192.0, -2187 / 6784.0, 11 / 84.0, 0});
        mOutWeights.push_back({5179 / 57600.0, 0, 7571 / 16695.0, 393 / 640.0, -92097 / 339200.0, 187 / 2100.0, 1 / 40.0});
    }

}
