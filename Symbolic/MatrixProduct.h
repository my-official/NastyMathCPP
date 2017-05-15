#pragma once





class Sum;


//
//class MatrixProduct : boost::operators<MatrixProduct>
//{
//public:
//	float m_Coefficient;
//	vector<int> m_Numbers;
//	vector<MatrixSymbol> m_Matricies;
//	
//	vector< Sum > m_Multipliers;
//	vector< Sum > m_Dividers;
//
//	MatrixProduct() : m_Coefficient(1)
//	{
//	}
//	MatrixProduct(int value);
//
//	bool operator==(const MatrixProduct& rhs) const;
//
//	MatrixProduct& operator*=(const MatrixProduct& rhs);
//	MatrixProduct& operator/=(const MatrixProduct& rhs);
//
//	MatrixProduct& operator*=(int factor);
//
//	bool IsSymbolic() const;
//	MatrixProduct& Substitute(string name, const Sum& value);
//	MatrixProduct& Approximate();
//	string AsString() const;
//	MatrixProduct& Simplify();
//
//	void ToFlatSum(Sum& sum);
//
//
//	MatrixProduct& AlphabeticalSort();
//	static bool alphabetical_less(const MatrixProduct& lhs, const  MatrixProduct& rhs);
//	MatrixProduct& ToCanonicalView();
//private:
//
//	template <class Functor>
//	void ForEachMultipliersAndDividers(Functor functor);
//
//	template <class Functor>
//	bool ForEachMultipliersAndDividersWhileTrue(Functor functor) const;
//
//	template <class Functor>
//	bool ForEachMultipliersAndDividersWhileFalse(Functor functor) const;
//};



#include "MatrixProduct.inl"