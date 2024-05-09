#include "Solver.h"

#include <cmath>

namespace ode {

    Solver::Solver(double step)
         : mMinTolerance(1e-8)
         , mMaxTolerance(1e-5)
         , mStep(step)
    {

    }

    void Solver::init(double initTime, const Values& initValues)
    {
        mTime = initTime;
        mValues = initValues;
        mMaxError = 0;
        mMinStep = -1;
        mStepCounter = 0;
    }

    void Solver::setSystem(System system)
    {
        mSystem = system;
        mAnalytic = nullptr;
    }

    void Solver::setAnalytic(Analytic analytic)
    {
        mAnalytic = analytic;
    }

    double Solver::getCurrentTime() const
    {
        return mTime;
    }

    Values Solver::getValues() const
    {
        return mValues;
    }

    double Solver::getMaxError() const
    {
        return mMaxError;
    }

    double Solver::getMinStep() const
    {
        return mMinStep;
    }

    uint64_t Solver::getCountSteps() const
    {
        return mStepCounter;
    }

    void Solver::calc(double time)
    {
        static const double errorTime = 1e-5;
        if (mOutWeights.size() > 1) {
            mStep = time - mTime;
        }
        do {
            calcStep();
            if (mTime + mStep - time > errorTime) {
                mStep = time - mTime;
                calcStep();
            }
            mTime += mStep;
            ++mStepCounter;
            if (mStep < mMinStep || mMinStep < 0) {
                mMinStep = mStep;
            }
            if (mOutWeights.size() > 1) {
                if (mMaxErrorCurrent < mMinTolerance) {
                    mStep *= 2;
                }
            }
        } while (time - mTime > errorTime);
    }

    void Solver::calcStep()
    {
        Values values;
        do {
            std::vector<Values> temp;
            values = mValues;
            temp.push_back(recalcSystem(mTime, values));
            for (uint64_t i = 1; i < mSteps.size(); ++i) {
                Values values = mValues;
                for (uint64_t j = 0; j < i; ++j) {
                    for (uint64_t k = 0; k < mValues.size(); ++k) {
                        values[k] += temp.back()[k] * mWeights[i][j] * mStep;
                    }
                }
                temp.push_back(recalcSystem(mTime + mSteps[i] * mStep, values));
            }
            Values solution1 = values;
            Values solution2 = values;
            for (uint64_t i = 0; i < values.size(); ++i) {
                for (uint64_t j = 0; j < temp.size(); ++j) {
                    solution1[i] += mOutWeights.front()[j] * temp[j][i] * mStep;
                    solution2[i] += mOutWeights.back()[j] * temp[j][i] * mStep;
                }
            }
            mMaxErrorCurrent = 0;
            for (uint64_t i = 0; i < solution1.size(); ++i) {
                double error = std::abs(solution1[i] - solution2[i]);
                if (error > mMaxErrorCurrent) {
                    mMaxErrorCurrent = error;
                }
            }
            if (mMaxErrorCurrent > mMaxTolerance) {
                mStep /= 2;
            } else {
                values = solution1;
            }

        } while (mMaxErrorCurrent > mMaxTolerance);
        if (mAnalytic) {
            Values solvers = mAnalytic(mTime + mStep);
            for (uint64_t i = 0; i < values.size(); ++i) {
                double error = std::abs(values[i] - solvers[i]);
                if (error > mMaxError) {
                    mMaxError = error;
                }
            }
        }
        mValues = values;
    }

    Values Solver::recalcSystem(double time, const Values& values)
    {
        return mSystem(time, values);
    }

}
