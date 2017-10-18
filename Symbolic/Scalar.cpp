#include "stdafx.h"
#include "Scalar.h"
#include "Composition.h"
#include "Sum.h"
#include "..\Utils.h"
#include "LatexContext.h"




Varrible::Varrible(const string& name) : m_Name(name)
{

}

std::string Varrible::AsLatexCode() const
{
	return m_Name;
}

bool Varrible::operator==(const Varrible& rhs) const
{
	return m_Name == rhs.m_Name;
}


bool Varrible::alphabetical_less(const Varrible& lhs, const Varrible& rhs)
{
	return lhs.m_Name < rhs.m_Name;
}


Scalar::Scalar(int value) : m_Coefficient(float(value))
{
	
}

Scalar::Scalar(const Varrible& varrible)
{
	m_Varribles.push_back(varrible);
}


SumOfScalar Scalar::operator+(const Scalar& rhs) const
{
	SumOfScalar result(*this);
	result.push_back(rhs);
	return result;
}

Scalar& Scalar::Substitute(string name, const Sum& value, Composition& composition)
{
	auto it = find(m_Varribles.begin(), m_Varribles.end(), name);

	while (it != m_Varribles.end())
	{
		m_Varribles.erase(it);


		if (value.size() == 1)
		{
			const Composition& rhsComposition = value[0];
			(*this) *= rhsComposition.m_Scalar;
			
			copy(rhsComposition.m_Multipliers.begin(), rhsComposition.m_Multipliers.end(), back_inserter(composition.m_Multipliers));
			copy(rhsComposition.m_Dividers.begin(), rhsComposition.m_Dividers.end(), back_inserter(composition.m_Dividers));
		}
		else
		{
			composition.m_Multipliers.push_back(value);
		}

		it = find(m_Varribles.begin(), m_Varribles.end(), name);
	}

	return *this;
}

Scalar& Scalar::Substitute(const Varrible& varrible, const Sum& value, Composition& composition)
{
	auto it = remove(m_Varribles.begin(), m_Varribles.end(), varrible);
	auto itCopy = it;

	while (it != m_Varribles.end())
	{
		composition.m_Multipliers.push_back(value);
		++it;
	}

	m_Varribles.erase(itCopy, m_Varribles.end());
	return *this;
}

Scalar& Scalar::Approximate()
{
	return *this;
}

std::string Scalar::AsString() const
{
	std::string result;

	if (!FCmp(m_Coefficient, 1.f))
		result += to_string(m_Coefficient);

	if (!result.empty())
	{
		result += '*';
	}


	for (auto num : m_Numbers)
	{
		if (num >= 0)
		{
			result += to_string(num) + '*';
		}
		else
		{
			result += "(" + to_string(num) + ")" + '*';
		}
	}

	for (auto var : m_Varribles)
	{
		result += var.m_Name;
		result += '*';
	}

	if (!result.empty())
		result.pop_back();	

	if (FCmp(m_Coefficient, 1.f) && result.empty())
		result += to_string(m_Coefficient);

	return result;
}

std::string Scalar::AsLatexCode() const
{
	std::string result;

	if (!FCmp(m_Coefficient, 1.f))
	{
		result += to_latex(m_Coefficient) + '*';
	}
		
	for (auto num : m_Numbers)
	{
		if (num >= 0)
		{
			result += to_string(num);
		}
		else
		{
			result += "(" + to_string(num) + ")";
		}
		result += '*';
	}
	
	for (auto& var : m_Varribles)
	{
		result += var.AsLatexCode() + '*';
	}
	
	if (!result.empty())
		result.pop_back();		
	else
		result = "1";			

	return result;
}

Scalar& Scalar::Simplify()
{
	if (IsFZero(m_Coefficient))
	{
		Clear();
		return *this;
	}

	for (int& num : m_Numbers)
	{
		m_Coefficient *= float(num);
	}
	m_Numbers.clear();

	return *this;
}

bool Scalar::alphabetical_less(const Scalar& lhs, const Scalar& rhs)
{
	return lexicographical_compare(lhs.m_Varribles.begin(), lhs.m_Varribles.end(), rhs.m_Varribles.begin(), rhs.m_Varribles.end(), &Varrible::alphabetical_less);
}

Scalar& Scalar::AlphabeticalSort()
{
	sort(m_Varribles.begin(), m_Varribles.end(), &Varrible::alphabetical_less);
	return *this;
}

Scalar Scalar::SymbolicPart()
{
	Scalar result(*this);
	result.m_Coefficient = 1.f;
	result.m_Numbers.clear();
	return result;
}


float& Scalar::NumericPart()
{
	for (auto& num : m_Numbers)
	{
		m_Coefficient *= float(num);
	}
	m_Numbers.clear();
	return m_Coefficient;
}

