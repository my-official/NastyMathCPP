#include "stdafx.h"
#include "Matricies.h"


float DotProduct(const Vector& a, const Vector& b)
{
	if (a.size() != b.size())
	{
		throw std::logic_error("DotProduct a.size() != b.size()");
	}

	float result = 0;

	for (size_t c = 0, size = a.size(); c < size; c++)
	{
		result += a[c] * b[c];
	}

	return result;
}


bool FCmp(float a, float b)
{
	return std::abs( a - b ) < 0.001f;
}

bool FCmp(complexF a, complexF b)
{
	return std::abs(a.real() - b.real()) < 0.001f && std::abs(a.imag() - b.imag()) < 0.001f;
}

bool IsFZero(float value)
{
	return FCmp(value, 0.f);
}


float sgn(float value)
{
	if (value > 0.f)
		return 1.f;
	else
		return -1.f;
}

std::string RankedMatrixAsString(const Matrix& m, const vector<ElementIndex>& linearlyIndependent)
{
	std::string result = "{\n";

	for (uint32_t y = 0; y < m.m_VerticalDimensional; ++y)
	{
		for (uint32_t x = 0; x < m.m_HorizontalDimensional; ++x)
		{
			if (std::find(linearlyIndependent.begin(), linearlyIndependent.end(), make_pair(y,x)  ) != linearlyIndependent.end() )
			{
				result += to_string(m[{y, x}]) + ", ";
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

Matrix ZeroMatrix(const Matrix& A)
{
	Matrix result(A);
	for (auto& e : result.m_Data)
	{
		e = 0;
	}
	return result;
}

Matrix IdentityMatrix(const Matrix& A)
{
	if (A.m_VerticalDimensional != A.m_HorizontalDimensional)
	{
		throw logic_error("IdentityMatrix m_VerticalDimensional != A.m_HorizontalDimensional");
	}
	
	Matrix result = ZeroMatrix(A);

	for (uint32_t c = 0; c < A.m_VerticalDimensional; c++)
	{
		result[{c,c}] = 1.0f;
	}
		
	return result;
}



Matrix PermutationColumnsMatrix(const Matrix& src, const Matrix& dest)
{
	if (src.m_HorizontalDimensional != dest.m_HorizontalDimensional ||
		src.m_VerticalDimensional != dest.m_VerticalDimensional)
	{
		throw logic_error("PermutationColumnsMatrix src.m_HorizontalDimensional != dest.m_HorizontalDimensional ||");
	}

	vector<uint32_t> usedColumns;
	usedColumns.reserve(src.m_HorizontalDimensional);

	Matrix result( src.m_HorizontalDimensional, src.m_VerticalDimensional);

	for (uint32_t src_x = 0; src_x < src.m_HorizontalDimensional; src_x++)
	{
		Matrix srcColumn = src.GetSubMatrix(0, src_x, src.m_VerticalDimensional, 1);


		uint32_t dest_x = dest.FindColumn(srcColumn);

		if (dest_x == Matrix::npos)
		{
			throw logic_error("PermutationColumnsMatrix FindColumn");
		}		

		while (find(usedColumns.begin(), usedColumns.end(), dest_x) != usedColumns.end())
		{
			dest_x = dest.FindColumn(srcColumn, dest_x + 1);

			if (dest_x == Matrix::npos)
			{
				throw logic_error("PermutationColumnsMatrix FindColumn");
			}
		}
		

		result[{src_x, dest_x}] = 1.f;		

		usedColumns.push_back(dest_x);
	}

	return result;
}



Matrix::Matrix(initializer_list<float> il)
{
	bool success = false;
	uint32_t dim = 0;
	for (uint32_t c = 1, size = 16; c <= size; c++)
	{
		if (c*c == il.size())
		{
			dim = c;
			success = true;
			break;
		}
	}

	if (!success)
	{
		throw std::logic_error("Matrix(initializer_list<float> ) matrix dimensional");
	}
	
	
	SetDimensionals(dim, dim);

	float* data = &m_Data[0];

	for (auto it = il.begin(); it != il.end(); ++it, ++data)
	{
		*data = *it;
	}
	
}

Matrix::Matrix(uint32_t vertical, uint32_t horizontal)
{
	SetDimensionals(vertical, horizontal);
	for (auto& e : m_Data)
	{
		e = 0.f;
	}
}

Matrix::Matrix(uint32_t vertical, uint32_t horizontal, initializer_list<float> il)
{
	if (vertical * horizontal != il.size())
	{
		throw std::logic_error("Matrix(uint32_t vertical, uint32_t horizontal, initializer_list<float> il) matrix dimensional");
	}

	SetDimensionals(vertical, horizontal);
	for (uint32_t y = 0; y < vertical; ++y)
	{
		for (uint32_t x = 0; x < horizontal; ++x)
		{
			m_Data[y * horizontal + x] = *(il.begin() + y * horizontal + x);
		}
	}
	
}

Matrix::Matrix(initializer_list<Matrix> il)
{
	float blockDimF = sqrt(float(il.size()));

	float intPart = 0;

	if (!IsFZero( modf(blockDimF, &intPart) ) )
	{
		throw std::logic_error("Matrix(initializer_list<Matrix> il) matrix dimensional");
	}

	uint32_t blockDim = uint32_t(blockDimF);

	vector<uint32_t> verticalDims;
	uint32_t verticalDim = 0;
	vector<uint32_t> horizontalDims;
	uint32_t horizontalDim = 0;

	for (uint32_t block_y = 0; block_y < blockDim; ++block_y)
	{
		const Matrix& m = *(il.begin() + block_y * blockDim);
		verticalDims.push_back(m.m_VerticalDimensional);
		verticalDim += m.m_VerticalDimensional;
	}

	for (uint32_t block_x = 0; block_x < blockDim; ++block_x)
	{
		const Matrix& m = *(il.begin() + block_x);
		horizontalDims.push_back(m.m_HorizontalDimensional);
		horizontalDim += m.m_HorizontalDimensional;
	}
		
	SetDimensionals(verticalDim, horizontalDim);

	uint32_t y = 0;
	


	for (uint32_t block_y = 0; block_y < blockDim; ++block_y)
	{
		uint32_t x = 0;
		uint32_t blockVertical = verticalDims[block_y];

		

		for (uint32_t block_x = 0; block_x < blockDim; ++block_x)
		{
			const Matrix& m = *(il.begin() + block_y * blockDim + block_x);

			
			uint32_t blockHorizontal = horizontalDims[block_x];

			if (m.m_VerticalDimensional != blockVertical)
			{
				throw std::logic_error("Matrix(initializer_list<Matrix> il) m_VerticalDimensional != verticalDims[block_y]");
			}

			if (m.m_HorizontalDimensional != blockHorizontal)
			{
				throw std::logic_error("Matrix(initializer_list<Matrix> il) m_HorizontalDimensional != horizontalDims[block_x]");
			}
			

			SetSubMatrix(y,x,m);

			x += blockHorizontal;
		}	
		y += blockVertical;
	}
	

}


void Matrix::SetSubMatrix(uint32_t y, int32_t x, const Matrix& m)
{
	if (y + m.m_VerticalDimensional > m_VerticalDimensional)
	{
		throw std::logic_error("SetSubMatrix y + m.m_VerticalDimensional <= m_VerticalDimensional");
	}

	if (x + m.m_HorizontalDimensional > m_HorizontalDimensional)
	{
		throw std::logic_error("SetSubMatrix x + m.m_HorizontalDimensional <= m_HorizontalDimensional");
	}

	for (uint32_t src_y = 0; src_y < m.m_VerticalDimensional; ++src_y)
	{
		for (uint32_t src_x = 0; src_x < m.m_HorizontalDimensional; ++src_x)
		{
			uint32_t dest_y = y + src_y;
			uint32_t dest_x = x + src_x;

			m_Data[dest_y * m_HorizontalDimensional + dest_x] = m.m_Data[src_y * m.m_HorizontalDimensional + src_x];
		}
	}
}

Matrix::Matrix(uint32_t vertical, uint32_t horizontal, initializer_list<Matrix> il)
{
	vector<uint32_t> verticalDims;
	uint32_t verticalDim = 0;
	vector<uint32_t> horizontalDims;
	uint32_t horizontalDim = 0;

	for (uint32_t block_y = 0; block_y < vertical; ++block_y)
	{
		const Matrix& m = *(il.begin() + block_y * horizontal);
		verticalDims.push_back(m.m_VerticalDimensional);
		verticalDim += m.m_VerticalDimensional;
	}

	for (uint32_t block_x = 0; block_x < horizontal; ++block_x)
	{
		const Matrix& m = *(il.begin() + block_x);
		horizontalDims.push_back(m.m_HorizontalDimensional);
		horizontalDim += m.m_HorizontalDimensional;
	}

	SetDimensionals(verticalDim, horizontalDim);



	uint32_t y = 0;

	for (uint32_t block_y = 0; block_y < vertical; ++block_y)
	{
		uint32_t x = 0;
		uint32_t blockVertical = verticalDims[block_y];



		for (uint32_t block_x = 0; block_x < horizontal; ++block_x)
		{
			const Matrix& m = *(il.begin() + block_y * horizontal + block_x);


			uint32_t blockHorizontal = horizontalDims[block_x];

			if (m.m_VerticalDimensional != blockVertical)
			{
				throw std::logic_error("Matrix(initializer_list<Matrix> il) m_VerticalDimensional != verticalDims[block_y]");
			}

			if (m.m_HorizontalDimensional != blockHorizontal)
			{
				throw std::logic_error("Matrix(initializer_list<Matrix> il) m_HorizontalDimensional != horizontalDims[block_x]");
			}


			SetSubMatrix(y, x, m);

			x += blockHorizontal;
		}
		y += blockVertical;
	}
}

bool Matrix::operator==(const Matrix& rhs) const
{	
	return std::equal(m_Data.begin(), m_Data.end(), rhs.m_Data.begin(), static_cast<bool(*)(float,float)>( &FCmp ) );
}

Matrix& Matrix::operator+=(const Matrix& rhs)
{
	if (m_HorizontalDimensional != rhs.m_HorizontalDimensional || m_VerticalDimensional != rhs.m_VerticalDimensional)
	{
		if (m_HorizontalDimensional == m_VerticalDimensional && m_HorizontalDimensional == 0)
		{
			*this = rhs;
			return *this;
		}
		else
		{
			throw logic_error("Matrix::operator+ m_HorizontalDimensional != rhs.m_HorizontalDimensional || m_VerticalDimensional != rhs.m_VerticalDimensional");
		}
	}

	Matrix& result = (*this);
	for (uint32_t c = 0, size = (uint32_t)m_Data.size(); c < size; c++)
	{
		result.m_Data[c] += rhs.m_Data[c];
	}
	return *this;
}

Matrix Matrix::operator+(const Matrix& rhs) const
{	
	if (m_HorizontalDimensional != rhs.m_HorizontalDimensional || m_VerticalDimensional != rhs.m_VerticalDimensional)
	{
		if (m_HorizontalDimensional == m_VerticalDimensional && m_HorizontalDimensional == 0)
		{
			return rhs;			
		}
		else
		{
			throw logic_error("Matrix::operator+ m_HorizontalDimensional != rhs.m_HorizontalDimensional || m_VerticalDimensional != rhs.m_VerticalDimensional");
		}
	}

	Matrix result = (*this);
	for (uint32_t c = 0, size = (uint32_t)m_Data.size(); c < size; c++)
	{
		result.m_Data[c] += rhs.m_Data[c];
	}
	return result;
}

Matrix Matrix::operator-(const Matrix& rhs) const
{
	if (m_HorizontalDimensional != rhs.m_HorizontalDimensional || m_VerticalDimensional != rhs.m_VerticalDimensional)
	{
		if (m_HorizontalDimensional == m_VerticalDimensional && m_HorizontalDimensional == 0)
		{
			return rhs;
		}
		else
		{
			throw logic_error("Matrix::operator+ m_HorizontalDimensional != rhs.m_HorizontalDimensional || m_VerticalDimensional != rhs.m_VerticalDimensional");
		}
	}

	Matrix result = (*this);
	for (uint32_t c = 0, size = (uint32_t)m_Data.size(); c < size; c++)
	{
		result.m_Data[c] -= rhs.m_Data[c];
	}
	return result;
}

Matrix Matrix::operator*(const Matrix& rhs) const
{
	if (m_HorizontalDimensional != rhs.m_VerticalDimensional)
	{
		throw std::logic_error("m_HorizontalDimensional != rhs.m_VerticalDimensional");
	}

	Matrix result;
	result.SetDimensionals(m_VerticalDimensional, rhs.m_HorizontalDimensional);

	
	for (uint32_t y = 0; y < m_VerticalDimensional; ++y)
	{
		Vector row = GetRowAsVector(y);
		for (uint32_t x = 0; x < rhs.m_HorizontalDimensional; ++x)
		{			
			Vector column = rhs.GetColumnAsVector(x);

			float value = DotProduct(row, column);

			result[{y, x}] = value;
		}
	}

	return result;
}

float& Matrix::operator[](ElementIndex idx)
{
	return m_Data[idx.first * m_HorizontalDimensional + idx.second];
}

float Matrix::operator[](ElementIndex idx) const
{
	return m_Data[idx.first * m_HorizontalDimensional + idx.second];
}

void Matrix::SetDimensionals(uint32_t vertical, uint32_t horizontal)
{
	m_Data.reserve(16*1024 / sizeof(float) );
	m_Data.resize(horizontal * vertical, 0.0f);
	m_HorizontalDimensional = horizontal;
	m_VerticalDimensional = vertical;
}

float Matrix::Det(vector<float>* bottomDets /*= nullptr*/) const
{
	if (m_HorizontalDimensional != m_VerticalDimensional || m_HorizontalDimensional == 0)
	{
		throw std::logic_error("Det m_HorizontalDimensional != rhs.m_VerticalDimensional");
	}

	if (m_HorizontalDimensional == 1)
	{
		return m_Data[0];
	}
		

	if (m_HorizontalDimensional == 2)
	{
		return ( (*this)[{ 0,0 }] * (*this)[{ 1,1 }] ) - ( (*this)[{ 0,1 }] * (*this)[{ 1,0 }] );		
	}

	if (m_HorizontalDimensional == 3)
	{	
		return (*this)[{ 0, 0 }] * (*this)[{ 1, 1 }] * (*this)[{ 2, 2 }]
			+ (*this)[{ 0, 2 }] * (*this)[{ 1, 0 }] * (*this)[{ 2, 1 }]
			+ (*this)[{ 0, 1 }] * (*this)[{ 1, 2 }] * (*this)[{ 2, 0 }]

			- (*this)[{ 0, 2 }] * (*this)[{ 1, 1 }] * (*this)[{ 2, 0 }]
			- (*this)[{ 0, 1 }] * (*this)[{ 1, 0 }] * (*this)[{ 2, 2 }]
			- (*this)[{ 0, 0 }] * (*this)[{ 1, 2 }] * (*this)[{ 2, 1 }];

			
	}
	else
	{
		float det = 0;

		Minor minor;
		minor.SetDimensionals(m_HorizontalDimensional - 1, m_HorizontalDimensional - 1);
		float sign = 1.f;
		for (uint32_t x = 0; x < m_HorizontalDimensional; x++, sign *= -1.f )
		{
			float elementValue = (*this)[{ 0, x }];
			float minorDet;

			if (!IsFZero(elementValue))
			{
				ExtractMinor(0, x, minor);
				float minorDet = sign * elementValue * minor.Det();
				det += minorDet;
			}
			else
				minorDet = 0.f;

			if (bottomDets)
			{
				bottomDets->push_back(minorDet);
			}

		}

		return det;
	}

	
}


Eigen::MatrixXf Matrix::AsEigenMatrix() const
{
	Eigen::MatrixXf m(m_VerticalDimensional, m_HorizontalDimensional);
	for (uint32_t y = 0; y < m_VerticalDimensional; ++y)
	{
		for (uint32_t x = 0; x < m_HorizontalDimensional; ++x)
		{
			m(y, x) = (*this)[{y, x}];
		}
	}
	return m;
}

std::string Matrix::AsStringRanked(RankMethod rankMethod /*= RankMethod::LeftTop*/) const
{
	vector<ElementIndex> linearlyIndependent;
	Rank(rankMethod, &linearlyIndependent);
	return ::RankedMatrixAsString(*this, linearlyIndependent);	
}

float Matrix::Trace() const
{
	if (m_HorizontalDimensional != m_VerticalDimensional)
		throw logic_error("Matrix::Trace m_HorizontalDimensional != m_VerticalDimensional");

	float result = 0.f;

	for (uint32_t y = 0; y < m_VerticalDimensional; ++y)
	{		
		result += (*this)[{y, y}];
	}

	return result;
}

std::vector<complexF> Matrix::Eigenvalues() const
{
	if (m_HorizontalDimensional != m_VerticalDimensional)
		throw logic_error("Matrix::Eigenvalues m_HorizontalDimensional != m_VerticalDimensional");

	vector<complexF> result;

	using namespace Eigen;

	
	EigenSolver<MatrixXf> solver(AsEigenMatrix());

	for (uint32_t c = 0, size = (uint32_t)solver.eigenvalues().size(); c < size; c++)
	{
		result.push_back(solver.eigenvalues()[c]);
	}	

	return result;
}

Vector Matrix::SingularValues() const
{
	Vector result;

	using namespace Eigen;
	
	JacobiSVD<MatrixXf> solver(AsEigenMatrix());

	for (uint32_t c = 0, size = (uint32_t)solver.singularValues().size(); c < size; c++)
	{
		result.push_back(solver.singularValues()[c]);
	}

	return result;
}
//
//std::vector<complexF> Matrix::Eigenvalues2() const
//{
//	if (m_HorizontalDimensional != m_VerticalDimensional)
//		throw logic_error("Matrix::Eigenvalues m_HorizontalDimensional != m_VerticalDimensional");
//
//	vector<complexF> result;
//	switch (m_HorizontalDimensional)
//	{
//		case 1:
//		{
//			result.emplace_back( m_Data[0], 0.f);
//		}
//		break;
//		case 2:
//		{
//			//lambda = 0.5f * A.trace() +- 0.5f * sqrt( sqr(A.trace()) - 4 * A.Det() ) )
//			
//			float trace = Trace();
//			float det = Det();
//						
//			complexF t(trace*trace - 4.f * det, 0.f);
//			t = sqrt(t);
//
//			complexF eigenvalue1 = 0.5f * (trace + t);
//			complexF eigenvalue2 = 0.5f * (trace - t);
//
//			result.push_back(eigenvalue1);
//			if (!FCmp( eigenvalue1,  eigenvalue2) )
//			{
//				result.push_back(eigenvalue2);
//			}
//		}
//		break;
//		case 3:
//		{
//			//y^3 + p*y + q = 0
//
//			//a*x^3 + b*x^2 + c*x + d = 0
//
//			//x = y - b / (3*a)
//			
//			//p = (3*a*c-b*b) / ( 3*a*a)
//
//			//q = (2*b^3) / ( 3*a*a)
//
//
//			//x^3 + a*x^2 + b*x + c = 0
//
//			///k3
//
//			Matrix A = *this;
//			float trace = A.Trace();
//
//			float a = -Trace();
//			float b = -0.5f*( (A*A).Trace() - trace*trace);
//			float c = -Det();
//
//			float Q = (a*a - 3*b) / 9;
//			float R = (2*a*a*a - 9*a*b + 27*c) / 54;
//			float S = Q*Q*Q-R*R;
//
//			float a_div_3 = a / 3.f;
//
//			if ( IsFZero(S) )
//			{				
//				complexF eigenvalue1( -2.f*cbrt(R) - a_div_3, 0.f);
//				complexF eigenvalue2( cbrt(R) - a_div_3, 0.f);
//
//				result.push_back(eigenvalue1);
//				result.push_back(eigenvalue2);
//			}
//			else
//			{
//				if (S > 0.0f)
//				{
//					float phi = (1.f/3.f)*acos( R / sqrt(Q*Q*Q) );
//					const float Pi = 3.1416f;
//
//					complexF eigenvalue1( -2.f * sqrt(Q) * cos(phi) - a_div_3, 0.f);
//					complexF eigenvalue2( -2.f * sqrt(Q) * cos(phi + (2.f / 3.f)*Pi) - a_div_3, 0.f);
//					complexF eigenvalue3( -2.f * sqrt(Q) * cos(phi - (2.f / 3.f)*Pi) - a_div_3, 0.f);
//
//					result.push_back(eigenvalue1);
//					result.push_back(eigenvalue2);
//					result.push_back(eigenvalue3);
//				}
//				else
//				{
//					if (IsFZero(Q))
//					{		
//						float x1 = -cbrt(c - a*a*a / 27.f) - a_div_3;
//						complexF eigenvalue1(x1, 0.f);
//
//						
//						float rePart = -0.5f*(a + x1);
//						float imPart = 0.5f* sqrt(std::abs( (a - 3.f* x1) * (a + x1) - 4.f*b) );
//
//						complexF eigenvalue2(rePart, imPart);
//						complexF eigenvalue3(rePart, -imPart);						
//
//						result.push_back(eigenvalue1);
//						result.push_back(eigenvalue2);
//						result.push_back(eigenvalue3);
//					}
//					else
//					{
//						if (Q > 0.0f)
//						{
//							float phi = (1.f / 3.f)*acosh( std::abs(R) / sqrt(Q*Q*Q));
//
//							complexF eigenvalue1(-2.f * sgn(R) * sqrt(Q) * cosh(phi) - a_div_3, 0.f);
//							complexF eigenvalue2(sgn(R) * sqrt(Q) * cosh(phi) - a_div_3,  sqrt(3.f)*sqrt(Q)*sinh(phi));
//							complexF eigenvalue3(eigenvalue2.real(), -eigenvalue2.imag());
//
//							result.push_back(eigenvalue1);
//							result.push_back(eigenvalue2);
//							result.push_back(eigenvalue3);
//						}
//						else
//						{
//							float phi = (1.f / 3.f)*asinh(std::abs(R) / sqrt( std::abs(Q*Q*Q) ) );
//
//							complexF eigenvalue1(-2.f * sgn(R) * sqrt(std::abs(Q)) * sinh(phi) - a_div_3, 0.f);
//							complexF eigenvalue2(sgn(R) * sqrt(std::abs(Q)) * sinh(phi) - a_div_3, sqrt(3.f)*sqrt(std::abs(Q))*cosh(phi));
//							complexF eigenvalue3(eigenvalue2.real(), -eigenvalue2.imag());
//
//							result.push_back(eigenvalue1);
//							result.push_back(eigenvalue2);
//							result.push_back(eigenvalue3);
//						}
//					}
//				}
//			}
//
//
//
//		}
//		break;
//		default:
//			throw logic_error("Matrix::Eigenvalues default: not implemented");
//		break;
//	}
//	return result;
//}

float Matrix::NormSpectral() const
{
	return AsEigenMatrix().operatorNorm();	
}

bool Matrix::IsStable() const
{
	auto eigenvalues = Eigenvalues();

	for (auto& eigenvalue : eigenvalues)
	{
		if (eigenvalue.real() <= 0.f)
			return false;
	}
	return true;
}

//
//void Matrix::Svd(Matrix& U, Matrix& Sigma, Matrix& V) const
//{
//
//}

uint32_t Matrix::RankInternal_Step1(vector<ElementIndex>* linearlyIndependent, uint32_t target_x /*= 0*/, uint32_t iteration /*= 0*/)
{
	uint32_t target_y = 0;

	for (; target_y < m_VerticalDimensional; target_y++)
	{
		if (IsFZero((*this)[{target_y, target_x}]))
			continue;
			
		auto row = GetRowAsVector(target_y);

		if ( all_of(row.begin(), row.begin() + target_x, &IsFZero) )
			break;
	}

	if (target_y < m_VerticalDimensional)
	{
		for (uint32_t y = 0; y < m_VerticalDimensional; ++y)
		{
			if (y == target_y)
				continue;

			if (IsFZero( (*this)[{y, target_x}] ))
				continue;

			float multiplier = (*this)[{y, target_x}] / (*this)[{target_y, target_x}];
			for (uint32_t x = target_x; x < m_HorizontalDimensional; ++x)
			{
				(*this)[{y, x}] -= multiplier *  (*this)[{target_y, x}];
			}
		}

		if (linearlyIndependent)
		{
			linearlyIndependent->push_back({ target_y, target_x });
		}
		++iteration;
		++target_x;

		if (iteration == std::min(m_VerticalDimensional, m_HorizontalDimensional))
		{
			return iteration;
		}
	}
	else
	{
		++target_x;
	}
	
	if (target_x == m_HorizontalDimensional)
	{
		return iteration;
	}

	return RankInternal_Step1(linearlyIndependent, target_x, iteration);
}



void Matrix::ForAllMatrixInternalRefill(Matrix& M, uint32_t target_idx, float beginValue)
{
	for (uint32_t idx = 0; idx < target_idx; ++idx)
	{
		M.m_Data[idx] = beginValue;		
	}
}

uint32_t Matrix::Rank(RankMethod rankMethod /*= RankMethod::LeftTop*/, vector<ElementIndex>* linearlyIndependent /*= nullptr*/) const
{
	if (m_VerticalDimensional == 0 || m_HorizontalDimensional == 0)
	{
		throw std::logic_error("Rank m_VerticalDimensional == 0 || m_HorizontalDimensional == 0");
	}

	if (m_VerticalDimensional == 1 || m_HorizontalDimensional == 1)
	{
		for (uint32_t y = 0; y < m_VerticalDimensional; ++y)
		{
			for (uint32_t x = 0; x < m_HorizontalDimensional; ++x)
			{
				if (!IsFZero((*this)[{y, x}]))
				{
					if (linearlyIndependent)
					{
						linearlyIndependent->push_back({ y, x });
					}

					return 1;
				}
			}
		}

		return 0;
	}


	float result = 0;

	Matrix t = *this;
	
	switch (rankMethod)
	{
		case RankMethod::LeftTop:

		break;
		case RankMethod::RightTop:
			t.FlipHorizontal();
		break;
		case RankMethod::RightBottom:
		{
			t.FlipHorizontal();
			t.FlipVertical();
		}
		break;
		case RankMethod::LeftBottom:
		{
			t.FlipVertical();
		}
		break;
		default:
			throw logic_error("RankMethod");
		break;
	}


	
	uint32_t rank = t.RankInternal_Step1(linearlyIndependent);



	if (linearlyIndependent)
	{
		switch (rankMethod)
		{
			case RankMethod::LeftTop:

			break;
			case RankMethod::RightTop:
			{
				for (auto& e : *linearlyIndependent)
				{					 
					e.second = m_HorizontalDimensional -1 - e.second;
				}
			}
			break;
			case RankMethod::RightBottom:
			{
				for (auto& e : *linearlyIndependent)
				{
					e.first = m_VerticalDimensional - 1 - e.first;
				}
				for (auto& e : *linearlyIndependent)
				{
					e.second = m_HorizontalDimensional - 1 - e.second;
				}				
			}
			break;
			case RankMethod::LeftBottom:
			{
				for (auto& e : *linearlyIndependent)
				{
					e.first = m_VerticalDimensional - 1 - e.first;
				}
			}	
			break;
			default:
			throw logic_error("RankMethod");
			break;
		}
	}

	return rank;
}



Vector Matrix::GetRowAsVector(uint32_t verticalIndex) const
{
	Vector result(m_HorizontalDimensional);

	for (uint32_t c = 0; c < m_HorizontalDimensional; c++)
	{
		result[c] = (*this)[{verticalIndex, c}];
	}
	return result;
}

Vector Matrix::GetColumnAsVector(uint32_t horizontalIndex) const
{
	Vector result(m_VerticalDimensional);
	for (uint32_t c = 0; c < m_VerticalDimensional; c++)
	{
		result[c] = (*this)[{c, horizontalIndex}];
	}
	return result;
}

void Matrix::ExtractMinor(uint32_t verticalIndex, uint32_t horizontalIndex, Minor& minor) const
{
	if (m_HorizontalDimensional != m_VerticalDimensional)
	{
		throw std::logic_error("GetMinor m_HorizontalDimensional != rhs.m_VerticalDimensional");
	}

	if (m_HorizontalDimensional < 2)
	{
		throw std::logic_error("GetMinor m_HorizontalDimensional < 2");
	}

	if (m_HorizontalDimensional <= verticalIndex || m_HorizontalDimensional <= horizontalIndex)
	{
		throw std::logic_error("GetMinor m_HorizontalDimensional <= verticalIndex || m_HorizontalDimensional <= horizontalIndex");
	}


	

	
	uint32_t dest_y = 0;

	for (uint32_t y = 0; y < m_VerticalDimensional; ++y)
	{	
		if (y == verticalIndex)
		{
			continue;
		}

		uint32_t dest_x = 0;

		for (uint32_t x = 0; x < m_HorizontalDimensional; ++x)
		{
			if (x == horizontalIndex)
			{				
				continue;
			}

			minor[{dest_y, dest_x}] = (*this)[{y, x}];

			++dest_x;
		}
		++dest_y;


	}
}

std::string Matrix::AsString() const
{
	std::string result = "{\n";

	for (uint32_t y = 0; y < m_VerticalDimensional; ++y)
	{
		for (uint32_t x = 0; x < m_HorizontalDimensional; ++x)
		{
			result += to_string( (*this)[{y, x}] ) + ", ";
		}

		result.pop_back();
		result.pop_back();
		result += "\n";
	}

	result += " }\n";

	return result;
}

Matrix Matrix::GetSubMatrix(uint32_t y, uint32_t x, uint32_t verticalDimensional, uint32_t horizontalDimensional) const
{
	if (y + verticalDimensional > m_VerticalDimensional)
	{
		throw std::logic_error("GetSubMatrix y + verticalDimensional > m_VerticalDimensional");
	}

	if (x + horizontalDimensional > m_HorizontalDimensional)
	{
		throw std::logic_error("GetSubMatrix x + horizontalDimensional > m_HorizontalDimensional");
	}

	Matrix result;
	result.SetDimensionals(verticalDimensional, horizontalDimensional);

	for (uint32_t c = 0; c < verticalDimensional; ++c)
	{
		Vector row = GetRowAsVector(y + c);

		auto src_begin = row.begin() + x;
		auto src_end = src_begin + horizontalDimensional;

		auto dest_begin = result.m_Data.begin() + horizontalDimensional * c;		
		std::copy(src_begin, src_end,	dest_begin);
		
	}
	return result;
}

Matrix& Matrix::GrowBottom(Matrix m)
{
	if (m_VerticalDimensional == 0 && m_HorizontalDimensional == 0)
	{
		return *this = m;
	}

	Matrix t = *this;

	if (m_HorizontalDimensional != m.m_HorizontalDimensional)
	{
		throw std::logic_error("GrowBottom m_HorizontalDimensional != m.m_HorizontalDimensional");
	}

	auto prevVerticalDimensional = m_VerticalDimensional;

	SetDimensionals(m_VerticalDimensional + m.m_VerticalDimensional, m_HorizontalDimensional);
	SetSubMatrix(0, 0, t);
	SetSubMatrix(prevVerticalDimensional,0,m);
	return *this;
}

Matrix& Matrix::GrowRight(Matrix m)
{
	if (m_VerticalDimensional == 0 && m_HorizontalDimensional == 0)
	{
		return *this = m;
	}

	Matrix t = *this;
	
	if (m_VerticalDimensional != m.m_VerticalDimensional)
	{
		throw std::logic_error("GrowRight m_VerticalDimensional != m.m_VerticalDimensional");
	}

	auto prevHorizontalDimensional = m_HorizontalDimensional;

	SetDimensionals(m_VerticalDimensional, m_HorizontalDimensional + m.m_HorizontalDimensional);
	SetSubMatrix(0, 0, t);
	SetSubMatrix(0, prevHorizontalDimensional, m);

	return *this;
}

Matrix Matrix::Transpose() const
{
	Matrix result;

	result.SetDimensionals(m_HorizontalDimensional, m_VerticalDimensional);

	for (uint32_t y = 0; y < m_VerticalDimensional; ++y)
	{
		for (uint32_t x = 0; x < m_HorizontalDimensional; ++x)
		{
			result[{x, y}] = (*this)[{y, x}];
		}
	}
	
	return result;
}

Matrix Matrix::Inverse() const
{
//	vector<float> bottomMinors;
	float det = Det(/*&bottomMinors*/);
	
	if (IsFZero(det))
	{
		throw logic_error("Inverse IsFZero(det)");
	}

		

	Matrix result(m_HorizontalDimensional, m_VerticalDimensional);
		
	//float sign = 1.f;
	//for (uint32_t x = 0; x < m_HorizontalDimensional; ++x, sign *= -1.f )
	//{
	//	result[{x, 0}] += sign * bottomMinors[x] / det;
	//}
	
	
	float signY = 1.f;

	Minor minor;
	minor.SetDimensionals(m_HorizontalDimensional - 1, m_HorizontalDimensional - 1);

	for (uint32_t y = 0; y < m_VerticalDimensional; ++y)
	{
		float signX = signY;
		for (uint32_t x = 0; x < m_HorizontalDimensional; ++x)
		{
			ExtractMinor(y, x, minor);
			
			result[{x, y}] += signX * minor.Det() / det;
			signX *= -1.f;
		}

		signY *= -1.f;
	}
		

	return result;
}

void Matrix::FlipHorizontal()
{
	Matrix T = *this;

	for (uint32_t x = 0; x < m_HorizontalDimensional; ++x)
	{
		Matrix sub = T.GetSubMatrix( 0, x, m_VerticalDimensional, 1 );
		SetSubMatrix(0, m_HorizontalDimensional - 1 - x , sub);
	}

}

void Matrix::FlipVertical()
{
	Matrix T = *this;

	for (uint32_t y = 0; y < m_VerticalDimensional; ++y)
	{
		Matrix sub = T.GetSubMatrix(y, 0, 1, m_HorizontalDimensional);
		SetSubMatrix(m_VerticalDimensional - 1 - y, 0, sub);
	}
}

void Matrix::ExcludeColumnsExcept(const vector<ElementIndex>& elements)
{
	Matrix T = *this;

	auto prev = m_HorizontalDimensional;

	SetDimensionals(0,0);
	
	for (uint32_t x = 0; x < prev; ++x)
	{
		if (std::find_if(elements.begin(), elements.end(),
		
			[x](const ElementIndex& idx) -> bool
			{
				return idx.second == x;
			}) == elements.end()			)
		{
			continue;
		}

		auto column = T.GetSubMatrix(0,x, T.m_VerticalDimensional, 1);
				
		this->GrowRight(column);
	}
}

bool Matrix::IsZeroMatrix() const
{
	return all_of(m_Data.begin(), m_Data.end(), &IsFZero);
}

uint32_t Matrix::FindColumn(const Matrix& srcColumn, uint32_t offset_x /*= 0*/) const
{
	if (offset_x >= m_HorizontalDimensional)
		throw logic_error("Matrix::FindColumn offset_x >= m_HorizontalDimensional");
	
	for (uint32_t x = offset_x; x < m_HorizontalDimensional; x++)
	{
		Matrix column = GetSubMatrix(0, x, m_VerticalDimensional, 1);

		if (srcColumn == column)
			return x;
	}
	return npos;
}

bool Matrix::IsIdentityMatrix() const
{
	for (uint32_t y = 0; y < m_VerticalDimensional; ++y)
	{
		for (uint32_t x = 0; x < m_HorizontalDimensional; ++x)
		{
			if (y == x)
			{
				if (!FCmp((*this)[{y, x}], 1.0f))
					return false;
			}
			else
			{
				if (!IsFZero((*this)[{y, x}]))
					return false;
			}
		}
	}
	return true;
}

