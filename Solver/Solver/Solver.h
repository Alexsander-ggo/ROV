#ifndef SOLVER_H
#define SOLVER_H

#include <cstdint>
#include <vector>

namespace ode {

    using Values = std::vector<double>;
    using System = Values (*) (double, const Values&);
    using Analytic = Values (*) (double);

    class Solver
    {
    public:
        Solver(double step = 1e-3);

        virtual void initButcherTable() = 0;

        void init(double initTime, const Values& initValues);

        void setSystem(System system);

        void setAnalytic(Analytic analytic);

        double getCurrentTime() const;

        Values getValues() const;

        double getMaxError() const;

        double getMinStep() const;

        uint64_t getCountSteps() const;

        void calc(double time);

    private:
        void calcStep();

        Values recalcSystem(double time, const Values& Values);

    protected:
        double mMinTolerance;
        double mMaxTolerance;
        double mMaxError;
        double mMaxErrorCurrent;
        double mTime;
        double mStep;
        double mMinStep;
        uint64_t mStepCounter;
        System mSystem;
        Analytic mAnalytic;
        Values mValues;
        Values mSteps;
        std::vector<Values> mWeights;
        std::vector<Values> mOutWeights;
    };

}

#endif // SOLVER_H
