#include "stdafx.h"
#include "MatrixSymbol.h"


string tolower(const string& s)
{
	string result(s);
	for (auto& c : result)
	{
		c = tolower(c, locale::classic());
	}
	return result;
}


MatrixSymbol::MatrixSymbol(uint32_t vertical, uint32_t horizontal, string name) : SymbolicMatrix(vertical, horizontal, tolower(name) ), m_Name(name)
{

}

MatrixSymbol::MatrixSymbol(initializer_list<Sum> il) : SymbolicMatrix(il)
{

}

MatrixSymbol::MatrixSymbol(const string& name) : m_Name(name)
{

}

//MatrixSymbol::MatrixSymbol(const string& name, initializer_list<MatrixSymbol> il) : m_Name(name)
//{
//
//}

MatrixSymbol::~MatrixSymbol()
{
}

MatrixExpression MatrixSymbol::operator+(const MatrixSymbol& rhs)  const
{
	MatrixExpression result(*this);
	result += rhs;
	return result;
}

MatrixExpression MatrixSymbol::operator*(const MatrixSymbol& rhs)  const
{
	MatrixExpression result(*this);
	result *= rhs;
	return result;
}

std::string MatrixSymbol::AsLatexCode() const
{
	return m_Name;
}

MatrixSymbol MatrixSymbol::CreateAppropriateZeroMatrix()
{
	MatrixSymbol result("O");
	return result;
}

bool MatrixExpression::operator==(const MatrixExpression& rhs) const
{
	MatrixExpression a(*this);
	MatrixExpression b(rhs);

	a.ToCanonicalView();
	b.ToCanonicalView();

	if (a.size() != b.size())
		return false;

	return is_permutation(a.begin(), a.end(), b.begin());
}

MatrixExpression& MatrixExpression::operator+=(const MatrixExpression& rhs)
{
	copy(rhs.begin(), rhs.end(), back_inserter(*this));
	return *this;
}

MatrixExpression& MatrixExpression::operator*=(const MatrixExpression& rhs)
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


MatrixExpression& MatrixExpression::operator*=(const Sum& scalar)
{
	for (auto& summand : *this)
	{
		summand *= scalar;
	}
	return *this;
}

MatrixExpression::MatrixExpression(const MatrixSymbol& ms)
{	
	emplace_back(ms);
}

MatrixExpression& MatrixExpression::operator-=(const MatrixExpression& rhs)
{
	int preSize = size();
	copy(rhs.begin(), rhs.end(), back_inserter(*this));

	for (int c = preSize, newSize = size(); c < newSize; c++)
	{
		this->operator[](c) *= -1;
	}
	return *this;
}

MatrixExpression MatrixExpression::operator-() const
{
	MatrixExpression result(*this);
	result *= -1;
	return result;
}

MatrixExpression& MatrixExpression::ToCanonicalView()
{
	SimplifyAdditive();
	return *this;
}

std::string MatrixExpression::AsLatexCode() const
{
	if (this->empty())
		return "O";

	string result;

	if (size() > 1)
	{
		result += "(";
	}
	
	for (const auto& summand : *this)
	{
		result += summand.AsLatexCode() + " + ";
	}

	result.pop_back();
	result.pop_back();
	result.pop_back();

	if (size() > 1)
	{
		result += ")";
	}
		
	return result;
}

MatrixExpression& MatrixExpression::SimplifyAdditive()
{
	vector< CompositionOfSum > flatSummands;
	for (auto& summand : *this)
	{
		summand.SimplifyMultiplicative();
		summand.ToFlatSummands(flatSummands);		
	}

	SimplifyAdditiveInternal(flatSummands);

	static_cast< vector< CompositionOfSum >& >( *this ) = std::move(flatSummands);
	return *this;
}

