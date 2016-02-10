#pragma once


typedef std::vector<float> Vector;

float DotProduct(const Vector& a, const Vector& b);
bool FCmp(float a, float b);
bool IsFZero(float value);



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

	virtual float Det(vector<float>* bottomDets = nullptr) const = 0;
	//virtual int Rank() const = 0;

	virtual Vector GetRowAsVector(uint32_t verticalIndex) const = 0;
	virtual Vector GetColumnAsVector(uint32_t horizontalIndex) const = 0;	
	
};


typedef std::pair<uint32_t, uint32_t> ElementIndex;

enum class RankMethod
{
	LeftTop,
	RightTop,
	RightBottom,
	LeftBottom
};


class Matrix;
typedef Matrix Minor;


class Matrix : public BaseMatrix
{
public:
	

	vector<float> m_Data;

	Matrix() {}
	Matrix(uint32_t vertical, uint32_t horizontal);//заполняет нулями

	Matrix(initializer_list<float> il);//предполагается квадратная матрица
	Matrix(initializer_list<Matrix> il);//предполагается квадратная матрица
		
	Matrix(uint32_t vertical, uint32_t horizontal, initializer_list<float> il);
	Matrix(uint32_t vertical, uint32_t horizontal, initializer_list<Matrix> il);

	virtual ~Matrix() {}

	
	virtual bool operator==(const Matrix& rhs) const;	

	virtual Matrix& operator+=(const Matrix& rhs);
	virtual Matrix operator+(const Matrix& rhs) const;
	virtual Matrix operator*(const Matrix& rhs) const;
	virtual float& operator[](ElementIndex idx);
	virtual float operator[](ElementIndex idx) const;

	virtual void SetDimensionals(uint32_t vertical, uint32_t horizontal) override;
	virtual float Det(vector<float>* bottomDets = nullptr) const override;	

	virtual std::string AsString() const;
		
	virtual uint32_t Rank(RankMethod rankMethod = RankMethod::LeftTop, vector<ElementIndex>* linearlyIndependent = nullptr) const;
	std::string AsStringRanked(RankMethod rankMethod = RankMethod::LeftTop) const;

	
	virtual Vector GetRowAsVector(uint32_t verticalIndex) const override;
	virtual Vector GetColumnAsVector(uint32_t horizontalIndex) const override;

	virtual void ExtractMinor(uint32_t verticalIndex, uint32_t horizontalIndex, Minor& minor) const;//координаты в матрице исключающего элемента

		
	void SetSubMatrix(uint32_t y, int32_t x, const Matrix& m);
	Matrix GetSubMatrix(uint32_t  y, uint32_t  x, uint32_t  verticalDimensional, uint32_t horizontalDimensional);

	virtual Matrix& GrowBottom(Matrix m);
	virtual Matrix& GrowRight(Matrix m);
	
	Matrix Transpose() const;
	Matrix Inverse() const;

	void FlipHorizontal();
	void FlipVertical();
	void ExcludeColumnsExcept(const vector<ElementIndex>& elements);

	template <typename Fn>
	static void ForAllMatrix(uint32_t vertical, uint32_t horizontal, float beginValue, float endValue, float step, Fn fn);

	bool IsZeroMatrix() const;
private:
	friend class MatrixDr;
	uint32_t RankInternal_Step1(vector<ElementIndex>* linearlyIndependent, uint32_t target_x = 0, uint32_t iteration = 0);		
	
	template <typename Fn>
	static void ForAllMatrixInternal(Matrix& M, uint32_t target_idx, float beginValue, float endValue, float step, Fn fn);
	static void ForAllMatrixInternalRefill(Matrix& M, uint32_t target_idx, float beginValue);
};






std::string RankedMatrixAsString(const Matrix& m, const vector<ElementIndex>& linearlyIndependent);
Matrix ZeroMatrix(const Matrix& A);
Matrix IdentityMatrix(const Matrix& A);
void GrowLambdarMatrix(Matrix& Lambdar, const Matrix& A, const Matrix& B);



class DAE
{
public:
	DAE();	
		
	map<uint32_t, Matrix> m_Coefficients;

	uint32_t GetMatrixDimensional() const;

	Matrix& operator[](uint32_t order);
	Matrix operator[](uint32_t order) const;

	string AsString() const;
	void EraseZeroMatrix();
};




class OperatorR
{
public:
	OperatorR();
	OperatorR(initializer_list<Matrix> il);
	
	vector<Matrix> m_Rs;

	Matrix& operator[](uint32_t i);
	Matrix operator[](uint32_t i) const;

	string AsString() const;

	DAE ApplyTo(const Matrix& A, const Matrix& B, const Matrix& Q);
private:
};






class MatrixDr : public Matrix
{
public:

	MatrixDr(const Matrix Br, const Matrix Ar, const Matrix Lambdar);

	ElementIndex m_BrOrigin;
	ElementIndex m_ArOrigin;
	ElementIndex m_LambdarOrigin;

	Matrix m_Br;
	Matrix m_Ar;
	Matrix m_Lambdar;

	uint32_t m_D;

	Matrix m_Q;
	vector<ElementIndex> m_SpecialLinearlyIndependent;	
	Matrix m_Gamma;


	
	string Print(bool separated = true);
	std::string PrintRankedMatrix(bool separated = true);

	
	Matrix GetMatrixQ() const;
	Matrix GetMatrixGamma() const;
	OperatorR GetOperatorR() const;
private:
	void FullComputation();
	void CompleteRankComputation(vector<ElementIndex>* linearlyIndependent);

	Matrix EnOOOMatrix(const Matrix& gamma) const;
};






template <typename Fn>
void Matrix::ForAllMatrix(uint32_t vertical, uint32_t horizontal, float beginValue, float endValue, float step, Fn fn)
{
	if (vertical == 0 || horizontal == 0)
		throw logic_error("ForAllMatrix vertical == 0 || horizontal == 0");

	Matrix M(vertical, horizontal);

	for (auto& value : M.m_Data)
	{
		value = beginValue;
	}

	fn(M);

	ForAllMatrixInternal(M, M.m_Data.size(), beginValue, endValue, step, fn);
}

template <typename Fn>
void Matrix::ForAllMatrixInternal(Matrix& M, uint32_t target_idx, float beginValue, float endValue, float step, Fn fn)
{	
	
	for (uint32_t idx = 0; idx < target_idx; ++idx)
	{		
		//M.m_Data[idx] += step;

		for (float c = beginValue + step; c <= endValue; c += step)
		{	
			M.m_Data[idx] = c;
			ForAllMatrixInternalRefill(M, idx, beginValue);
			fn(M);
			
			ForAllMatrixInternal(M, idx, beginValue, endValue, step, fn);
		//	ForAllMatrixInternal(M, idx, beginValue, endValue, step, fn);
		}
	}

}