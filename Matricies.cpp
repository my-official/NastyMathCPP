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

Matrix Matrix::operator*(const Matrix& rhs)
{
	if (m_HorizontalDimensional != rhs.m_VerticalDimensional)
	{
		throw std::logic_error("m_HorizontalDimensional != rhs.m_VerticalDimensional");
	}

	Matrix result;
	result.SetDimensionals(rhs.m_HorizontalDimensional, m_VerticalDimensional);

	
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
	m_Data.resize(horizontal * vertical, 0.0f);
	m_HorizontalDimensional = horizontal;
	m_VerticalDimensional = vertical;
}

float Matrix::Det() const
{
	if (m_HorizontalDimensional != m_VerticalDimensional)
	{
		throw std::logic_error("Det m_HorizontalDimensional != rhs.m_VerticalDimensional");
	}

	if (m_HorizontalDimensional < 2)
	{
		throw std::logic_error("Det m_HorizontalDimensional < 2");
	}

	float det = 0;

	if (m_HorizontalDimensional == 2)
	{
		det = ( (*this)[{ 0,0 }] * (*this)[{ 1,1 }] ) - ( (*this)[{ 0,1 }] * (*this)[{ 1,0 }] );
	}
	else
	{
		float sign = 1.f;
		for (uint32_t c = 0; c < m_HorizontalDimensional; c++, sign *= -1.f )
		{
			Minor minor = GetMinor(0, c);								

			det += sign * (*this)[{ 0, c }] * minor.Det();
		}
	}

	return det;
}

float Matrix::Rank(vector<ElementIndex>* linearlyIndependent /*= nullptr*/) const
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
				if (!FCMP((*this)[{y, x}], 0.f))
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


	uint32_t target_x = 0;
	uint32_t iteration = 0;

	Matrix t = *this;
	
	t.RankInternal_Step1(target_x, iteration);
	
	Matrix sub = t.GetSubMatrix( 0, 0 , target_x, std::min(m_VerticalDimensional, target_x) );

	////TODO

			
	return result;
}


void Matrix::RankInternal_Step1(uint32_t& target_x, uint32_t& iteration)
{
	uint32_t target_y = 0;

	for (; target_y < m_VerticalDimensional; target_y++)
	{
		if (FZero((*this)[{target_y, target_x}]))
			continue;
			
		auto row = GetRowAsVector(target_y);

		if ( all_of(row.begin(), row.begin() + iteration, &FZero) )
			break;
	}

	if (target_y < m_VerticalDimensional)
	{
		for (uint32_t y = 0; y < m_VerticalDimensional; ++y)
		{
			if (y == target_y)
				continue;

			if (FZero(t[{y, target_x}]))
				continue;

			float multiplier = t[{y, iteration}] / t[{target_y, iteration}];
			for (uint32_t x = 0; x < m_HorizontalDimensional; ++x)
			{
				t[{y, x}] -= multiplier *  t[{target_y, x}];
			}
		}

		++iteration;
		++target_x;

		if (iteration == std::min(m_VerticalDimensional, m_HorizontalDimensional))
		{
			return;
		}
	}
	else
	{
		++target_x;
	}
	
	if (target_x == m_HorizontalDimensional)
	{
		return;
	}

	RankInternal_Step1(target_x, iteration);
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

Matrix Matrix::GetMinor(uint32_t verticalIndex, uint32_t horizontalIndex) const
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


	Matrix result;
	result.SetDimensionals(m_VerticalDimensional - 1, m_HorizontalDimensional - 1);

	
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

			result[{dest_y, dest_x}] = (*this)[{y, x}];

			++dest_x;
		}
		++dest_y;


	}

	return result;
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

	result += " }";

	return result;
}


