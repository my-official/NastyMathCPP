#pragma once

#include "..\Matrix\SymbolicMatrix.h"

class MatrixExpression;


//#define MatrixSymbolDef(a) MatrixSymbol a(#a)

class MatrixSymbol : public SymbolicMatrix
{
public:	
	MatrixSymbol(const string& name);
	MatrixSymbol(initializer_list<Sum> il);
	//MatrixSymbol(const string& name, initializer_list<MatrixSymbol> il);
	MatrixSymbol(uint32_t vertical, uint32_t horizontal, string name);	
	virtual ~MatrixSymbol();

	static MatrixSymbol CreateAppropriateZeroMatrix();



	string m_Name;

	MatrixExpression operator+(const MatrixSymbol& rhs) const;
	MatrixExpression operator*(const MatrixSymbol& rhs) const;

	virtual string AsLatexCode() const override;

	
};

class CompositionOfSum;

class MatrixExpression	: public vector< CompositionOfSum >, boost::operators<MatrixExpression>
{
public:	 
	MatrixExpression()	{	}
	MatrixExpression(const MatrixSymbol& ms);

	bool operator==(const MatrixExpression& rhs) const;

	MatrixExpression& operator+=(const MatrixExpression& rhs);
	MatrixExpression& operator-=(const MatrixExpression& rhs);
	MatrixExpression& operator*=(const MatrixExpression& rhs);
	MatrixExpression& operator*=(const Sum&  scalar);
	MatrixExpression operator-() const;
	
	MatrixExpression& ToCanonicalView();

	string AsLatexCode() const;

	MatrixExpression& SimplifyAdditive();
private:
	void SimplifyAdditiveInternal(vector< CompositionOfSum >& flatSummands);
};


class CompositionOfSum : public boost::operators<MatrixExpression>
{
public:	
	CompositionOfSum() {}
	CompositionOfSum(const MatrixSymbol& ms);

	Sum					 m_Coefficient;
	vector<MatrixSymbol> m_FlatMultipliers;
	vector<MatrixExpression> m_Multipliers;
	

	bool operator==(const CompositionOfSum& rhs) const;
	CompositionOfSum& operator*=(const CompositionOfSum& rhs);	
	CompositionOfSum& operator*=(const MatrixExpression& rhs);
	CompositionOfSum& operator*=(const MatrixSymbol& rhs);
	CompositionOfSum& operator*=(const Sum& scalar);
	CompositionOfSum operator-() const;

	bool HasFlatPartOnly();

	CompositionOfSum& ToCanonicalView();
		
	string AsLatexCode() const;

	void ToFlatSummands(vector<CompositionOfSum>& flatSummands);
	CompositionOfSum& SimplifyMultiplicative();

	static bool equal_matrix_parts(const CompositionOfSum& lhs, const CompositionOfSum& rhs);
};