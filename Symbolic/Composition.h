#pragma once
#include "Scalar.h"


class Sum;






class Composition : boost::operators<Composition>
{
public:
	Scalar m_Scalar;
	vector< Sum > m_Multipliers;
	vector< Sum > m_Dividers;

	Composition() : m_Scalar(1)
	{
	}
	Composition(int value);

	bool operator==(const Composition& rhs) const;

	Composition& operator*=(const Sum& rhs);
	Composition& operator/=(const Sum& rhs);

	Composition& operator*=(const Composition& rhs);
	Composition& operator/=(const Composition& rhs);

	Composition& operator*=(int factor);

	bool IsSymbolic() const;
	Composition& Substitute(string name, const Sum& value);
	Composition& Substitute(const Varrible& varrible, const Sum& value);
	Composition& Approximate();
	string AsString() const;
	string AsLatexCode() const;

	void GetVarribles(vector<Varrible>& varribles) const;
	

	void ToFlatCompositionOfSums_Factor();
	void ToFlatSumOfCompositions(Sum& sum);


	Composition& AlphabeticalSort();
	static bool alphabetical_less(const Composition& lhs, const  Composition& rhs);	
	Composition& ToCanonicalView();


	Composition& Simplify();
	Composition& ReduceSimilarMulsAndDivs();
	Composition& ReduceScalarAndDiv();


	Composition& DecomposeCyclicFractions();

	/*GiNaC::ex AsGinacEx() const;
	static Composition FromGinacEx(const GiNaC::ex& expression);*/

	bool IsScalarOnly() const;
private:

	template <class Functor>
	void ForEachMultipliersAndDividers(Functor functor);

	template <class Functor>
	void ForEachMultipliersAndDividers(Functor functor) const;

	template <class Functor>
	bool ForEachMultipliersAndDividersWhileTrue(Functor functor) const;

	template <class Functor>
	bool ForEachMultipliersAndDividersWhileFalse(Functor functor) const;
	
	
	Composition& SimplifyMultiplicative();
};

//
//class FlatComposition : public Composition, private boost::operators<Composition>
//{
//
//};


#include "Composition.inl"
