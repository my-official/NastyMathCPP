#include "stdafx.h"
#include "Composition.h"
#include "..\Utils.h"
#include "Sum.h"




Composition::Composition(int value) : m_Scalar(value)
{
	
}

bool Composition::operator==(const Composition& rhs) const
{
	Composition a(*this);
	Composition b(rhs);

	a.ToCanonicalView();
	b.ToCanonicalView();

	if (a.m_Scalar != b.m_Scalar)
		return false;

	if (a.m_Multipliers.size() != b.m_Multipliers.size())
		return false;

	if (!is_permutation(a.m_Multipliers.begin(), a.m_Multipliers.end(), b.m_Multipliers.begin()))
		return false;


	if (a.m_Dividers.size() != b.m_Dividers.size())
		return false;

	if (!is_permutation(a.m_Dividers.begin(), a.m_Dividers.end(), b.m_Dividers.begin()))
		return false;

	return true;
}

Composition& Composition::operator*=(const Composition& rhs)
{
	m_Scalar *= rhs.m_Scalar;	
	std::copy(rhs.m_Multipliers.begin(), rhs.m_Multipliers.end(), back_inserter(m_Multipliers));
	std::copy(rhs.m_Dividers.begin(), rhs.m_Dividers.end(), back_inserter(m_Dividers));
	return *this;
}

Composition& Composition::operator*=(int factor)
{
	m_Scalar *= factor;	
	return *this;
}

Composition& Composition::operator/=(const Composition& rhs)
{
	Sum divider;
	divider.push_back(rhs);
	m_Dividers.emplace_back(std::move(divider));
	return *this;
}

Composition& Composition::operator*=(const Sum& rhs)
{
	m_Multipliers.push_back(rhs);
	return *this;
}

Composition& Composition::operator/=(const Sum& rhs)
{
	m_Dividers.push_back(rhs);
	return *this;
}

bool Composition::IsSymbolic() const
{
	/*if (!m_Varribles.empty())
		return true;*/

	return ForEachMultipliersAndDividersWhileFalse([](const Sum& sym)->bool
	{
		return sym.IsSymbolic();
	});
}

Composition& Composition::Substitute(string name, const Sum& value)
{
	if (value.empty())
		throw EXCEPTION;

	ForEachMultipliersAndDividers([&name, &value](Sum& sym)
	{
		sym.Substitute(name, value);
	});

	m_Scalar.Substitute(name, value, *this);

	return *this;
}

Composition& Composition::Substitute(const Varrible& varrible, const Sum& value)
{
	ForEachMultipliersAndDividers([&varrible, &value](Sum& sym)
	{
		sym.Substitute(varrible, value);
	});

	m_Scalar.Substitute(varrible, value, *this);

	return *this;
}


void Composition::GetVarribles(vector<Varrible>& varribles) const
{
	ForEachMultipliersAndDividers([&varribles](const Sum& sym)
	{
		sym.GetVarribles(varribles);
	});

	m_Scalar.GetVarribles(varribles);
}


Composition& Composition::Approximate()
{
	/*int64_t numerator = 1;

	for (auto num : m_Numbers)
	{
		numerator *= num;
	}

	m_Numbers.clear();

	for (auto num : m_Numbers)
	{
		numerator *= num;
	}

	m_Coefficient *= float(numerator);

	ForEachMultipliersAndDividers([](Sum& sym)
	{
		sym.Approximate();
	});*/

	return *this;
}

std::string Composition::AsString() const
{
	std::string result;

	result += m_Scalar.AsString();
		
	if (!result.empty())
	{
		result += '*';
	}


	for (auto& sym : m_Multipliers)
	{
		if (sym.size() == 1)
			result += sym.AsString() + '*';
		else
			result += "( " + sym.AsString() + " )*";
	}

	result.pop_back();


	for (auto& sym : m_Dividers)
	{
		if (sym.size() == 1)
			result += "/" + sym.AsString();
		else
			result += "/( " + sym.AsString() + " )";
	}
	
	return result;
}


