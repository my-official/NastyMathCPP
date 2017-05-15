#include "stdafx.h"
#include "Sum.h"
#include "Composition.h"
#include "..\Parser.h"
#include "..\Utils.h"



Sum::Sum(const string& expression)
{
	Parser().Parse(expression, *this);
}


Sum::Sum(const char* expression)
{
	Parser().Parse(expression, *this);
}


Sum::Sum(int value)
{
	if (value != 0)
		emplace_back(value);
}

Sum::Sum(float value) : Sum( static_cast<double>(value) )
{
	
}

Sum::Sum(double value)
{
	pair<int, int> ratio = GetRatio(value);
	emplace_back(ratio.first);
	if (ratio.second != 1)
	{
		back().m_Dividers.emplace_back(ratio.second);
	}
}


Sum::Sum(const Varrible& var)
{
	Composition comp;
	comp.m_Scalar.m_Varribles.push_back(var);
	emplace_back( std::move(comp) );
}



bool Sum::operator==(const Sum& rhs) const
{
	Sum a(*this);
	Sum b(rhs);


	/*a.Simplify(Simplification::SumOfFlatProducts);
	b.Simplify(Simplification::SumOfFlatProducts);*/

	a.ToCanonicalView();
	b.ToCanonicalView();

	if (a.size() != b.size())
		return false;

	return is_permutation(a.begin(), a.end(), b.begin());
}

Sum& Sum::operator+=(const Sum& rhs)
{
	copy(rhs.begin(), rhs.end(), back_inserter(*this));
	return *this;
}

Sum& Sum::operator-=(const Sum& rhs)
{
	int preSize = size();
	copy(rhs.begin(), rhs.end(), back_inserter(*this));

	for (int c = preSize, newSize = size(); c < newSize; c++)
	{
		this->operator[](c) *= -1;		
	}	
	return *this;
}

Sum Sum::operator-() const
{
	Sum result(*this);
	result *= -1;
	return result;
}

Sum& Sum::operator*=(const Sum& rhs)
{
	if (empty())
		return *this;

	if (rhs.empty())
	{
		clear();
		return *this;		
	}


	for (auto& lhsSummand : *this)
	{
		lhsSummand *= rhs;
	}

	return *this;
}

Sum& Sum::operator/=(const Sum& rhs)
{
	if (empty())
		return *this;

	if (rhs.empty())
		throw EXCEPTION;


	for (auto& lhsSummand : *this)
	{
		lhsSummand /= rhs;		
	}

	return *this;
}

bool Sum::IsSymbolic() const
{
	for (auto& summand : *this)
	{
		if (summand.IsSymbolic())
			return true;
	}
	return false;
}

std::vector<std::string> Sum::GetSymbols() const
{
	return std::vector<std::string>();
}

void Sum::GetVarribles(vector<Varrible>& varribles) const
{
	for (const Composition& summand : *this)
	{
		summand.GetVarribles(varribles);
	}
	
}

Sum& Sum::Substitute(string name, const Sum& value)
{
	for (auto& summand : *this)
	{
		summand.Substitute(name, value);
	}
	return *this;
}

Sum& Sum::Substitute(const Varrible& varrible, const Sum& value)
{
	for (auto& summand : *this)
	{
		summand.Substitute(varrible, value);
	}
	return *this;
}

Sum& Sum::Approximate()
{
	for (auto& summand : *this)
	{
		summand.Approximate();
	}
	return *this;
}

std::string Sum::AsString() const
{
	if (this->empty())
		return "0";
	
	std::string result;
		

	for (const auto& summand : *this)
	{
		result += summand.AsString() + " + ";
	}

	result.pop_back();
	result.pop_back();
	result.pop_back();
	

	return result;
}


std::string Sum::AsLatexCode() const
{
	if (this->empty())
		return "0";

	std::string result;

	for (const Composition& summand : *this)
	{
		result += summand.AsLatexCode() + " + ";
	}

	result.pop_back();
	result.pop_back();
	result.pop_back();

	return result;
}



Sum& Sum::Simplify()
{			
	for (Composition& summand : *this)
	{		
		//Рекурсивное полное упрощение (или аддитивное упрощение?)
		//Избавление от цепных дробей
		//Разложение на произведение сумм и деление
		summand.Simplify();
		
		//Раскрытие скобок		
//		summand.ToSumOfFlatRatios(sumsOfFlatRatio);
	}

	SimplifyAdditive();
	return *this;

	
	SumOfFlatRatios sumsOfFlatRatio;
	SumOfScalar commonDenominator;
	SumOfScalar commonNumerator;
	
	ToCommonDenominator(sumsOfFlatRatio, commonNumerator, commonDenominator);
	
	SimplifyFlatAdditive(commonNumerator);
	SimplifyFlatAdditive(commonDenominator);

	FlatRatio flatRatio;
	flatRatio.Numerator = move(commonNumerator);
	flatRatio.Denominator = move(commonDenominator);

	flatRatio.SimplifyFlatMultiplicative();


	//SimplifyFlatMultiplicative(sumsOfFlatRatio);

	return *this;
	


	//a / (a + b)	  + b / (a + b)

	//a  / (a + b)	  + b / (a + b)
	

	//Sum ratioOfSumsOfScalars;
	//for (auto& summand : *this)
	//{
	//	summand.Simplify();
	//	summand.ToFlatSum(ratioOfSumsOfScalars);
	//}
	//
	//ratioOfSumsOfScalars.ToGeneralDenominator();
	//ratioOfSumsOfScalars.front().m_Multipliers.front().SimplifyAdditive();
	//ratioOfSumsOfScalars.front().SimplifyMultiplicative();
	
	return *this;

	/*
		vector< pair<FlatSumOfScalar, FlatSumOfScalar> > sum;

		Sum ratioOfSumsOfScalars;


		for (auto& summand : *this)
		{
			summand.Simplify();
			Sum ratioOfSumsOfScalars = summand.ToFlatSum();


			summand.DecomposeCyclicFractions();
			summand.SimplifyMultiplicative();

			FlatSumOfScalar numerator;
			FlatSumOfScalar denominator;
			Sum ratioOfSumsOfScalars = summand.ToRatioOfFlatSumsOfScalars(numerator, denominator);

			sum.push_back( make_pair(numerator, denominator) );
		}

		SumOfFlatComposition generalNumerator;
		FlatComposition generalDenominator;
		Composition singleComposition = ToGeneralDenominator(sum, generalDenominator, generalNumerator);

		generalNumerator.SimplifyAdditive();

		singleComposition.SimplifyMultiplicative();

		return *this;*/
}


