#pragma once

#include "..\Utils.h"

template <typename ElementType>
using Vector = std::vector<ElementType>;


template <typename ElementType>
ElementType DotProduct(const Vector<ElementType>& a, const Vector<ElementType>& b);




enum class RankMethod
{
	LeftTop,
	RightTop,
	RightBottom,
	LeftBottom
};

typedef std::pair<uint32_t, uint32_t> ElementIndex;

//class Matrix;
//typedef Matrix Minor;

template <class T> 
struct equal_to_zero {
	bool operator() (const T& x) const { return x == 0; }
	typedef T first_argument_type;	
	typedef bool result_type;
};



template <typename ElementType,
	class DerivedMatrixT,
	class IsEqual = equal_to<ElementType>,
	class IsEqualZero = equal_to_zero<ElementType> >
class BaseMatrix : boost::operators< DerivedMatrixT >
{
public:

	BaseMatrix() : m_VerticalDimensional(0),
		m_HorizontalDimensional(0)		
	{

	}

	using zero_t = float;

	BaseMatrix(zero_t) : BaseMatrix()
	{

	}


	BaseMatrix(uint32_t vertical, uint32_t horizontal);//заполняет нулями


	BaseMatrix(initializer_list<ElementType> il);//предполагается квадратная матрица
	BaseMatrix(initializer_list<DerivedMatrixT> il);//предполагается квадратная матрица

	BaseMatrix(uint32_t vertical, uint32_t horizontal, initializer_list<ElementType> il);
	BaseMatrix(uint32_t vertical, uint32_t horizontal, initializer_list<DerivedMatrixT> il);

	

	virtual ~BaseMatrix() { static_assert(sizeof(DerivedMatrixT) == sizeof(BaseMatrix) && is_base_of<BaseMatrix, DerivedMatrixT>::value,"Derived Matrix type must have same data-members"); }



	uint32_t m_VerticalDimensional;
	uint32_t m_HorizontalDimensional;

	vector<ElementType> m_Data;


	virtual bool operator==(const DerivedMatrixT& rhs) const;

	virtual DerivedMatrixT& operator+=(const DerivedMatrixT& rhs);
	virtual DerivedMatrixT& operator-=(const DerivedMatrixT& rhs);
	virtual DerivedMatrixT& operator*=(const DerivedMatrixT& rhs);
	virtual DerivedMatrixT& operator/=(const DerivedMatrixT& rhs);


	virtual DerivedMatrixT& Scale(const ElementType& rhs);	

	
	virtual DerivedMatrixT operator-() const;

	virtual ElementType& operator[](ElementIndex idx);
	virtual ElementType operator[](ElementIndex idx) const;


	virtual void SetDimensionals(uint32_t vertical, uint32_t horizontal);

	virtual DerivedMatrixT& GrowBottom(DerivedMatrixT m);
	virtual DerivedMatrixT& GrowRight(DerivedMatrixT m);

	virtual void FlipHorizontal();
	virtual void FlipVertical();
	virtual void ExcludeColumnsExcept(const vector<ElementIndex>& elements);

	virtual bool IsZeroMatrix() const;
	virtual bool IsIdentityMatrix() const;
	virtual uint32_t FindColumn(const DerivedMatrixT& srcColumn, uint32_t offset_x = 0) const;
	static const uint32_t npos = -1;



	virtual DerivedMatrixT GetSubMatrix(uint32_t y, uint32_t x, uint32_t verticalDimensional, uint32_t horizontalDimensional) const;
	virtual void SetSubMatrix(uint32_t y, int32_t x, const DerivedMatrixT& m);
		

	virtual Vector<ElementType> GetRowAsVector(uint32_t verticalIndex) const;
	virtual Vector<ElementType> GetColumnAsVector(uint32_t horizontalIndex) const;

	
	virtual void ExtractMinor(uint32_t verticalIndex, uint32_t horizontalIndex, DerivedMatrixT& minor) const;//координаты в матрице исключающего элемента
		


	virtual ElementType Det() const;
	virtual uint32_t Rank(RankMethod rankMethod = RankMethod::LeftTop, vector<ElementIndex>* linearlyIndependent = nullptr) const;
	virtual std::string AsStringRanked(RankMethod rankMethod = RankMethod::LeftTop) const;
	
	virtual ElementType Trace() const;

	virtual DerivedMatrixT Transpose() const;
	virtual DerivedMatrixT Inverse() const;

	virtual std::string AsString() const;

	virtual std::string AsLatexCode() const;
	virtual std::string AsLatexCodeRanked(RankMethod rankMethod = RankMethod::LeftTop) const;


	


//protected:
public:
	virtual uint32_t RankInternal_Step1(vector<ElementIndex>* linearlyIndependent, uint32_t target_x = 0, uint32_t iteration = 0);

private:
	IsEqual m_IsEqual;
	IsEqualZero m_IsEqualZero;
};






//template <
//	class charT, class traits,
//	typename ElementType,	class DerivedMatrixT,	class IsEqual = equal_to<ElementType>,	class IsEqualZero = equal_to_zero<ElementType> 
//>
//std::basic_ostream<charT, traits>& operator<<(basic_ostream<charT, traits>& latex, const BaseMatrix<ElementType, DerivedMatrixT, IsEqual, IsEqualZero>& a);




template <class DerivedMatrixT>
DerivedMatrixT ZeroMatrix(const DerivedMatrixT& A);

template <class DerivedMatrixT>
DerivedMatrixT IdentityMatrix(const DerivedMatrixT& A);

template <class DerivedMatrixT>
DerivedMatrixT PermutationColumnsMatrix(const DerivedMatrixT& src, const DerivedMatrixT& dest);


template <class DerivedMatrixT>
string to_string(const DerivedMatrixT& val)
{
	return val.AsString();
}

template <class DerivedMatrixT>
string to_latex(const DerivedMatrixT& val)
{
	return val.AsLatexCode();
}








#include "BaseMatrix.inl"