std::string Composition::AsLatexCode() const
{
	std::string numerator;

	numerator += m_Scalar.AsLatexCode();
		

	if (!numerator.empty())
	{
		numerator += '*';
	}


	for (auto& sym : m_Multipliers)
	{
		if (sym.size() == 1)
			numerator += sym.AsLatexCode() + '*';
		else
			numerator += "( " + sym.AsLatexCode() + " )*";
	}

	numerator.pop_back();

	if (numerator.empty())
		numerator += "1";

	
	std::string result;


	bool frac = !m_Dividers.empty();

	if (frac)
		result += "\\frac{";

	result += numerator;

	if (frac)
		result += "}{";	

	for (auto& sym : m_Dividers)
	{
		if (sym.size() == 1)
			result += sym.AsLatexCode();
		else
			result += "( " + sym.AsLatexCode() + " )";
	}

	if (frac)
		result += "}";

	return result;
}

Composition& Composition::AlphabeticalSort()
{
	m_Scalar.AlphabeticalSort();

	if (m_Dividers.size() == 1)
		m_Dividers.front().AlphabeticalSort();

	return *this;
}

bool Composition::alphabetical_less(const Composition& lhs, const Composition& rhs)
{
	if ( decltype(m_Scalar)::alphabetical_less(lhs.m_Scalar, rhs.m_Scalar))
		return 	true;
	else
	{		
		if (decltype(m_Scalar)::alphabetical_less(rhs.m_Scalar, lhs.m_Scalar))
			return false;
		else
			return lexicographical_compare(lhs.m_Dividers.begin(), lhs.m_Dividers.end(), rhs.m_Dividers.begin(), rhs.m_Dividers.end(), &Sum::alphabetical_less);
	}
	/*auto lhs_begin_it = lhs.m_Varribles.begin();
	auto rhs_begin_it = rhs.m_Varribles.begin();

	auto lhs_end_it = lhs.m_Varribles.end();
	auto rhs_end_it = rhs.m_Varribles.end();

	auto lhs_it = lhs_begin_it;
	auto rhs_it = rhs_begin_it;

	for (; lhs_it != lhs_end_it && rhs_it != rhs_end_it;  ++lhs_it, ++rhs_it)
	{
	const Varrible& lhsVar = *lhs_it;
	const Varrible& rhsVar = *rhs_it;

	if ( !Varrible::alphabetical_equal(lhsVar, rhsVar) )
	return Varrible::alphabetical_less(lhsVar, rhsVar);
	}

	if (lhs_it != lhs_end_it)
	{
	return false;
	}
	else
	{
	return rhs_it != rhs_end_it;
	}	*/
}



Composition& Composition::ToCanonicalView()
{
	Simplify();
	return *this;
}

Composition& Composition::ReduceSimilarMulsAndDivs()
{
	sort(m_Multipliers.begin(), m_Multipliers.end(), &Sum::alphabetical_less);
	sort(m_Dividers.begin(), m_Dividers.end(), &Sum::alphabetical_less);

	vector<Sum> intersection;
	intersection.reserve(std::min(m_Multipliers.size(), m_Dividers.size()));

	set_intersection(m_Multipliers.begin(), m_Multipliers.end(), m_Dividers.begin(), m_Dividers.end(), back_inserter(intersection), &Sum::alphabetical_less);

	if (!intersection.empty())
	{
		vector<Sum> multipliers;
		set_difference(m_Multipliers.begin(), m_Multipliers.end(), intersection.begin(), intersection.end(), back_inserter(multipliers), &Sum::alphabetical_less);
		m_Multipliers = std::move(multipliers);

		vector<Sum> dividers;
		set_difference(m_Dividers.begin(), m_Dividers.end(), intersection.begin(), intersection.end(), back_inserter(dividers), &Sum::alphabetical_less);
		m_Dividers = std::move(dividers);
	}
	return *this;
}

Composition& Composition::ReduceScalarAndDiv()
{
	if (m_Dividers.size() == 1)
	{
		Sum& div = m_Dividers.front();
		div.Simplify();

		switch (div.size())
		{
			case 0: m_Dividers.clear(); break;
			case 1:
			{
				Composition& singleDivider = div.front();
				if (singleDivider.m_Dividers.empty())
				{
					Scalar::Reduce(m_Scalar, singleDivider.m_Scalar);
					if (singleDivider.m_Scalar == 1)
						m_Dividers.clear();
				}
			}
			break;
			default: break;
		}
	}

	return *this;
}

