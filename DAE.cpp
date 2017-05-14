#include "stdafx.h"
#include "DAE.h"




ODE1::ODE1(const RealMatrix& B) : m_A(IdentityMatrix(B)), m_B(B)
{

}

ODE1::ODE1(const RealMatrix& A, const RealMatrix& B) : m_A(IdentityMatrix(B)), m_B(B)
{
	if (IsFZero(A.Det()))
	{
		throw EXCEPTION;
	}

	auto AInv = A.Inverse();



}

void ODE1::CheckStability(uint32_t numIterations /*= 1000*/, float t0 /*= 0.f*/)
{
	//float delta = numeric_limits<float>::max() / float(numIterations);

	//float t = t0;

	//for (uint32_t c = 0, size = numIterations / 2; c < size; c++)
	//{
	//	float x = exp( - t)
	//	

	//	t += delta;
	//}
}

uint32_t ODE1::GetMatrixDimensional() const
{
	return m_A.m_HorizontalDimensional;
}

std::string ODE1::AsString() const
{
	string result;
	result += m_A.AsString() + " * " + "dx + " + m_B.AsString() + " * x = O";

	return result;
}


