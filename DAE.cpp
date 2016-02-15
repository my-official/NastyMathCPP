#include "stdafx.h"
#include "DAE.h"




MatrixDr::MatrixDr(const Matrix Br, const Matrix Ar, const Matrix Lambdar) : Matrix(1, 3, { Br, Ar, Lambdar }),
m_BrOrigin({ 0,0 }),
m_ArOrigin({ 0,Br.m_HorizontalDimensional }),
m_LambdarOrigin({ 0,Br.m_HorizontalDimensional + Ar.m_HorizontalDimensional }),
m_Br(Br),
m_Ar(Ar),
m_Lambdar(Lambdar),
m_D(0),
m_Q(Ar.m_HorizontalDimensional, Ar.m_HorizontalDimensional)

{
	FullComputation();
}

MatrixDr::MatrixDr()
{

}


std::string MatrixDr::Print(bool separated /*= true*/) const
{
	std::string result = "{\n";

	for (uint32_t y = 0; y < m_VerticalDimensional; ++y)
	{
		for (uint32_t x = 0; x < m_HorizontalDimensional; ++x)
		{
			if (separated)
			{
				if (x == m_ArOrigin.second)
				{
					result += " | ";
				}

				if (x == m_LambdarOrigin.second)
				{
					result += " | ";
				}
			}

			result += to_string((*this)[{y, x}]) + ", ";
		}

		result.pop_back();
		result.pop_back();
		result += "\n";
	}

	result += " }\n";

	return result;
}

std::string MatrixDr::PrintRankedMatrix(bool separated /*= true*/) const
{
	vector<uint32_t> widths(m_HorizontalDimensional);

	std::string result = "{\n";

	for (uint32_t y = 0; y < m_VerticalDimensional; ++y)
	{
		for (uint32_t x = 0; x < m_HorizontalDimensional; ++x)
		{
			if (separated)
			{
				if (x == m_ArOrigin.second)
				{
					result += " | ";
				}

				if (x == m_LambdarOrigin.second)
				{
					result += " | ";
				}
			}


			if (std::find(m_SpecialLinearlyIndependent.begin(), m_SpecialLinearlyIndependent.end(), make_pair(y, x)) != m_SpecialLinearlyIndependent.end())
			{
				auto value = to_string((*this)[{y, x}]);

				widths[x] = std::max<uint32_t>(widths[x], (uint32_t)value.length());
				result += value + ", ";
			}
			else
			{
				result += "********, ";
			}
		}

		result.pop_back();
		result.pop_back();
		result += "\n";
	}

	result += " }\n";

	return result;
}

Matrix MatrixDr::GetMatrixQ() const
{
	return m_Q;
}

Matrix MatrixDr::GetMatrixGamma() const
{
	return m_Gamma;
}

OperatorR MatrixDr::GetOperatorR() const
{
	Matrix Gamma = GetMatrixGamma();

	Matrix R = EnOOOMatrix(Gamma) * Gamma.Transpose() * (Gamma * Gamma.Transpose()).Inverse();

	OperatorR result;

	for (uint32_t x = 0, size = R.m_HorizontalDimensional; x < size; x += m_Ar.m_HorizontalDimensional)
	{
		auto Ri = R.GetSubMatrix(0, x, m_Ar.m_HorizontalDimensional, m_Ar.m_HorizontalDimensional);

		if (!Ri.IsZeroMatrix())
			result.m_Rs.push_back(Ri);
	}

	return result;
}

void MatrixDr::FullComputation()
{
	vector<ElementIndex> generalLinearlyIndependent;
	auto rank = Matrix::Rank(RankMethod::RightTop, &generalLinearlyIndependent);

	vector<ElementIndex> lambdaLinearlyIndependent;
	auto rankLambdar = m_Lambdar.Rank(RankMethod::LeftTop, &lambdaLinearlyIndependent);

	uint32_t n = m_Ar.m_HorizontalDimensional;

	m_D = rank - n - rankLambdar;

	vector<ElementIndex> aLinearlyIndependent;
	auto rankAr = m_Ar.Rank(RankMethod::LeftTop, &aLinearlyIndependent);

	vector<ElementIndex> bLinearlyIndependent;
	RankComputationForBr(&bLinearlyIndependent);



	for (auto& idx : aLinearlyIndependent)
	{
		idx.first += m_ArOrigin.first;
		idx.second += m_ArOrigin.second;
	}

	for (auto& idx : lambdaLinearlyIndependent)
	{
		idx.first += m_LambdarOrigin.first;
		idx.second += m_LambdarOrigin.second;
	}

	m_SpecialLinearlyIndependent.insert(m_SpecialLinearlyIndependent.end(), bLinearlyIndependent.begin(), bLinearlyIndependent.end());
	m_SpecialLinearlyIndependent.insert(m_SpecialLinearlyIndependent.end(), aLinearlyIndependent.begin(), aLinearlyIndependent.end());
	m_SpecialLinearlyIndependent.insert(m_SpecialLinearlyIndependent.end(), lambdaLinearlyIndependent.begin(), lambdaLinearlyIndependent.end());

	ComputeMatrixQ(bLinearlyIndependent);

	//m_Q = IdentityMatrix(m_Q);


	m_Gamma = *this;
	m_Gamma.ExcludeColumnsExcept(m_SpecialLinearlyIndependent);

}

