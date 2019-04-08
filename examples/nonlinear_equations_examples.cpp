#include <iostream>
#include <functional>
#include <memory>
#include <vector>
#include <iostream>
#include <set>

#include "../src/include/nonlinear_equations/BisectionPolicy.h"
#include "../src/include/nonlinear_equations/TangentPolicy.h"
#include "../src/include/nonlinear_equations/IterationPolicy.h"
#include "../src/include/nonlinear_equations/NonlinearEquationSolver.h"


template<class NumericalField>
class BisectionListener : 
	public BisectionSolverPolicy<NumericalField>::BisectionSolverListener {
public:
	void onNextIteration(double x1, double x2) {
		std::cout << " Iteration = " << x1 << " " << x2 << std::endl;
	 }
};

template<class NumericalField>
class TangentListener :
	public TangentSolverPolicy<NumericalField>::TangentSolverListener {
public:
	void onNextIteration(double xn) {
		std::cout << " Iteration = " << xn << std::endl;
	}
};


template<class NumericalField>
class IterationListener :
	public IterationSolverPolicy<NumericalField>::IterationSolverListener {
public:
	void onNextIteration(double xn) {
		std::cout << " Iteration = " << xn << std::endl;
	}
};

int main()
{
	{
		NonlinearEquationSolver<BisectionSolverPolicy, double> solver;

		solver.AddListener(std::make_shared<BisectionListener<double>>());

		solver.SetMaxIteration(10000);

		solver.SetPrecisionPredicate([](double previous, double current) -> bool {
			return (fabs(previous - current) < 0.0000001);
		});

		solver.SetFunction([](double x) -> double {
			return std::pow(x, 3) - 0.2 * std::pow(x, 2) + 0.5 * x + 1.5;
		});

		auto result = solver.FindSolution({ -1, 0 });

		auto discrepancy = solver.ComputeDiscrepancy(result);

		std::cout << "Result = " << result << " with discrepancy = " << discrepancy << std::endl;
	}
	
	{
		NonlinearEquationSolver<TangentSolverPolicy, double> solver;

		solver.AddListener(std::make_shared<TangentListener<double>>());

		solver.SetMaxIteration(10000);

		solver.SetPrecisionPredicate([](double previous, double current) -> bool {
			return (fabs(previous - current) < 0.0000001);
		});

		solver.SetFunction([](double x) -> double {
			return std::pow(x, 3) + 4 * x - 3;
		});

		solver.SetAdditionalInfo([](double x) -> double {
			return 3*std::pow(x, 2) + 4;
		});

		auto result = solver.FindSolution({ 1 });

		auto discrepancy = solver.ComputeDiscrepancy(result);

		std::cout << "Result = " << result << " with discrepancy = " << discrepancy << std::endl;
	}

	{
		NonlinearEquationSolver<IterationSolverPolicy, double> solver;

		solver.AddListener(std::make_shared<IterationListener<double>>());

		solver.SetMaxIteration(10000);

		solver.SetPrecisionPredicate([](double previous, double current) -> bool {
			return (fabs(previous - current) < 0.0000001);
		});

		/* std::pow(x, 3) + 4 * x - 3 - for this function we have following equation: x = (3 - x ^ 3) / 4 */
		solver.SetFunction([](double x) -> double {
			return (3 - std::pow(x, 3) )/ 4;
		});

		auto result = solver.FindSolution({ 1 });

		auto discrepancy = solver.ComputeDiscrepancy(result);

		std::cout << "Result = " << result << " with discrepancy = " << discrepancy << std::endl;
	}
	return 0;
}