void Scalar::Reduce(Scalar& numerator, Scalar& denominator)
{
//m_Coefficient;
	auto numeratorCoefficientRatio = GetRatio(numerator.m_Coefficient);
	auto denominatorCoefficientRatio = GetRatio(denominator.m_Coefficient);



///////m_Numbers
	int numeratorNum = numeratorCoefficientRatio.first * denominatorCoefficientRatio.second;

	{
		for (auto num : numerator.m_Numbers)
		{
			numeratorNum *= num;
		}

		numerator.m_Numbers.clear();
	}

	int denominatorNum = numeratorCoefficientRatio.second * denominatorCoefficientRatio.first;
	{
		for (auto num : denominator.m_Numbers)
		{
			denominatorNum *= num;
		}

		denominator.m_Numbers.clear();
	}

	ReduceRatio(numeratorNum, denominatorNum);

	if (denominatorNum < 0)
		numeratorNum *= -1;
	denominatorNum = std::abs(denominatorNum);
	
	numerator.m_Coefficient = float(numeratorNum);
	denominator.m_Coefficient = float(denominatorNum);
	
/////////////////m_Varribles


	for (auto it = denominator.m_Varribles.begin(); it != denominator.m_Varribles.end(); )
	{
		auto ret = find(numerator.m_Varribles.begin(), numerator.m_Varribles.end(), *it);

		if (ret != numerator.m_Varribles.end())
		{
			numerator.m_Varribles.erase(ret);
			it = denominator.m_Varribles.erase(it);
		}
		else
		{
			++it;
		}
	}

}

bool Scalar::equal_symolic_part(const Scalar& lhs, const Scalar& rhs)
{
	if (lhs.m_Varribles.size() != rhs.m_Varribles.size())
			return false;

	return is_permutation(lhs.m_Varribles.begin(), lhs.m_Varribles.end(), rhs.m_Varribles.begin());
}


void Scalar::GetVarribles(vector<Varrible>& varribles) const
{
	copy(m_Varribles.begin(), m_Varribles.end(), back_inserter(varribles));	
}

void Scalar::Clear()
{
	m_Numbers.clear();
	m_Varribles.clear();
}

Scalar& Scalar::operator*=(const Varrible& varrible)
{
	m_Varribles.emplace_back(varrible);
	return *this;
}

Scalar& Scalar::operator*=(int factor)
{
	m_Coefficient *= float(factor);
	return *this;
}

Scalar& Scalar::operator*=(const Scalar& rhs)
{
	m_Coefficient *= rhs.m_Coefficient;
	std::copy(rhs.m_Numbers.begin(), rhs.m_Numbers.end(), back_inserter(m_Numbers));
	//std::copy(rhs.m_Consts.begin(), rhs.m_Consts.end(), back_inserter(m_Consts));
	std::copy(rhs.m_Varribles.begin(), rhs.m_Varribles.end(), back_inserter(m_Varribles));

	return *this;

}

bool Scalar::operator==(const Scalar& rhs) const
{
	if (!FCmp(m_Coefficient, rhs.m_Coefficient))
		return false;

	//if (IsFZero(m_Coefficient))
	//	return true;

	if (m_Numbers.size() != rhs.m_Numbers.size())
		return false;

	if (!is_permutation(m_Numbers.begin(), m_Numbers.end(), rhs.m_Numbers.begin()))
		return false;

	if (m_Varribles.size() != rhs.m_Varribles.size())
		return false;
	
	if (!is_permutation(m_Varribles.begin(), m_Varribles.end(), rhs.m_Varribles.begin()))
		return false;

	return true;
}

SumOfScalar& SumOfScalar::operator*=(const SumOfScalar& rhs)
{
	SumOfScalar result;

	for (auto& lhsScalar : *this)
	{
		for (auto& rhsScalar : rhs)
		{
			result.push_back(lhsScalar*rhsScalar);
		}
	}

	//SimplifyAdditive

	*this = move(result);
	return *this;
}

SumOfScalar::SumOfScalar(const Scalar& scalar)
{
	push_back(scalar);
}

SumOfScalar& SumOfScalar::operator+=(const SumOfScalar& rhs)
{
	copy(rhs.begin(), rhs.end(), back_inserter(*this));
	return *this;
}

SumOfScalar& SumOfScalar::operator+=(const Scalar& rhs)
{
	push_back(rhs);
	return *this;
}

std::vector<Varrible> SumOfScalar::TakeOutCommonVarribles()
{
	return vector<Varrible>();
}

