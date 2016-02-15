#pragma once

#include "Matricies.h"


class OperatorR;

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


	string Print(bool separated = true) const;
	std::string PrintRankedMatrix(bool separated = true) const;


	Matrix GetMatrixQ() const;
	Matrix GetMatrixGamma() const;
	OperatorR GetOperatorR() const;
private:
	void FullComputation();
	void RankComputationForBr(vector<ElementIndex>* linearlyIndependent);
	void ComputeMatrixQ(vector<ElementIndex>& bLinearlyIndependent);

	Matrix EnOOOMatrix(const Matrix& gamma) const;

	friend struct ESFDesc;
	MatrixDr();

};



class DAE;

class OperatorR
{
public:
	OperatorR();
	OperatorR(initializer_list<Matrix> il);

	vector<Matrix> m_Rs;

	Matrix& operator[](uint32_t i);
	Matrix operator[](uint32_t i) const;

	string AsString() const;

	DAE ApplyTo(const Matrix& A, const Matrix& B, const Matrix& QInv) const;
	DAE ApplyTo(const DAE& dae) const;
	//private:
};



//A*dx + B*x = 0
class ODE1
{
public:
	ODE1(const Matrix& B);
	ODE1(const Matrix& A, const Matrix& B);

	Matrix m_A;
	Matrix m_B;

	void CheckStability(uint32_t numIterations = 1000, float t0 = 0.f);
	uint32_t GetMatrixDimensional() const;
	string AsString() const;
};




struct ESFDesc;


class DAE
{
public:
	DAE();
	DAE(const Matrix& A, const Matrix& B);//A*dx + B*x = 0

	map<uint32_t, Matrix> m_Coefficients;

	uint32_t GetOrder() const;
	uint32_t GetMatrixDimensional() const;

	Matrix& operator[](uint32_t order);
	Matrix operator[](uint32_t order) const;

	string AsString() const;
	void EraseZeroMatricies();

	ESFDesc ConvertToESF(const uint32_t MAX_INDEX = 5);
private:
	static void GrowLambdarMatrix(Matrix& Lambdar, const Matrix& A, const Matrix& B);
};


struct ESFDesc
{
	uint32_t d;
	uint32_t n;
	uint32_t Index;
	MatrixDr Dr;
	Matrix Gamma;
	OperatorR R;
	Matrix Lambdar;
	Matrix Q;
};

void PrintESFDesc(const ESFDesc& desc);


