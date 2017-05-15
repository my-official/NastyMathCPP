#pragma once



class Varrible : boost::operators<Varrible>
{
public:
	Varrible(const string& name);
	string m_Name;
	

	string AsLatexCode() const;

	bool operator==(const Varrible& rhs) const;
		
	static bool alphabetical_less(const Varrible& lhs, const  Varrible& rhs);
};

//typedef char Varrible;
typedef string Constant;

class ExplicitFunc
{
public:
};

class ImplicitFunc
{
public:
};


class Sum;
class Composition;
class SumOfScalar;

class Scalar : boost::multipliable< Scalar, boost::equality_comparable<Scalar> >

{
public:
	float m_Coefficient = 1.f;
	vector<int> m_Numbers;
	//vector<Constant> m_Consts;
	vector<Varrible> m_Varribles;
	//vector<ExplicitFunc> m_ExplicitFuncs;
	//vector<ImplicitFunc> m_ImplicitFuncs;	

	Scalar(int value);
	Scalar(const Varrible& varrible);


	bool operator==(const Scalar& rhs) const;
	Scalar& operator*=(const Scalar& rhs);
	Scalar& operator*=(int factor);
	Scalar& operator*=(const Varrible& varrible);

	SumOfScalar operator+(const Scalar& rhs) const;

	
	void Clear();

	Scalar& Substitute(string name, const Sum& value, Composition& composition);
	Scalar& Substitute(const Varrible& varrible, const Sum& value, Composition& composition);

	Scalar& Approximate();
	string AsString() const;
	string AsLatexCode() const;
	/*GiNaC::ex AsGinacEx() const;*/

	Scalar& Simplify();

	static bool alphabetical_less(const Scalar& lhs, const  Scalar& rhs);
	Scalar& AlphabeticalSort();
	Scalar SymbolicPart();
	float& NumericPart();
	static void Reduce(Scalar& numerator, Scalar& denominator);

	static bool equal_symolic_part(const Scalar& lhs, const Scalar& rhs);
	
	

	
	void GetVarribles(vector<Varrible>& varribles) const;
};


class SumOfScalar;
using CompositionOfFlatSum = vector<SumOfScalar>;
class LatexContext;

class SumOfScalar : public vector<Scalar>, boost::operators<SumOfScalar>
{
public:
	SumOfScalar() = default;
	SumOfScalar(const Scalar& scalar);

	//bool operator==(const Scalar& rhs) const;
	SumOfScalar& operator+=(const SumOfScalar& rhs);
	SumOfScalar& operator*=(const SumOfScalar& rhs);
	
	SumOfScalar& operator+=(const Scalar& rhs);


	vector<Varrible> TakeOutCommonVarribles();

	bool  GroupBy(const vector<Varrible>&, multimap<Varrible, SumOfScalar>& grouped);


	static bool alphabetical_less(const Scalar& lhs, const  Scalar& rhs);

	bool ToCompositionOfFlatSum(CompositionOfFlatSum& result, LatexContext* latexContext);
private:
	bool ToCompositionOfFlatSumInternal(CompositionOfFlatSum& result, string& latexCode) const;

};


struct FlatRatio
{
	SumOfScalar Numerator;
	SumOfScalar Denominator;

	static bool DenominatorEqual(const FlatRatio& lhs, const FlatRatio& rhs);
	void SimplifyFlatMultiplicative();

	
};

using SumOfFlatRatios = vector<FlatRatio>;



SumOfScalar ToSumOfFlat(const CompositionOfFlatSum& composition);

//
//class CompositionOfFlatSum : public vector<SumOfScalar>
//{
//public:
//
//
//};



//using SumOfScalar = vector<Scalar>;


using VarVector = vector<Varrible>;

class CommonFactors
{
public:
	vector< vector<VarVector> > m_Factors;
	bool HasAtLeastOneCommonFactor(int summandIndex);
	bool HasSingleCommonFactor(VarVector& );
};




