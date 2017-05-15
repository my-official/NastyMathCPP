#pragma once
#include "Composition.h"
//////////////////////////////////////////////////////////////////////////
//0.12222222f * 2 / 3 * Pi*exp(t)*t*f(t)*sin(cos(f(t ^ 2)))*e * 3 ^ (2 / 3)*sqrt(4)*sqrt(3)*tg(Pi)

class Composition;

enum class Simplification
{
	SumOfFlatProducts,
};


enum class SimplificationMode
{
	Auto,
	Manual,
};


class Varrible;




class Sum : public vector< Composition >, boost::operators<Sum>
{
public:
	Sum() = default;
	Sum(const char expression) = delete;
	Sum(int value);
	Sum(float value);
	Sum(double value);
	Sum(const string& expression);
	Sum(const Varrible& var);
	
	Sum(const char* expression);

	

	string m_Name;

	bool operator==(const Sum& rhs) const;

	Sum& operator+=(const Sum& rhs);
	Sum& operator-=(const Sum& rhs);

	Sum& operator*=(const Sum& rhs);
	Sum& operator/=(const Sum& rhs);

	Sum operator-() const;


	bool IsSymbolic() const;
	vector<string> GetSymbols() const;
	void  GetVarribles(vector<Varrible>& varribles) const;

	Sum& Substitute(string name, const Sum& value);
	
	template <class Iterator>
	Sum& Substitute(Iterator first, Iterator last, const Sum& value);

	Sum& Substitute(const Varrible& varrible, const Sum& value);


	Sum& Approximate();
	string AsString() const;
	string AsLatexCode() const;

	/*static Sum FromGinacEx(const GiNaC::ex& expression);
	GiNaC::ex AsGinacEx() const;*/

	Sum& Simplify();

	Sum& ToCanonicalView();
	Sum& ToFlatSumOfCompositions_Expand();
	bool ToCompositionOfSums_Factor(vector<Sum>& newMultipliers);

	Sum& AlphabeticalSort();

	static bool alphabetical_less(const Sum& lhs, const  Sum& rhs);

	Sum& SimplifyAdditive();	
private:
	void ToCommonDenominator(SumOfFlatRatios& sumsOfFlatRatio, SumOfScalar& numerator, SumOfScalar& denominator);
	void SimplifyFlatAdditive(SumOfScalar& sumOfScalar);
	
};

string to_string(const Sum& val);
string to_latex(const Sum& val);










#include "Sum.inl"