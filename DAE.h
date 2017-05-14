#pragma once

#include "Matrix\BaseMatrix.h"
#include "Matrix\RealMatrix.h"


template <class MatrixT/* = RealMatrix*/>
class OperatorR;

template <class MatrixT/* = RealMatrix*/>
class MatrixDr;


template <class MatrixT = RealMatrix>
struct ESFDesc
{
	uint32_t d;
	uint32_t n;
	uint32_t Index;
	MatrixDr<MatrixT> Dr;
	MatrixT Gamma;
	OperatorR<MatrixT> R;
	MatrixT Lambdar;
	MatrixT Q;
	
};

template <class MatrixT = RealMatrix>
void PrintESFDesc(const ESFDesc<MatrixT>& desc);

template <class MatrixT = RealMatrix>
string PrintESFDescAsLatexCode(const ESFDesc<MatrixT>& desc);




template <class MatrixT = RealMatrix>
class DAE
{
public:
	DAE()	{	}
	DAE(const MatrixT& A, const MatrixT& B);//A*dx + B*x = 0

	map<uint32_t, MatrixT> m_Coefficients;

	uint32_t GetOrder() const;
	uint32_t GetMatrixDimensional() const;

	MatrixT& operator[](uint32_t order);
	MatrixT operator[](uint32_t order) const;

	string AsString() const;
	string AsLatexCode() const;
	void EraseZeroMatricies();

	ESFDesc<MatrixT> ConvertToESF(const uint32_t MAX_INDEX = 5, uint32_t idlingCounter = 0);
//private:
public:
	static void GrowLambdarMatrix(MatrixT& Lambdar, const MatrixT& A, const MatrixT& B);
};




 




template <class MatrixT = RealMatrix>
class OperatorR
{
public:
	OperatorR() {}
	OperatorR(initializer_list<MatrixT> il);

	vector<MatrixT> m_Rs;

	MatrixT& operator[](uint32_t i);
	MatrixT operator[](uint32_t i) const;

	OperatorR<MatrixT> operator*(const MatrixT& rhs) const;

	string AsString() const;
	string AsLatexCode() const;

	DAE<MatrixT> ApplyTo(const MatrixT& A, const MatrixT& B, const MatrixT& QInv) const;
	DAE<MatrixT> ApplyTo(const DAE<MatrixT>& dae) const;

	template <class MatrixTRhs>
	bool operator == (const OperatorR<MatrixTRhs>& rhs) const;
	//private:
};

template <class MatrixT>
template <class MatrixTRhs>
bool OperatorR<MatrixT>::operator==(const OperatorR<MatrixTRhs>& rhs) const
{
	return equal(m_Rs.begin(), m_Rs.end(), rhs.m_Rs.begin(), rhs.m_Rs.end(), [](const MatrixT& lhs, const MatrixTRhs& rhs)->bool
	{
		return rhs == lhs;
	});
}

template <class MatrixT = RealMatrix>
class MatrixDr : public MatrixT
{
public:

	MatrixDr(const MatrixT Br, const MatrixT Ar, const MatrixT Lambdar);

	ElementIndex m_BrOrigin;
	ElementIndex m_ArOrigin;
	ElementIndex m_LambdarOrigin;

	MatrixT m_Br;
	MatrixT m_Ar;
	MatrixT m_Lambdar;

	uint32_t m_D;

	MatrixT m_Q;
	vector<ElementIndex> m_SpecialLinearlyIndependent;
	MatrixT m_Gamma;


	string Print(bool separated = true) const;
	std::string PrintRankedMatrix(bool separated = true) const;


	MatrixT GetMatrixQ() const;
	MatrixT GetMatrixGamma() const;
	OperatorR<MatrixT> GetOperatorR() const;

	virtual std::string AsLatexCode() const;
	virtual std::string AsLatexCodeRanked(RankMethod rankMethod = RankMethod::LeftTop) const;
private:
	void FullComputation();
	vector<ElementIndex> RankComputationForBr();
	void ComputeMatrixQ(vector<ElementIndex>& bLinearlyIndependent);

	MatrixT EnOOOMatrix(const MatrixT& gamma) const;
		
	friend struct ESFDesc<MatrixT>;
	MatrixDr() {}
};








//A*dx + B*x = 0
class ODE1
{
public:
	ODE1(const RealMatrix& B);
	ODE1(const RealMatrix& A, const RealMatrix& B);

	RealMatrix m_A;
	RealMatrix m_B;

	void CheckStability(uint32_t numIterations = 1000, float t0 = 0.f);
	uint32_t GetMatrixDimensional() const;
	string AsString() const;
};





#include "DAE.inl"