void MatrixExpression::SimplifyAdditiveInternal(vector< CompositionOfSum >& flatSummands)
{
	for (auto it = flatSummands.begin(); it != flatSummands.end(); )
	{		
		CompositionOfSum& a = *it;
		if (a.m_Coefficient == 0)
		{
			it = flatSummands.erase(it);
		}
		else
		{
			++it;
		}
	}

	if (flatSummands.size() < 2)
		return;

	for (auto it = flatSummands.begin(); it != flatSummands.end(); )
	{
		CompositionOfSum& a = *it;
		
		auto ret = find_if(it + 1, flatSummands.end(), [&a](const CompositionOfSum& rhs) {  return CompositionOfSum::equal_matrix_parts(a, rhs); });
		if (ret != flatSummands.end())
		{			
			CompositionOfSum& b = *ret;

			b.m_Coefficient += a.m_Coefficient;

			if (b.m_Coefficient == 0)
			{
				flatSummands.erase(ret);
			}

			it = flatSummands.erase(it);
		}
		else
		{
			++it;
		}
	}
}

bool CompositionOfSum::operator==(const CompositionOfSum& rhs) const
{
	CompositionOfSum a(*this);
	CompositionOfSum b(rhs);

	a.ToCanonicalView();
	b.ToCanonicalView();

	if (a.m_Coefficient != b.m_Coefficient)
		return false;

	if (a.m_FlatMultipliers.size() != b.m_FlatMultipliers.size())
		return false;

	if (!equal(a.m_FlatMultipliers.begin(), a.m_FlatMultipliers.end(), b.m_FlatMultipliers.begin()))
		return false;

	if (a.m_Multipliers.size() != b.m_Multipliers.size())
		return false;

	if (!equal(a.m_Multipliers.begin(), a.m_Multipliers.end(), b.m_Multipliers.begin()))
		return false;

	return true;
}



CompositionOfSum& CompositionOfSum::operator*=(const CompositionOfSum& rhs)
{
	MatrixExpression mul;
	mul.push_back(rhs);	
	return *this;
}



CompositionOfSum& CompositionOfSum::operator*=(const MatrixExpression& rhs)
{
	m_Multipliers.push_back(rhs);
	return *this;
}


CompositionOfSum& CompositionOfSum::operator*=(const Sum& scalar)
{
	m_Coefficient *= scalar;
	return *this;
}

CompositionOfSum::CompositionOfSum(const MatrixSymbol& ms)
{
	m_FlatMultipliers.emplace_back(ms);
}

CompositionOfSum& CompositionOfSum::operator*=(const MatrixSymbol& rhs)
{
	if (m_Multipliers.empty())
		m_FlatMultipliers.emplace_back(rhs);
	else
		m_Multipliers.emplace_back(rhs);
	return *this;
}

CompositionOfSum CompositionOfSum::operator-() const
{
	CompositionOfSum result;
	result *= -1;
	return result;
}

bool CompositionOfSum::HasFlatPartOnly()
{
	return m_Multipliers.empty();
}

CompositionOfSum& CompositionOfSum::ToCanonicalView()
{
	return *this;
}

std::string CompositionOfSum::AsLatexCode() const
{
	string result;

	if (!m_Coefficient.empty())
	{
		result += m_Coefficient.AsLatexCode();
	}
	
	for (auto& sym : m_FlatMultipliers)
	{
		result += sym.AsLatexCode() + " ";
	}

	for (auto& sym : m_Multipliers)
	{
		if (sym.size() == 1)
			result += sym.AsLatexCode();
		else
			result += "( " + sym.AsLatexCode() + " )";
	}
	

	if (result.empty())
		result += "1";
	
	return result;
}

void CompositionOfSum::ToFlatSummands(vector<CompositionOfSum>& flatSummands)
{

}

CompositionOfSum& CompositionOfSum::SimplifyMultiplicative()
{
	return *this;
}

bool CompositionOfSum::equal_matrix_parts(const CompositionOfSum& lhs, const CompositionOfSum& rhs)
{
	return true;
}
