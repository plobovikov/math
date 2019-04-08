#pragma once

template<class NumericalField>
struct IterationSolverPolicy {

public:

	class IterationSolverListener {
	public:
		virtual void onNextIteration(NumericalField) = 0;
	};

	typedef IterationSolverListener SolverListenerType;

	typedef std::shared_ptr<SolverListenerType> SolverListenerPtrType;

	typedef std::set<SolverListenerPtrType> SolverListenerPtrCollectionType;

	class AdditionalInfoType { };

	typedef std::function<NumericalField(NumericalField)> FunctionType;

	typedef NumericalField IterationItemType;

	typedef std::function<bool(IterationItemType, IterationItemType)> PrecisionPredicateType;


	static IterationItemType NextIteration(
		const IterationItemType& item,
		const FunctionType& function,
		const AdditionalInfoType& additionalInfo,
		const SolverListenerPtrCollectionType& listeners) {

		auto f = function(item);
		auto xn = f;

		for (auto pListener : listeners) {
			pListener->onNextIteration(xn);
		}

		return xn;
	}

	static bool CheckPrecision(
		PrecisionPredicateType predicate,
		const IterationItemType& previous,
		const IterationItemType& current) {

		return predicate(previous, current);

	}

	static NumericalField ChoiceSolution(
		IterationItemType /*previous*/,
		IterationItemType current) {

		return current;
	}
};