Sum& Sum::ToCanonicalView()
{
	Simplify();
	ToFlatSumOfCompositions_Expand();	
	//AlphabeticalSort();
	return *this;
}

Sum& Sum::ToFlatSumOfCompositions_Expand()
{
	//Sum result;

	//for (auto& summand : *this)
	//{
	//	summand.ToFlatSum(result);
	//}

	//*this = std::move(result);

	return *this;
}

bool Sum::ToCompositionOfSums_Factor(vector<Sum>& newMultipliers)
{
	return true;
}


Sum& Sum::AlphabeticalSort()
{
	for (auto& s : *this)
	{
		s.AlphabeticalSort();
	}

	std::sort(begin(), end(), &Composition::alphabetical_less);
	return *this;
}

bool Sum::alphabetical_less(const Sum& lhs, const Sum& rhs)
{
	return lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end(), &Composition::alphabetical_less);
}

Sum& Sum::SimplifyAdditive()
{
	if (empty())
		return *this;

	//for (auto& summand : *this)
	//{
	//	summand.Simplify();
	//}

	if (size() > 1)
	{
		for (auto it = begin(); it != end() - 1; )
		{
			Composition& a = *it;
			Composition& b = *(it + 1);

			if (a.m_Scalar.SymbolicPart() == b.m_Scalar.SymbolicPart())
			{
				b.m_Scalar.NumericPart() += a.m_Scalar.NumericPart();

				it = erase(it);
			}
			else
			{
				++it;
			}
		}
	}

	for (auto it = begin(); it != end(); )
	{
		if (*it == 0)
			it = erase(it);
		else
			++it;
	}

	return *this;
}





void Sum::ToCommonDenominator(SumOfFlatRatios& sumsOfFlatRatio, SumOfScalar& numerator, SumOfScalar& denominator)
{
	if (sumsOfFlatRatio.size() < 2)
		return;
			
	denominator = SumOfScalar(1);

	for (int c_ratio = 0, size_ratio = sumsOfFlatRatio.size(); c_ratio < size_ratio; c_ratio++)
	{
		FlatRatio& ratio = sumsOfFlatRatio[c_ratio];				
		
		for (int c_other_ratio = 0, size_other_ratio = c_ratio; c_other_ratio < size_other_ratio; c_other_ratio++)
		{			
			if (c_ratio == c_other_ratio)
				continue;
			
			FlatRatio other_ratio = sumsOfFlatRatio[c_other_ratio];
			
			if ( FlatRatio::DenominatorEqual(ratio, other_ratio) )
				continue;

			ratio.Numerator *= other_ratio.Denominator;
			other_ratio.Numerator *= ratio.Denominator;
						
			denominator *= ratio.Denominator * other_ratio.Denominator;
		}
	}


	numerator = SumOfScalar(1);

	for (auto& ratio : sumsOfFlatRatio)
	{
		numerator *= ratio.Numerator;
	}	
}

void Sum::SimplifyFlatAdditive(SumOfScalar& sumOfScalar)
{
	for (auto it = sumOfScalar.begin(); it != sumOfScalar.end(); )
	{
		Scalar& a = *it;
		if (a.m_Coefficient == 0)
		{
			it = sumOfScalar.erase(it);
		}
		else
		{
			++it;
		}
	}

	if (sumOfScalar.size() < 2)
		return;

	for (auto it = sumOfScalar.begin(); it != sumOfScalar.end(); )
	{
		Scalar& a = *it;

		auto ret = find_if(it + 1, sumOfScalar.end(), [&a](const Scalar& rhs) {  return Scalar::equal_symolic_part(a, rhs); });
		if (ret != sumOfScalar.end())
		{
			Scalar& b = *ret;

			b.m_Coefficient += a.m_Coefficient;

			if (b.m_Coefficient == 0)
			{
				sumOfScalar.erase(ret);
			}

			it = sumOfScalar.erase(it);
		}
		else
		{
			++it;
		}
	}
}


std::string to_string(const Sum& val)
{
	// convert int to string
	return val.AsString();
}

std::string to_latex(const Sum& val)
{
	return val.AsLatexCode();
}

bool FlatRatio::DenominatorEqual(const FlatRatio& lhs, const FlatRatio& rhs)
{
	//if (lhs.size() != rhs.size())
	//	return false;

//return equal(lhs.begin(), lhs.end(), rhs.begin() );
	return true;
}