void MatrixDr::RankComputationForBr(vector<ElementIndex>* linearlyIndependent)
{
	auto Br = m_Br;

	auto rankBr = Br.Rank();

	Br.FlipHorizontal();
	Br.RankInternal_Step1(linearlyIndependent, 0, rankBr - m_D);

	for (auto& e : *linearlyIndependent)
	{
		e.second = Br.m_HorizontalDimensional - 1 - e.second;
	}
}


void MatrixDr::ComputeMatrixQ(vector<ElementIndex>& bLinearlyIndependent)
{
	if (bLinearlyIndependent.size() != m_D)//скорее assert
	{
		throw logic_error("ComputeMatrixQ bLinearlyIndependent.size() != m_D");
	}

	//другой алгоритм
	uint32_t reverse_iteration_exclude = m_Br.m_HorizontalDimensional - m_D - 1;

	uint32_t reverse_iteration_include = m_Br.m_HorizontalDimensional - 1;

	uint32_t x = m_Br.m_HorizontalDimensional - 1;
	do
	{
		if (std::find_if(bLinearlyIndependent.begin(), bLinearlyIndependent.end(), [x](const ElementIndex& lhs)->bool
		{
			return lhs.second == x;
		}) != bLinearlyIndependent.end())
		{
			m_Q[{x, reverse_iteration_include}] = 1.0f;
			--reverse_iteration_include;
		}
		else
		{
			m_Q[{x, reverse_iteration_exclude }] = 1.0f;
			--reverse_iteration_exclude;
		}
	} while (x--);

}

Matrix MatrixDr::EnOOOMatrix(const Matrix& gamma) const
{
	Matrix result(m_Ar.m_HorizontalDimensional, gamma.m_HorizontalDimensional);

	Matrix En(m_Ar.m_HorizontalDimensional, m_Ar.m_HorizontalDimensional);
	En = IdentityMatrix(En);

	result.SetSubMatrix(0, 0, En);

	return result;
}

OperatorR::OperatorR(initializer_list<Matrix> il)
{
	for (auto& m : il)
	{
		m_Rs.push_back(m);
	}
}

OperatorR::OperatorR()
{

}

Matrix& OperatorR::operator[](uint32_t i)
{
	return	m_Rs.at(i);
}

Matrix OperatorR::operator[](uint32_t i) const
{
	return	m_Rs.at(i);
}

std::string OperatorR::AsString() const
{
	string result;

	for (uint32_t c = 0, size = (uint32_t)m_Rs.size(); c < size; c++)
	{
		result += "R_" + to_string(c) + " = " + m_Rs[c].AsString() + "\n";
	}

	return result;
}


DAE OperatorR::ApplyTo(const Matrix& A, const Matrix& B, const Matrix& QInv) const
{
	//A *dx + B * x = 0;

	uint32_t order = (uint32_t)m_Rs.size() - 1;

	if (order == 0)
		throw logic_error("OperatorR::ApplyTo order == 0");

	DAE result;

	do
	{
		auto Ri = m_Rs[order];
		result[1 + order] += Ri*A * QInv;
		result[order] += Ri*B * QInv;
	} while (order--);


	return result;

}

DAE OperatorR::ApplyTo(const DAE& dae) const
{
	DAE result;

	for (uint32_t order = 0, size = (uint32_t)m_Rs.size(); order < size; order++)
	{
		auto Ri = m_Rs[order];

		for (auto& coef : dae.m_Coefficients)
		{
			result[coef.first + order] += Ri*coef.second;
		}
	}


	result.EraseZeroMatricies();

	return result;
}

DAE::DAE()
{

}

DAE::DAE(const Matrix& A, const Matrix& B)
{
	m_Coefficients.insert(make_pair(1, A));
	m_Coefficients.insert(make_pair(0, B));

}

uint32_t DAE::GetOrder() const
{
	if (!m_Coefficients.empty())
		return (--m_Coefficients.end())->first;
	else
		return 0;

}

uint32_t DAE::GetMatrixDimensional() const
{
	if (!m_Coefficients.empty())
	{
		return m_Coefficients.begin()->second.m_HorizontalDimensional;
	}
	else
	{
		return 0;
	}
}

Matrix& DAE::operator[](uint32_t order)
{
	return m_Coefficients[order];
}

Matrix DAE::operator[](uint32_t order) const
{
	auto it = m_Coefficients.find(order);
	if (it != m_Coefficients.end())
	{
		return it->second;
	}
	else
	{
		return ZeroMatrix(Matrix(GetMatrixDimensional(), GetMatrixDimensional()));
	}
}

