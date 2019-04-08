#pragma once

template<template<class>class SolverPolicy, class NumericalField>
class NonlinearEquationSolver {

public:

	typedef std::function<bool(NumericalField, NumericalField)> PrecisionPredicateType;

	typedef std::function<NumericalField(NumericalField)> FunctionType;

	typedef typename SolverPolicy<NumericalField>::SolverListenerPtrType SolverListenerPtrType;

	typedef typename SolverPolicy<NumericalField>::SolverListenerPtrCollectionType SolverListenerPtrCollectionType;

	typedef typename SolverPolicy<NumericalField>::AdditionalInfoType AdditionalInfoType;

	typedef typename SolverPolicy<NumericalField>::IterationItemType IterationItemType;


	NonlinearEquationSolver() = default;

	~NonlinearEquationSolver() = default;

	template<class BinaryPredicate>
	void SetPrecisionPredicate(BinaryPredicate predicate) {
		m_Predicate = predicate;
	}

	template<class UnaryFunction>
	void SetFunction(UnaryFunction function) {
		m_Function = function;
	}

	template<class AdditionalInfoType>
	void SetAdditionalInfo(AdditionalInfoType additionalInfo) {
		m_AdditionalInfo = additionalInfo;
	}

	void AddListener(SolverListenerPtrType pListener) {
		m_Listeners.insert(pListener);
	}

	void SetMaxIteration(unsigned int unMaxIteration) {
		m_unMaxIteration = unMaxIteration;
	}

	NumericalField FindSolution(const IterationItemType& item) {

		unsigned long unIterationNum{ 0 };

		IterationItemType iterationResult{ item }, previousIterationResult{ item };

		do {
			previousIterationResult = iterationResult;

			iterationResult =
				SolverPolicy<NumericalField>::NextIteration(iterationResult,
					m_Function,
					m_AdditionalInfo,
					m_Listeners);
			unIterationNum++;
		} while (!SolverPolicy<NumericalField>::CheckPrecision(m_Predicate,
			previousIterationResult,
			iterationResult)
			&& unIterationNum < m_unMaxIteration);

		return SolverPolicy<NumericalField>::ChoiceSolution(previousIterationResult, iterationResult);
	}

	NumericalField ComputeDiscrepancy(NumericalField result) {

		return fabs(m_Function(result));
	}

private:

	SolverListenerPtrCollectionType m_Listeners;
	PrecisionPredicateType m_Predicate;
	FunctionType m_Function;
	AdditionalInfoType m_AdditionalInfo;
	unsigned long m_unMaxIteration{ 1000 };

};
