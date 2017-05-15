#pragma once

#include "BaseMatrix.h"
#include "..\Symbolic\Sum.h"
#include "BlockMatrix.h"


class RealMatrix;

class SymbolicMatrix : public BaseMatrix<Sum, SymbolicMatrix>
{	
public:	
	using BaseMatrix::BaseMatrix;
	SymbolicMatrix() : BaseMatrix()	{	}
	explicit SymbolicMatrix(uint32_t vertical, uint32_t horizontal, const string& elementNames);
	//static SymbolicMatrix CreateNamed(uint32_t vertical, uint32_t horizontal, const string& elementNames);
		
	SymbolicMatrix(const RealMatrix& rhs);
	

	
	virtual ~SymbolicMatrix()
	{
	}

	virtual vector<Sum> Eigenvalues() const;
	virtual vector<Sum> SingularValues() const;
	virtual Sum NormSpectral() const;	


	////ElementWise functions
	virtual SymbolicMatrix& Simplify();

	virtual SymbolicMatrix& ToCanonicalViewElementWise();

	template <class Iterator>
	SymbolicMatrix& Substitute(Iterator first, Iterator last, const Sum& value);

	SymbolicMatrix& Substitute(const Varrible& var, const Sum& value);
};




using BlockSymbolicMatrix = BlockMatrix<SymbolicMatrix>;



//
//
//class MatrixSymbolComposition;
//
//class SymbolicSum : public vector< MatrixSymbolComposition >, boost::operators<SymbolicSum>
//{
//public:
//	SymbolicSum()
//	{
//	}
//	SymbolicSum(const char expression) = delete;
//	SymbolicSum(int value);
//	SymbolicSum(double value);
//	SymbolicSum(const string& expression);
//	SymbolicSum(const char* expression);
//
//	string m_Name;
//
//	bool operator==(const SymbolicSum& rhs) const;
//
//	SymbolicSum& operator+=(const SymbolicSum& rhs);
//	SymbolicSum& operator-=(const SymbolicSum& rhs);
//
//	SymbolicSum& operator*=(const SymbolicSum& rhs);
//	SymbolicSum& operator/=(const SymbolicSum& rhs);
//
//	SymbolicSum operator-() const;
//
//
//	bool IsSymbolic() const;
//	vector<string> GetSymbols() const;
//
//	SymbolicSum& Substitute(string name, const SymbolicSum& value);
//	SymbolicSum& Approximate();
//	string AsString() const;
//	SymbolicSum& Simplify();
//private:
//
//};
//
//
//
//class MatrixSymbol : public SymbolicMatrix
//{
//public:
//	string m_Name;
//};
//
//class MatrixFunc
//{
//public:	
//	virtual operator SymbolicSum() const = 0;	
//};
//
//class Det
//{
//public:	
//	SymbolicSum m_Arg;
//
//	Det(const SymbolicSum& arg): m_Arg(arg) 
//	{
//
//	}
//	virtual operator SymbolicSum() const override
//	{
//		return SymbolicSum();
//	}
//};
//
//
//
//class MatrixSymbolComposition : boost::operators<MatrixSymbolComposition>
//{
//public:
//	vector<MatrixSymbol> m_Matricies;	
//
//	vector<MatrixFunc> m_ExplicitFuncs;
//
//	vector< SymbolicSum > m_Multipliers;
//	vector< SymbolicSum > m_Dividers;
//
//	MatrixSymbolComposition()
//	{
//	}
//	MatrixSymbolComposition(int value);
//
//	bool operator==(MatrixSymbolComposition rhs) const;
//
//	MatrixSymbolComposition& operator*=(const MatrixSymbolComposition& rhs);
//	MatrixSymbolComposition& operator/=(const MatrixSymbolComposition& rhs);
//
//	MatrixSymbolComposition& operator*=(int factor);
//
//	bool IsSymbolic() const;
//	MatrixSymbolComposition& Substitute(string name, const Symbolic& value);
//	MatrixSymbolComposition& Approximate();
//	string AsString() const;
//	MatrixSymbolComposition& Simplify();
//
//private:
//};



#include "SymbolicMatrix.inl"