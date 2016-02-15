#pragma once


typedef std::complex<float> complexF;


typedef std::vector<float> Vector;
typedef std::complex<float> complexF;


float DotProduct(const Vector& a, const Vector& b);
bool FCmp(float a, float b);
bool FCmp(complexF a, complexF b);

bool IsFZero(float value);

float sgn(float value);



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
	virtual Matrix operator-(const Matrix& rhs) const;
	virtual Matrix operator*(const Matrix& rhs) const;
	virtual float& operator[](ElementIndex idx);
	virtual float operator[](ElementIndex idx) const;

	virtual void SetDimensionals(uint32_t vertical, uint32_t horizontal) override;
	virtual float Det(vector<float>* bottomDets = nullptr) const override;	
	virtual Eigen::MatrixXf AsEigenMatrix() const;
	

	virtual std::string AsString() const;
		
	virtual uint32_t Rank(RankMethod rankMethod = RankMethod::LeftTop, vector<ElementIndex>* linearlyIndependent = nullptr) const;
	std::string AsStringRanked(RankMethod rankMethod = RankMethod::LeftTop) const;

	virtual float Trace() const;

	virtual vector<complexF> Eigenvalues() const;
	virtual Vector SingularValues() const;
	virtual float NormSpectral() const;

	virtual bool IsStable() const;

	//virtual vector<complexF> Eigenvalues2() const;	
	//virtual void Svd(Matrix& U, Matrix& Sigma, Matrix& V) const;



	
	virtual Vector GetRowAsVector(uint32_t verticalIndex) const override;
	virtual Vector GetColumnAsVector(uint32_t horizontalIndex) const override;

	virtual void ExtractMinor(uint32_t verticalIndex, uint32_t horizontalIndex, Minor& minor) const;//координаты в матрице исключающего элемента

		
	void SetSubMatrix(uint32_t y, int32_t x, const Matrix& m);
	Matrix GetSubMatrix(uint32_t y, uint32_t x, uint32_t verticalDimensional, uint32_t horizontalDimensional) const;

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
	bool IsIdentityMatrix() const;
	uint32_t FindColumn(const Matrix& srcColumn, uint32_t offset_x = 0) const;
	static const uint32_t npos = -1;


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
Matrix PermutationColumnsMatrix(const Matrix& src, const Matrix& dest);







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

	ForAllMatrixInternal(M, (uint32_t)M.m_Data.size(), beginValue, endValue, step, fn);
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