bool SumOfScalar::GroupBy(const vector<Varrible>& vars, multimap<Varrible, SumOfScalar>& grouped)
{	
	//map<Varrible, SumOfScalar> group;

	//SumOfScalar sumOfScalar(*this);
	//
	//for (auto& var : vars)
	//{	
	//	auto it = find_if(sumOfScalar.begin(), sumOfScalar.end(), [var](const Scalar& s)->bool
	//	{
	//		return	find(s.m_Varribles.begin(), s.m_Varribles.end(), var) != s.m_Varribles.end();
	//	});

	//	while (it != sumOfScalar.end())
	//	{
	//		sumOfScalar.erase(it);
	//		group[var].push_back(*it);

	//		it = find_if(sumOfScalar.begin(), sumOfScalar.end(), [var](const Scalar& s)->bool
	//		{
	//			return	find(s.m_Varribles.begin(), s.m_Varribles.end(), var) != s.m_Varribles.end();
	//		});
	//	}
	//}

	//if (sumOfScalar.empty())
	//	return false;

	//SumOfScalar& first = group.begin()->second;

	//if (!all_of(group.begin(), group.end(), [first](const pair<Varrible, SumOfScalar>& s)->bool
	//		{
	//			return first == s;
	//		})
	//						)
	//{
	//	return false;
	//}


	//		
			
	return true;
}


///ab + ac + ad + bd + cd  + dd

//a(b+c+d) + d(b + c + d)

//(a + d)(b + c + d)


////Другой вариант
////b(a + d) + a(c + d) + d(c + d)
///b(a+d) + (a + d)(c + d)
///(b + c + d)(a+d)

////Тупиковый вариант
//c(a+d) + d(b + a + d) + ab


////Еще вариант
//b(a + d) + c(a+d) + d(b + d)

////По d
//d(b + c + a + d) + a(b + c)


//a(b+c+d) + d


///ab + ac + ad + bd + cd  + dd

/////////ab,	ac,		ad,		bd,		cd,		dd
//ab	*		a		a		b		-		-
//ac	a		*		a		-		c		-
//ad	a		a		*		d		d		d
//bd	b		-		d		*		d		d	
//cd	-		c		d		d		*		d
//dd	-		-		d		d		d		*


/////////ab,	ac,		ad,		bd,		cd,		dd
//ab	*		a		a		b		-		-
//ac			*		a		-		c		-
//ad					*		d		d		d
//bd							*		d		d	
//cd									*		d
//dd											*



///a(b + c + d) + b(d) + c(d) + d(d)     





bool SumOfScalar::ToCompositionOfFlatSum(CompositionOfFlatSum& result, LatexContext* latexContext)
{
	for (auto& scalarSummand : *this)
	{
		scalarSummand.AlphabeticalSort();
	}

	sort(begin(), end(), &Scalar::alphabetical_less );


	string latexCode = R"(\begin{center}\begin{tabular}{)";

	latexCode += "||c|";

	for (decltype(size()) c = 0; c < size(); c++)
	{
		latexCode += "|c";
	}
	latexCode += R"(|} )";

	latexCode += "\n \\hline \n";

	latexCode += " & ";

	for (auto& summand : *this)
	{
		latexCode += summand.AsLatexCode() + " &";
	}
	latexCode.pop_back();
	latexCode += "\\\\\n";

	latexCode += R"(\hline)";

	latexCode += "\n";

	ToCompositionOfFlatSumInternal(result, latexCode);


	latexCode += R"(\hline \end{tabular}	\end{center})";

	*latexContext << latexCode << endl;

	/*set<Varrible> allVarribles;

	for (auto& scalarSummand : *this)
	{
		allVarribles.insert(scalarSummand.m_Varribles.begin(), scalarSummand.m_Varribles.end());
	}

	///Комбинаторный перебор
	for (auto var_it = allVarribles.begin(); var_it != allVarribles.end(); ++var_it)
	{
		vector<Varrible> groupVars;
		multimap<Varrible, SumOfScalar> grouped;

		groupVars.push_back(*var_it);

		if (GroupBy(groupVars, grouped))
		{

		}
	}

	CommonFactors commonFactors;





	using VarVector = decltype(front()->m_Varribles);
	using Indexes = pair<int, int>;


	unordered_map< Indexes, VarVector>  index2intersection;

	VarVector intersection;
	intersection.reserve(10);



	VarVector universalFactor;
	universalFactor.reserve(10);

	VarVector temporaryUniversalFactor;
	temporaryUniversalFactor.reserve(10);

	bool testUniversalFactor = true;


	auto& scalarSummand = this->front();

	for (int c_o_summand = 1, size_o_summand = size(); c_o_summand < size_o_summand; c_o_summand++)
	{
		auto& otherScalarSummand = (*this)[c_o_summand];

		intersection.clear();

		set_intersection(scalarSummand->m_Varribles.begin(), scalarSummand->m_Varribles.end(),
			otherScalarSummand->m_Varribles.begin(), otherScalarSummand->m_Varribles.end(),
			back_inserter(intersection));

		if (intersection.empty())
			continue;

		index2intersection[make_pair(c_summand, c_o_summand)] = intersection;

		if (testUniversalFactor)
		{

			temporaryUniversalFactor.clear();

			set_intersection(intersection.begin(), intersection.end(),
				universalFactor.begin(), universalFactor.end(),
				back_inserter(temporaryUniversalFactor));

			universalFactor = move(temporaryUniversalFactor);

			if (universalFactor.empty())
				testUniversalFactor = false;
		}
	}

	if (!commonFactors.HasAtLeastOneCommonFactor(c_summand))
	{
		return false;
	}

	if (!universalFactor.empty())
	{
		Scalar s = universalFactor;

		SumOfScalar firstMultiplier = s;

		SumOfScalar secondMultiplier = *this / s;

		secondMultiplier.


		return true;
	}

}*/








return true;
}

