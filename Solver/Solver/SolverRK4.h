#ifndef SOLVERRK4_H
#define SOLVERRK4_H

#include "Solver.h"

namespace ode {

    class SolverRK4 : public Solver
    {
    public:
        using Solver::Solver;

        void initButcherTable() override;
    };

}

#endif // SOLVERRK4_H
