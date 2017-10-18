#include "stdafx.h"
#include "SymbolicMatrix.h"
#include "..\Symbolic\Scalar.h"
#include "RealMatrix.h"


SymbolicMatrix::SymbolicMatrix(uint32_t vertical, uint32_t horizontal, const string& elementNames) : BaseMatrix(vertical, horizontal)
{
	for (uint32_t y = 0; y < m_VerticalDimensional; ++y)
	{
		for (uint32_t x = 0; x < m_HorizontalDimensional; ++x)
		{
			Sum var = Varrible(elementNames + "_{" + to_string(y + 1) + "" + to_string(x + 1) + "}");
			//Sum var = elementNames + "_" + to_string(y) + "_" + to_string(x);
			(*this)[{y, x}] = var;
		}
	}
}



SymbolicMatrix::SymbolicMatrix(const RealMatrix& rhs) : BaseMatrix(rhs.m_VerticalDimensional, rhs.m_HorizontalDimensional)
{	
	copy(rhs.m_Data.begin(), rhs.m_Data.end(), m_Data.begin());
}

std::vector<Sum> SymbolicMatrix::Eigenvalues() const
{
	return std::vector<Sum>();
}

std::vector<Sum> SymbolicMatrix::SingularValues() const
{
	return std::vector<Sum>();
}

Sum SymbolicMatrix::NormSpectral() const
{
	return 0;
}
//
//SymbolicMatrix SymbolicMatrix::Inverse() const
//{
//	SymbolicMatrix M = *this;
//	M.Simplify();
//
//	BaseMatrix<Sum, SymbolicMatrix> result = M;
//		 
//	return result.Inverse();
//}

SymbolicMatrix& SymbolicMatrix::Simplify()
{
	for (auto& elem : m_Data)
	{
		elem.Simplify();
	}
	return *this;
}

SymbolicMatrix& SymbolicMatrix::ToCanonicalViewElementWise()
{
	for (auto& elem : m_Data)
	{
		elem.ToCanonicalView();
	}
	return *this;
}

SymbolicMatrix& SymbolicMatrix::Substitute(const Varrible& var, const Sum& value)
{
	for (auto& elem : m_Data)
	{
		elem.Substitute(var, value);
	}
	return *this;
}