bool SumOfScalar::ToCompositionOfFlatSumInternal(CompositionOfFlatSum& result, string& latexCode) const
{
	

	/*set<Varrible> allVarribles;

	for (auto& scalarSummand : *this)
	{
		allVarribles.insert(scalarSummand.m_Varribles.begin(), scalarSummand.m_Varribles.end());
	}

	///Комбинаторный перебор
	for (auto var_it = allVarribles.begin(); var_it != allVarribles.end(); ++var_it)
	{
		vector<Varrible> groupVars;
		multimap<Varrible, SumOfScalar> grouped;

		groupVars.push_back(*var_it);

		if (GroupBy(groupVars, grouped))
		{

		}
	}

	CommonFactors commonFactors;

	*/


	using VarVector = decltype(front().m_Varribles);
	

	

	VarVector intersection;
	intersection.reserve(10);


	for (int c_summand = 0, size_summand = size(); c_summand < size_summand; c_summand++)
	{
		auto& scalarSummand = (*this)[c_summand];

		latexCode += scalarSummand.AsLatexCode() + " &";
				
		for (int c_element = 0, size_element = c_summand; c_element < size_element; c_element++)
		{
			latexCode += " &";
		}

		latexCode += scalarSummand.AsLatexCode() + " &";
		


		for (int c_o_summand = c_summand + 1, size_o_summand = size(); c_o_summand < size_o_summand; c_o_summand++)
		{
			auto& otherScalarSummand = (*this)[c_o_summand];

			intersection.clear();

			set_intersection(scalarSummand.m_Varribles.begin(), scalarSummand.m_Varribles.end(),
				otherScalarSummand.m_Varribles.begin(), otherScalarSummand.m_Varribles.end(),
				back_inserter(intersection), &Varrible::alphabetical_less );
						

			if (intersection.empty())
			{
				latexCode += "-";
			}
			else
			{
				for (auto& var : intersection)
				{
					latexCode += var.AsLatexCode();
				}
			}

			latexCode += " &";						
		}

		
		latexCode.pop_back();
		latexCode += "\\\\\n";
	}

		

	return true;

}

void FlatRatio::SimplifyFlatMultiplicative()
{
	//CompositionOfFlatSum numerator;
	// if (!Numerator.ToCompositionOfFlatSum(numerator))//Учитывая кратность
	//	 return;

	// CompositionOfFlatSum denominator;
	// if (!Denominator.ToCompositionOfFlatSum(denominator))
	//	 return;
	//
	// sort(numerator.begin(), numerator.end(), &SumOfScalar::alphabetical_less);
	// sort(denominator.begin(), denominator.end(), &SumOfScalar::alphabetical_less);
	// 
	// CompositionOfFlatSum intersection(numerator.size() + denominator.size());

	// auto it = std::set_intersection(numerator.begin(), numerator.end(), denominator.begin(), denominator.end(), intersection.begin());	 
	// intersection.resize(it - intersection.begin());

	// CompositionOfFlatSum reducedNumerator;
	// reducedNumerator.reserve(numerator.size());
	// std::set_difference(numerator.begin(), numerator.end(), intersection.begin(), intersection.end(), back_inserter(reducedNumerator));

	// CompositionOfFlatSum reducedDenominator;
	// reducedDenominator.reserve(denominator.size());	 
	// std::set_difference(denominator.begin(), denominator.end(), intersection.begin(), intersection.end(), back_inserter(reducedDenominator));

	// //раскрытите скобок
	// Numerator = ToSumOfFlat( numerator );
	// Denominator = ToSumOfFlat( denominator );
}

SumOfScalar ToSumOfFlat(const CompositionOfFlatSum& composition)
{
	SumOfScalar result = Scalar(1);

	for (auto& mul : composition)
	{
		result *= mul;
	}

	return result;
}

bool CommonFactors::HasAtLeastOneCommonFactor(int summandIndex)
{
	throw std::logic_error("The method or operation is not implemented.");
}
