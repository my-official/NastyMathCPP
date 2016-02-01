#pragma once


typedef std::vector<float> Vector;

float DotProduct(const Vector& a, const Vector& b);


class BaseMatrix
{
public:
	uint32_t m_VerticalDimensional;
	uint32_t m_HorizontalDimensional;
	
public:
	BaseMatrix() : m_VerticalDimensional(0),
		m_HorizontalDimensional(0)
		
	{

	}

	virtual ~BaseMatrix() {}

	virtual void SetDimensionals(uint32_t vertical, uint32_t horizontal) = 0;

	virtual float Det() const = 0;
	//virtual int Rank() const = 0;

	virtual Vector GetRowAsVector(uint32_t verticalIndex) const = 0;
	virtual Vector GetColumnAsVector(uint32_t horizontalIndex) const = 0;

	
	
};


typedef std::pair<uint32_t, uint32_t> ElementIndex;


class Matrix : public BaseMatrix
{
public:
	vector<float> m_Data;

	Matrix() {}
	Matrix(initializer_list<float> il);

	

	virtual Matrix operator*(const Matrix& rhs);
	virtual float& operator[](ElementIndex idx);
	virtual float operator[](ElementIndex idx) const;

	virtual void SetDimensionals(uint32_t vertical, uint32_t horizontal) override;
	virtual float Det() const override;	

	virtual float Rank(vector<ElementIndex>* linearlyIndependent = nullptr) const;

	virtual Vector GetRowAsVector(uint32_t verticalIndex) const override;
	virtual Vector GetColumnAsVector(uint32_t horizontalIndex) const override;

	virtual Matrix GetMinor(uint32_t verticalIndex, uint32_t horizontalIndex) const;//координаты в матрице исключающего элемента

	virtual std::string AsString() const;
private:
	void RankInternal_Step1(uint32_t& target_x, uint32_t& iteration);
};

typedef Matrix Minor;


//class MatrixDr : public BaseMatrix
//{
//public:
//	Matrix m_Br;
//	Matrix m_Ar;
//	Matrix m_Lambdar;
//
//	
//	virtual float Det() const override;
//
//	virtual Matrix operator*(const Matrix& rhs);
//private:
//	virtual void SetDimensionals(uint32_t horizontal, uint32_t vertical) override;
//};