std::string DAE::AsString() const
{
	string result;
	for (auto it = m_Coefficients.rbegin(); it != m_Coefficients.rend(); ++it)
	{
		result += it->second.AsString() + " * " + "d" + to_string(it->first) + "x +";
	}

	if (!result.empty())
	{
		result.replace(result.length() - 1, 1, "= O");
	}



	return result;
}

void DAE::EraseZeroMatricies()
{
	for (auto it = m_Coefficients.begin(); it != m_Coefficients.end();)
	{
		if (it->second.IsZeroMatrix())
		{
			it = m_Coefficients.erase(it);
		}
		else
		{
			++it;
		}
	}
}

ESFDesc DAE::ConvertToESF(const uint32_t MAX_INDEX /*= 5*/)
{
	if (m_Coefficients.count(0) == 0)
	{
		throw logic_error("ConvertToESF m_Coefficients.count(0) == 0");
	}
	Matrix B = m_Coefficients[0];

	Matrix A = m_Coefficients.count(1) ? m_Coefficients[1] : ZeroMatrix(B);



	using namespace std::rel_ops;

	uint32_t n = A.m_HorizontalDimensional;


	Matrix O = ZeroMatrix(A);

	Matrix Br = B;

	Br.GrowBottom(O);

	Matrix Ar(A);
	Ar.GrowBottom(B);

	Matrix Lambdar(2, 1, {
		O,
		A,
	});

	uint32_t rankLambdar = Lambdar.Rank();

	uint32_t index = 1;
	for (; index < MAX_INDEX; ++index)
	{
		Matrix LambdarNext = Lambdar;
		GrowLambdarMatrix(LambdarNext, A, B);

		uint32_t rankLambdarNext = LambdarNext.Rank();

		/*	cout << "trying index " << index << endl;
		cout << "Rank Lambdar = " << rankLambdar << endl;
		cout << "Rank rankLambdarNext = " << rankLambdarNext << endl;*/


		if (rankLambdarNext - rankLambdar == n)
		{
			break;
		}

		Br.GrowBottom(O);
		Ar.GrowBottom(O);

		Lambdar = LambdarNext;
		rankLambdar = Lambdar.Rank();

	}

	if (index == MAX_INDEX)
	{
		throw logic_error("more then MAX_INDEX");
	}

	/////
	//ESFDesc result2;
	//result2.Index = index;
	//
	//return result2;

	/////

	MatrixDr Dr(Br, Ar, Lambdar);
	Matrix Q = Dr.GetMatrixQ();

	Matrix Gamma = Dr.GetMatrixGamma();

	/*if (Gamma.Det() == 0)
	throw logic_error("Gamma.Det() == 0");*/


	OperatorR R = Dr.GetOperatorR();

	DAE esf = R.ApplyTo(A, B, Q);

	esf.EraseZeroMatricies();





	ESFDesc result;
	result.d = Dr.m_D;
	result.n = A.m_HorizontalDimensional;
	result.Index = index;
	result.Dr = Dr;
	result.Gamma = Gamma;
	result.R = R;
	result.Lambdar = Lambdar;
	result.Q = Q;
	*this = esf;

	return result;
}



void DAE::GrowLambdarMatrix(Matrix& Lambdar, const Matrix& A, const Matrix& B)
{
	Matrix O = ZeroMatrix(A);
	Matrix T;

	if (Lambdar.m_HorizontalDimensional > A.m_HorizontalDimensional)
	{
		for (uint32_t c = 0, size = Lambdar.m_HorizontalDimensional - A.m_HorizontalDimensional; c < size; c += O.m_HorizontalDimensional)
		{
			T.GrowRight(O);
		}
	}

	T.GrowRight(B);
	T.GrowRight(A);

	O = ZeroMatrix(Lambdar.GetSubMatrix(0, 0, Lambdar.m_VerticalDimensional, A.m_HorizontalDimensional));

	Lambdar.GrowRight(O);

	Lambdar.GrowBottom(T);
}

ODE1::ODE1(const Matrix& B) : m_A(IdentityMatrix(B)), m_B(B)
{

}

ODE1::ODE1(const Matrix& A, const Matrix& B) : m_A(IdentityMatrix(B)), m_B(B)
{
	if (IsFZero(A.Det()))
	{
		throw logic_error("ODE1(const Matrix& A, const Matrix& B) IsFZero(A.Det())");
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


void PrintESFDesc(const ESFDesc& desc)
{
	cout << "n == " << desc.n << endl;

	cout << "Index == " << desc.Index << endl;

	cout << "d == " << desc.d << endl;

	cout << "Dr = " << desc.Dr.Print() << endl;

	cout << "Rank Dr = " << desc.Dr.Rank() << endl;

	cout << "Ranked Dr = " << desc.Dr.PrintRankedMatrix() << endl;

	cout << "Gamma = " << desc.Gamma.AsString() << endl;

	cout << "Q = " << desc.Q.AsString() << endl;

	cout << "R = " << desc.R.AsString() << endl;
}