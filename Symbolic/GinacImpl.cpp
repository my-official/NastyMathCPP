#include "stdafx.h"
#include "Sum.h"


Sum Sum::FromGinacEx(const GiNaC::ex& expression)
{
	using namespace GiNaC;
	Sum result;

	if (is_a<add>(expression))
	{
		for (auto& e : expression)
		{
			result += Sum::FromGinacEx(e);
		}
		return result;
	}
	else
	{
		result.push_back(Composition::FromGinacEx(expression));
		return result;
	}
}

GiNaC::ex Sum::AsGinacEx() const
{
	using namespace GiNaC;

	ex result = 0;

	for (auto& summand : *this)
	{
		result += summand.AsGinacEx();
	}

	return result;
}

bool Sum::ToCompositionOfSums_Factor(vector<Sum>& newMultipliers)
{
	//предполагаетс€, что у слагаемых нет делителей
	using namespace GiNaC;
	ex expression = AsGinacEx();

	//cout << "AsGinacTx:  " << ex("a*c + a*d + b*c + b*d", lst({ symbol("a"), symbol("b"), symbol("c"), symbol("d") })) << endl;

	/*cout << "AsGinacEx:  " << expression << endl;
	expression = ex("b*c + b*d + a*c + a*d", lst({ symbol("a"), symbol("b"), symbol("c"), symbol("d"), symbol("a") }).unique() );*/

	expression = factor(expression);

	//cout << "Gactorize:  " << expression << endl;

	if (is_a<mul>(expression))
	{
		Composition composition = Composition::FromGinacEx(expression);

		assert(composition.m_Scalar == 1);
		assert(composition.m_Dividers.empty());

		move(composition.m_Multipliers.begin(), composition.m_Multipliers.end(), back_inserter(newMultipliers));
		return true;
	}
	else
	{
		return false;
	}



	//if (size() < 2)
	//	return;

	//for (auto& s : *this)
	//{
	//	if (!s.m_Multipliers.empty())
	//	
	//}


	//if (all_of(begin(), end(), [](Composition& summand)->bool
	//		{
	//			return summand.m_Multipliers.empty();
	//		})											
	//				)
	//{
	//	SumOfScalar sum;
	//	for (auto& summand : *this)
	//		sum.push_back(summand.m_Scalar);

	//	int numSummand = size();



	//	set<Varrible> vars;
	//	for (auto& scalar : sum)
	//	{
	//		for (auto& var : scalar.m_Varribles)
	//		{
	//			vars.insert(var);
	//		}
	//	}

	//	//a*c*e +a*c*f + a*d*e + a*d*f + b*c*e +b*c*f + b*d*e + b*d*f
	//	//a        c*e + c*f + d*e + d*f   
	//	//b        c*e + c*f + d*e + d*f
	//	

	//	
	//	//(a + b)*(c + d)*(e + f) 
	//	
	//	//TODO combinatorial
	//	
	//	multimap<Varrible, SumOfScalar> grouped;


	//	for (int c_var = 1, size_vars = vars.size() / 2; c_var < size_vars; c_var++)
	//	{
	//		vector<Varrible> varComposition;

	//		auto begin_it = vars.begin();
	//		auto end_it = begin_it;
	//		advance(end_it, 2);

	//		copy(begin_it, end_it, back_inserter(varComposition));

	//		if (sum.GroupBy(varComposition, grouped))
	//		{
	//			

	//			break;
	//		}

	//							

	//	}








	//}
	//else
	//{

	//}
}


Composition Composition::FromGinacEx(const GiNaC::ex& expression)
{
	using namespace GiNaC;
	Composition result;

	if (is_a<mul>(expression))
	{
		for (auto& e : expression)
		{
			result *= Composition::FromGinacEx(e);
		}
		return result;
	}

	if (is_a<add>(expression))
	{
		result.m_Multipliers.emplace_back(Sum::FromGinacEx(expression));
		return result;
	}

	if (is_a<symbol>(expression))
	{
		result.m_Scalar.m_Varribles.push_back(ex_to<symbol>(expression).get_name());
		return result;
	}

	if (is_a<numeric>(expression))
	{
		const numeric& n = ex_to<numeric>(expression);

		if (n.is_integer())
			result.m_Scalar.m_Numbers.push_back(n.to_int());
		else
			result.m_Scalar.m_Coefficient *= (float)n.to_double();

		return result;
	}

	assert(0);
	return result;
}

GiNaC::ex Composition::AsGinacEx() const
{
	GiNaC::ex result = m_Scalar.AsGinacEx();

	for (auto& mul : m_Multipliers)
	{
		result *= mul.AsGinacEx();
	}

	for (auto& dev : m_Dividers)
	{
		result /= dev.AsGinacEx();
	}
	return result;
}



GiNaC::ex Scalar::AsGinacEx() const
{
	using namespace GiNaC;
	lst symbols;

	for (auto& var : m_Varribles)
	{
		symbols.append(symbol(var.m_Name));
	}
	symbols.unique();

	GiNaC::ex result(AsString(), symbols);
	return result;
}
