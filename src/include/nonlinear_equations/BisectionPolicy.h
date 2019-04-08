#pragma once

template<class NumericalField>
struct BisectionSolverPolicy {
private:

	static bool Sign(NumericalField number) {
		return number > 0;
	}

public:

	class BisectionSolverListener {
	public:
		virtual void onNextIteration(NumericalField, NumericalField) = 0;
	};

	class BisectionSolverAdditionalInfo { };

	typedef BisectionSolverListener SolverListenerType;

	typedef std::shared_ptr<SolverListenerType> SolverListenerPtrType;

	typedef std::set<SolverListenerPtrType> SolverListenerPtrCollectionType;

	typedef BisectionSolverAdditionalInfo AdditionalInfoType;

	typedef std::function<NumericalField(NumericalField)> FunctionType;

	typedef std::pair<NumericalField, NumericalField> IterationItemType;

	typedef std::function<bool(NumericalField, NumericalField)> PrecisionPredicateType;


	static IterationItemType NextIteration(
		const IterationItemType& item,
		const FunctionType& function,
		const AdditionalInfoType& /*additionalInfo*/,
		const SolverListenerPtrCollectionType& listeners) {

		auto half = (item.first + item.second) / 2.0;
		auto valueOnLeft = function(item.first);
		auto valueOnHalf = function(half);
		auto valueOnRigth = function(item.second);

		IterationItemType result = (Sign(valueOnLeft) == Sign(valueOnHalf) ? std::make_pair(half, item.second) : std::make_pair(item.first, half));

		for (auto pListener : listeners) {
			pListener->onNextIteration(result.first, result.second);
		}

		return result;
	}

	static bool CheckPrecision(
		PrecisionPredicateType predicate,
		const IterationItemType& /*previous*/,
		const IterationItemType& current) {

		return predicate(current.first, current.second);
	}

	static NumericalField ChoiceSolution(
		IterationItemType /*previous*/,
		IterationItemType current) {

		return (current.first + current.second) / 2.0;
	}
};