#ifndef SOLVERDP_H
#define SOLVERDP_H

#include "Solver.h"

namespace ode {

    class SolverDP : public Solver
    {
    public:
        using Solver::Solver;

        void initButcherTable() override;
    };

}

#endif // SOLVERDP_H
