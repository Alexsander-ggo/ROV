#include "SolverRK4.h"

namespace ode {

    void SolverRK4::initButcherTable()
    {
        mSteps = {0, 0.5, 0.5, 1};
        mWeights.push_back({0, 0, 0, 0});
        mWeights.push_back({0.5, 0, 0, 0});
        mWeights.push_back({0, 0.5, 0, 0});
        mWeights.push_back({0, 0, 1, 0});
        mOutWeights.push_back({1 / 6.0, 1 / 3.0, 1 / 3.0, 1 / 6.0});
    }

}