Composition& Composition::DecomposeCyclicFractions()
{
	if (m_Dividers.empty())
		return *this;

	vector<Sum> muls;	

	ForEachMultipliersAndDividers([&](Sum& s)
	{
		for (int c_comp = 0, size_comp = s.size(); c_comp < size_comp; c_comp++)
		{
			auto& comp = s[c_comp];
			if (!comp.m_Dividers.empty())
			{
				vector<Sum>& subDividers = comp.m_Dividers;

				comp.DecomposeCyclicFractions();

				for (int c_compOther = 0, size_compOther = size_comp; c_compOther < size_compOther; c_compOther++)
				{
					if (c_compOther == c_comp)
						continue;

					auto& compOther = s[c_compOther];
					
					copy(subDividers.begin(), subDividers.end(), back_inserter(compOther.m_Multipliers));
				}
				
				move(subDividers.begin(), subDividers.end(), back_inserter(muls));
				subDividers.clear();
			}
		}
	});

	copy(muls.begin(), muls.end(), back_inserter(m_Multipliers));
	move(muls.begin(), muls.end(), back_inserter(m_Dividers));
	
	return *this;
}



bool Composition::IsScalarOnly() const
{
	return m_Multipliers.empty() && m_Dividers.empty();
}

Composition& Composition::SimplifyMultiplicative()
{
	m_Scalar.Simplify();

	if (m_Scalar == 0)
	{
		m_Multipliers.clear();
		m_Dividers.clear();
		return *this;
	}

	for (auto& div : m_Dividers)
	{
		if (div.size() != 1)
			throw EXCEPTION;

		Composition& composition = div.front();
		if (!composition.IsScalarOnly())
		{
			throw EXCEPTION;
		}

		composition.m_Scalar.Simplify();

		m_Scalar.m_Coefficient /= composition.m_Scalar.m_Coefficient;
		
	}
	m_Dividers.clear();


	for (auto it = m_Multipliers.begin(); it != m_Multipliers.end(); )
	{
		Sum& mul = *it;

		if (mul.empty())
		{
			/*m_Scalar = 0;
			m_Multipliers.clear();
			m_Dividers.clear();*/
			*this = 0;
			return *this;
		}


		if (mul.size() != 1)
		{
			++it;
			continue;
		}

		Composition& composition = mul.front();
		if (!composition.IsScalarOnly())
		{
			++it;
			continue;
		}

		m_Scalar *= composition.m_Scalar;
		it = m_Multipliers.erase(it);
	}

	

	//Composition composition;
	//composition.m_Scalar = m_Scalar;

	//Sum scalar;
	//scalar.push_back(composition);

	//m_Scalar = 1;



	//ToFlatCompositionOfSums_Factor();

	//ReduceSimilarMulsAndDivs();	

	return *this;
}



Composition& Composition::Simplify()
{
	ForEachMultipliersAndDividers([](Sum& s)
	{
		s.Simplify();
	});

//	DecomposeCyclicFractions();	
	SimplifyMultiplicative();
	return *this;
}


void Composition::ToFlatCompositionOfSums_Factor()
{
	vector<Sum> newMuls;
	for (auto& mul : m_Multipliers)
	{
		mul.ToCompositionOfSums_Factor(newMuls);		
	}

	move(newMuls.begin(), newMuls.end(), back_inserter(m_Multipliers));	
		
	vector<Sum> newDivs;
	for (auto& div : m_Dividers)
	{
		div.ToCompositionOfSums_Factor(newDivs);
	}

	move(newDivs.begin(), newDivs.end(), back_inserter(m_Dividers));
}


void Composition::ToFlatSumOfCompositions(Sum& sum)
{
	Composition firstMulOfThis(*this);

	firstMulOfThis.m_Multipliers.clear();
	firstMulOfThis.m_Dividers.clear();


	Sum r;
	r.emplace_back(firstMulOfThis);


	for (auto& mul : m_Multipliers)
	{
		mul.ToFlatSumOfCompositions_Expand();
		r *= mul;
	};


	if (!m_Dividers.empty())
	{
		Sum divider = 1;
		for (auto& div : m_Dividers)
		{
			div.ToFlatSumOfCompositions_Expand();
			divider *= div;
		};

		divider.Simplify();
		r /= divider;
	}

	copy(r.begin(), r.end(), back_inserter(sum));
}
