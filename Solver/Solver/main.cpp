#include "SolverDP.h"
#include "SolverRK4.h"

#include <cmath>
#include <iomanip>
#include <iostream>

ode::Values System(double a, double b, double t,
                   const ode::Values& values) {
    double y = values.at(0);
    return {a * t - b * y};
}

ode::Values Analytic(double a, double b, double d, double t) {
    return {a / b * (t - 1 / b) + (d + a / (b * b)) * std::exp(-b * t)};
}

ode::Values System2(double t, const ode::Values& values) {
    double u1 = values.at(0);
    double u2 = values.at(1);
    double c = std::cos(t);
    double s = std::sin(t);
    return {9 * u1 + 24 * u2 + 5 * c - 1 / 3.0 * s,
            -24 * u1 - 51 * u2 - 9 * c + 1 / 3.0 * s};
}

ode::Values Analytic2(double t) {
    double e1 = std::exp(-3 * t);
    double e2 = std::exp(-39 * t);
    double c = std::cos(t);
    return {2 * e1 - e2 + 1 / 3.0 * c,
            -e1 + 2 * e2 - 1 / 3.0 * c};
}

const int Size = 13;
double a[Size] = {1, -2, 0.3, -0.7, 1.2, -0.8, 2.5, -2, 0.3, -0.7, 1.2, -0.8, 2.5};
double b[Size] = {0.2, 0.2, 0.2, 0.7, 0.7, 0.7, 2.1, 2.1, 2.1, 1.3, 1.3, 1.3, 1.3};
double d[Size] = {1, 1, 1, 1, 0, 0, 0, 0, 0.5, 0.5, 0.5, 0.5, 0.5};
int index = -1;

template <typename Solver>
void SolverTest() {
    std::cout << "System equals:" << std::fixed << std::endl;
    double time = 1;
    do {
        Solver solver(1e-2);
        solver.initButcherTable();
        if (index >= 0) {
            solver.init(0, {d[index]});
            solver.setSystem([] (double t, const ode::Values& values) {
                return System(a[index], b[index], t, values);
            });
            solver.setAnalytic([] (double t) {
                return Analytic(a[index], b[index], d[index], t);
            });
        } else {
                solver.init(0, {4 / 3.0, 2 / 3.0});
                solver.setSystem(System2);
                solver.setAnalytic(Analytic2);
        }
        solver.calc(time);
        ode::Values values = solver.getValues();
        for (uint8_t i = 0; i < values.size(); ++i) {
            std::cout << "Y" << (i + 1) << "(" << std::setprecision(5)
                      << solver.getCurrentTime() << ") = "
                      << std::setprecision(15) << values.at(i) << std::endl;
        }
        std::cout << "Max error: " << solver.getMaxError() << std::endl;
        std::cout << "Min step: " << solver.getMinStep() << std::endl;
        std::cout << "Count steps: " << solver.getCountSteps() << std::endl;
        ++index;
        if (index < Size) {
            std::cout << "Variant " << (index + 1) << ":" << std::endl;
        }
    } while (index < Size);
    index = -1;
}

int main()
{
    std::cout << "Test SolverRK4" << std::endl;
    SolverTest<ode::SolverRK4>();
    std::cout << "Test SolverDP" << std::endl;
    SolverTest<ode::SolverDP>();
    return 0;
}
