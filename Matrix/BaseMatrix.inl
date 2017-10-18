#pragma once


template <typename ElementType, class DerivedMatrixT, class IsEqual, class IsEqualZero>
void BaseMatrix<ElementType,DerivedMatrixT,IsEqual,IsEqualZero>::SetSubMatrix(uint32_t y, int32_t x, const DerivedMatrixT& m)
{
	if (y + m.m_VerticalDimensional > m_VerticalDimensional)
	{
		throw EXCEPTION;
	}

	if (x + m.m_HorizontalDimensional > m_HorizontalDimensional)
	{
		throw EXCEPTION;
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


template <typename ElementType, class DerivedMatrixT, class IsEqual, class IsEqualZero>
BaseMatrix<ElementType,DerivedMatrixT,IsEqual,IsEqualZero>::BaseMatrix(uint32_t vertical, uint32_t horizontal)
{
	SetDimensionals(vertical, horizontal);
	for (auto& e : m_Data)
	{
		e = 0.f;
	}
}


template <typename ElementType, class DerivedMatrixT, class IsEqual, class IsEqualZero>
BaseMatrix<ElementType,DerivedMatrixT,IsEqual,IsEqualZero>::BaseMatrix(initializer_list<ElementType> il)
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
		throw EXCEPTION;
	}


	SetDimensionals(dim, dim);

	ElementType* data = &m_Data[0];

	for (auto it = il.begin(); it != il.end(); ++it, ++data)
	{
		*data = *it;
	}

}
template <typename ElementType, class DerivedMatrixT, class IsEqual, class IsEqualZero>
BaseMatrix<ElementType,DerivedMatrixT,IsEqual,IsEqualZero>::BaseMatrix(uint32_t vertical, uint32_t horizontal, initializer_list<ElementType> il)
{
	if (vertical * horizontal != il.size())
	{
		throw EXCEPTION;
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
template <typename ElementType, class DerivedMatrixT, class IsEqual, class IsEqualZero>
BaseMatrix<ElementType,DerivedMatrixT,IsEqual,IsEqualZero>::BaseMatrix(initializer_list<DerivedMatrixT> il)
{
	uint32_t blockDim = uint32_t(sqrt(il.size()));
	
	vector<uint32_t> verticalDims;
	uint32_t verticalDim = 0;
	vector<uint32_t> horizontalDims;
	uint32_t horizontalDim = 0;

	for (uint32_t block_y = 0; block_y < blockDim; ++block_y)
	{
		const DerivedMatrixT& m = *(il.begin() + block_y * blockDim);
		verticalDims.push_back(m.m_VerticalDimensional);
		verticalDim += m.m_VerticalDimensional;
	}

	for (uint32_t block_x = 0; block_x < blockDim; ++block_x)
	{
		const DerivedMatrixT& m = *(il.begin() + block_x);
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
			const DerivedMatrixT& m = *(il.begin() + block_y * blockDim + block_x);


			uint32_t blockHorizontal = horizontalDims[block_x];

			if (m.m_VerticalDimensional != blockVertical)
			{
				throw EXCEPTION;
			}

			if (m.m_HorizontalDimensional != blockHorizontal)
			{
				throw EXCEPTION;
			}


			SetSubMatrix(y, x, m);

			x += blockHorizontal;
		}
		y += blockVertical;
	}


}

template <typename ElementType, class DerivedMatrixT, class IsEqual, class IsEqualZero>
BaseMatrix<ElementType,DerivedMatrixT,IsEqual,IsEqualZero>::BaseMatrix(uint32_t vertical, uint32_t horizontal, initializer_list<DerivedMatrixT> il)
{
	vector<uint32_t> verticalDims;
	uint32_t verticalDim = 0;
	vector<uint32_t> horizontalDims;
	uint32_t horizontalDim = 0;

	for (uint32_t block_y = 0; block_y < vertical; ++block_y)
	{
		const DerivedMatrixT& m = *(il.begin() + block_y * horizontal);
		verticalDims.push_back(m.m_VerticalDimensional);
		verticalDim += m.m_VerticalDimensional;
	}

	for (uint32_t block_x = 0; block_x < horizontal; ++block_x)
	{
		const DerivedMatrixT& m = *(il.begin() + block_x);
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
			const DerivedMatrixT& m = *(il.begin() + block_y * horizontal + block_x);


			uint32_t blockHorizontal = horizontalDims[block_x];

			if (m.m_VerticalDimensional != blockVertical)
			{
				throw EXCEPTION;
			}

			if (m.m_HorizontalDimensional != blockHorizontal)
			{
				throw EXCEPTION;
			}


			SetSubMatrix(y, x, m);

			x += blockHorizontal;
		}
		y += blockVertical;
	}
}



template <typename ElementType, class DerivedMatrixT, class IsEqual, class IsEqualZero>
bool BaseMatrix<ElementType,DerivedMatrixT,IsEqual,IsEqualZero>::operator==(const DerivedMatrixT& rhs) const
{
	if (m_HorizontalDimensional != rhs.m_HorizontalDimensional || m_VerticalDimensional != rhs.m_VerticalDimensional)
	{
		if ((m_HorizontalDimensional == m_VerticalDimensional && m_HorizontalDimensional == 0) ||
			(rhs.m_HorizontalDimensional == rhs.m_VerticalDimensional && rhs.m_HorizontalDimensional == 0) )
		{
			return false;
		}
		else
		{
			throw EXCEPTION;
		}
	}
	else
	{
		return std::equal(m_Data.begin(), m_Data.end(), rhs.m_Data.begin(), m_IsEqual);
	}
}


template <typename ElementType, class DerivedMatrixT, class IsEqual, class IsEqualZero>
DerivedMatrixT BaseMatrix<ElementType,DerivedMatrixT,IsEqual,IsEqualZero>::operator-() const
{
	DerivedMatrixT result = static_cast<const DerivedMatrixT&>(*this);
	
	for (auto& e : result.m_Data)
	{
		e = -e;
	}
	return result;
}

template <typename ElementType, class DerivedMatrixT, class IsEqual, class IsEqualZero>
DerivedMatrixT& BaseMatrix<ElementType,DerivedMatrixT,IsEqual,IsEqualZero>::operator+=(const DerivedMatrixT& rhs)
{
	if (m_HorizontalDimensional != rhs.m_HorizontalDimensional || m_VerticalDimensional != rhs.m_VerticalDimensional)
	{
		if (m_HorizontalDimensional == m_VerticalDimensional && m_HorizontalDimensional == 0)
		{
			*this = rhs;
			return static_cast<DerivedMatrixT&>(*this);
		}
		else
		{
			throw EXCEPTION;
		}
	}
		
	for (uint32_t c = 0, size = (uint32_t)m_Data.size(); c < size; c++)
	{
		this->m_Data[c] += rhs.m_Data[c];
	}
			
	return static_cast<DerivedMatrixT&>(*this);
}

template <typename ElementType, class DerivedMatrixT, class IsEqual, class IsEqualZero>
DerivedMatrixT& BaseMatrix<ElementType,DerivedMatrixT,IsEqual,IsEqualZero>::operator-=(const DerivedMatrixT& rhs)
{
	if (m_HorizontalDimensional != rhs.m_HorizontalDimensional || m_VerticalDimensional != rhs.m_VerticalDimensional)
	{
		if (m_HorizontalDimensional == m_VerticalDimensional && m_HorizontalDimensional == 0)
		{			
			*this = -rhs;
			return static_cast<DerivedMatrixT&>(*this);
		}
		else
		{
			throw EXCEPTION;
		}
	}
		
	for (uint32_t c = 0, size = (uint32_t)m_Data.size(); c < size; c++)
	{
		this->m_Data[c] -= rhs.m_Data[c];
	}
	return static_cast<DerivedMatrixT&>(*this);
}

template <typename ElementType, class DerivedMatrixT, class IsEqual, class IsEqualZero>
DerivedMatrixT& BaseMatrix<ElementType,DerivedMatrixT,IsEqual,IsEqualZero>::operator*=(const DerivedMatrixT& rhs)
{
	if ( (m_HorizontalDimensional == 0 && m_VerticalDimensional == 0) || (rhs.m_HorizontalDimensional == 0 && rhs.m_VerticalDimensional == 0) )
	{
		if (m_HorizontalDimensional == 0 && m_VerticalDimensional == 0)
		{
			*this = ZeroMatrix(rhs);
			return static_cast<DerivedMatrixT&>(*this);
		}
		else
		{			
			*this = ZeroMatrix(*this);
			return static_cast<DerivedMatrixT&>(*this);			
		}			
	}

	if (m_HorizontalDimensional != rhs.m_VerticalDimensional)
		throw EXCEPTION;

	DerivedMatrixT result;
	result.SetDimensionals(m_VerticalDimensional, rhs.m_HorizontalDimensional);


	for (uint32_t y = 0; y < m_VerticalDimensional; ++y)
	{
		Vector<ElementType> row = GetRowAsVector(y);
		for (uint32_t x = 0; x < rhs.m_HorizontalDimensional; ++x)
		{
			Vector<ElementType> column = rhs.GetColumnAsVector(x);

			ElementType value = DotProduct(row, column);

			result[{y, x}] = value;
		}
	}

	*this = move(result);

	return static_cast<DerivedMatrixT&>(*this);
}



template <typename ElementType, class DerivedMatrixT, class IsEqual, class IsEqualZero>
DerivedMatrixT& BaseMatrix<ElementType, DerivedMatrixT, IsEqual, IsEqualZero>::operator/=(const DerivedMatrixT& rhs)
{
	auto rhsInv = rhs.Inverse();
	(*this) *= rhsInv;
	return static_cast<DerivedMatrixT&>(*this);
}


template <typename ElementType, class DerivedMatrixT, class IsEqual, class IsEqualZero>
DerivedMatrixT& BaseMatrix<ElementType, DerivedMatrixT, IsEqual, IsEqualZero>::Scale(const ElementType& rhs)
{
	for (auto& e : m_Data)
	{
		e *= rhs;
	}
	return static_cast<DerivedMatrixT&>(*this);
}


template <typename ElementType, class DerivedMatrixT, class IsEqual, class IsEqualZero>
ElementType& BaseMatrix<ElementType,DerivedMatrixT,IsEqual,IsEqualZero>::operator[](ElementIndex idx)
{
	return m_Data[idx.first * m_HorizontalDimensional + idx.second];
}
template <typename ElementType, class DerivedMatrixT, class IsEqual, class IsEqualZero>
ElementType BaseMatrix<ElementType,DerivedMatrixT,IsEqual,IsEqualZero>::operator[](ElementIndex idx) const
{
	return m_Data[idx.first * m_HorizontalDimensional + idx.second];
}


template <typename ElementType, class DerivedMatrixT, class IsEqual, class IsEqualZero>
void BaseMatrix<ElementType,DerivedMatrixT,IsEqual,IsEqualZero>::SetDimensionals(uint32_t vertical, uint32_t horizontal)
{
	m_Data.resize(horizontal * vertical, 0.0f);
	m_HorizontalDimensional = horizontal;
	m_VerticalDimensional = vertical;
}




template <typename ElementType, class DerivedMatrixT, class IsEqual, class IsEqualZero>
DerivedMatrixT& BaseMatrix<ElementType,DerivedMatrixT,IsEqual,IsEqualZero>::GrowBottom(DerivedMatrixT m)

{
	if (m_VerticalDimensional == 0 && m_HorizontalDimensional == 0)
	{
		*this = m;
		return static_cast<DerivedMatrixT&>(*this);
	}

	DerivedMatrixT t = static_cast<const DerivedMatrixT&>(*this);

	if (m_HorizontalDimensional != m.m_HorizontalDimensional)
	{
		throw EXCEPTION;
	}

	auto prevVerticalDimensional = m_VerticalDimensional;

	SetDimensionals(m_VerticalDimensional + m.m_VerticalDimensional, m_HorizontalDimensional);
	SetSubMatrix(0, 0, t);
	SetSubMatrix(prevVerticalDimensional, 0, m);
	return static_cast<DerivedMatrixT&>(*this);
}



template <typename ElementType, class DerivedMatrixT, class IsEqual, class IsEqualZero>
DerivedMatrixT& BaseMatrix<ElementType,DerivedMatrixT,IsEqual,IsEqualZero>::GrowRight(DerivedMatrixT m)

{
	if (m_VerticalDimensional == 0 && m_HorizontalDimensional == 0)
	{
		*this = m;
		return static_cast<DerivedMatrixT&>(*this);
	}

	DerivedMatrixT t = static_cast<const DerivedMatrixT&>(*this);

	if (m_VerticalDimensional != m.m_VerticalDimensional)
	{
		throw EXCEPTION;
	}

	auto prevHorizontalDimensional = m_HorizontalDimensional;

	SetDimensionals(m_VerticalDimensional, m_HorizontalDimensional + m.m_HorizontalDimensional);
	SetSubMatrix(0, 0, t);
	SetSubMatrix(0, prevHorizontalDimensional, m);

	return static_cast<DerivedMatrixT&>(*this);
}

template <typename ElementType, class DerivedMatrixT, class IsEqual, class IsEqualZero>
void BaseMatrix<ElementType,DerivedMatrixT,IsEqual,IsEqualZero>::FlipHorizontal()
{
	DerivedMatrixT T = static_cast<const DerivedMatrixT&>(*this);

	for (uint32_t x = 0; x < m_HorizontalDimensional; ++x)
	{
		DerivedMatrixT sub = T.GetSubMatrix(0, x, m_VerticalDimensional, 1);
		SetSubMatrix(0, m_HorizontalDimensional - 1 - x, sub);
	}
}

template <typename ElementType, class DerivedMatrixT, class IsEqual, class IsEqualZero>
void BaseMatrix<ElementType,DerivedMatrixT,IsEqual,IsEqualZero>::FlipVertical()
{
	DerivedMatrixT T = static_cast<const DerivedMatrixT&>(*this);

	for (uint32_t y = 0; y < m_VerticalDimensional; ++y)
	{
		DerivedMatrixT sub = T.GetSubMatrix(y, 0, 1, m_HorizontalDimensional);
		SetSubMatrix(m_VerticalDimensional - 1 - y, 0, sub);
	}
}



template <typename ElementType, class DerivedMatrixT, class IsEqual, class IsEqualZero>
void BaseMatrix<ElementType,DerivedMatrixT,IsEqual,IsEqualZero>::ExcludeColumnsExcept(const vector<ElementIndex>& elements)
{
	DerivedMatrixT T = static_cast<const DerivedMatrixT&>(*this);

	auto prev = m_HorizontalDimensional;

	SetDimensionals(0, 0);

	for (uint32_t x = 0; x < prev; ++x)
	{
		if (std::find_if(elements.begin(), elements.end(),

			[x](const ElementIndex& idx) -> bool
		{
			return idx.second == x;
		}) == elements.end())
		{
			continue;
		}

		auto column = T.GetSubMatrix(0, x, T.m_VerticalDimensional, 1);

		this->GrowRight(column);
	}
}

template <typename ElementType, class DerivedMatrixT, class IsEqual, class IsEqualZero>
void BaseMatrix<ElementType, DerivedMatrixT, IsEqual, IsEqualZero>::RankInternal_Step1()
{
	for (uint32_t target_x = 0; target_x < m_HorizontalDimensional; ++target_x)
	{
		uint32_t target_y = 0;

		for (; target_y < m_VerticalDimensional; target_y++)
		{
			if (m_IsEqualZero((*this)[{target_y, target_x}]))
				continue;

			auto row = GetRowAsVector(target_y);

			if (all_of(row.begin(), row.begin() + target_x, m_IsEqualZero))
				break;
		}

		if (target_y == m_VerticalDimensional)
			continue;

		for (uint32_t y = 0; y < m_VerticalDimensional; ++y)
		{
			if (y == target_y)
				continue;

			if (m_IsEqualZero((*this)[{y, target_x}]))
				continue;

			ElementType multiplier = (*this)[{y, target_x}] / (*this)[{target_y, target_x}];
			for (uint32_t x = target_x; x < m_HorizontalDimensional; ++x)
			{
				(*this)[{y, x}] -= multiplier *  (*this)[{target_y, x}];
			}
		}
	}
}

template <typename ElementType, class DerivedMatrixT, class IsEqual, class IsEqualZero>
uint32_t BaseMatrix<ElementType, DerivedMatrixT, IsEqual, IsEqualZero>::RankInternal_Step2(vector<ElementIndex>* linearlyIndependent)
{
	for (uint32_t target_y = 0; target_y < m_VerticalDimensional; ++target_y)
	{
		uint32_t target_x = 0;

		for (; target_x < m_HorizontalDimensional; target_x++)
		{
			if (m_IsEqualZero((*this)[{target_y, target_x}]))
				continue;

			auto column = GetColumnAsVector(target_x);

			if (all_of(column.begin(), column.begin() + target_y, m_IsEqualZero))
				break;
		}
		

		if (target_x == m_HorizontalDimensional)
			continue;

		for (uint32_t x = 0; x < m_HorizontalDimensional; ++x)
		{
			if (x == target_x)
				continue;

			if (m_IsEqualZero((*this)[{target_y, x}]))
				continue;

			ElementType multiplier = (*this)[{target_y, x}] / (*this)[{target_y, target_x}];
			for (uint32_t y = target_y; y < m_VerticalDimensional; ++y)
			{
				(*this)[{y, x}] -= multiplier *  (*this)[{y, target_x}];
			}
		}

		if (linearlyIndependent)
		{
			linearlyIndependent->push_back({ target_y, target_x });
		}
		
	}

	uint32_t rank = 0;	
	for (uint32_t y = 0; y < m_VerticalDimensional; ++y)
	{
		for (uint32_t x = 0; x < m_HorizontalDimensional; ++x)
		{
			if (m_IsEqualZero((*this)[{y, x}]))
				continue;

			++rank;
		}
	}

	return rank;
}

template <typename ElementType, class DerivedMatrixT, class IsEqual, class IsEqualZero>
std::string BaseMatrix<ElementType,DerivedMatrixT,IsEqual,IsEqualZero>::AsString() const
{
	std::string result = "{\n";

	for (uint32_t y = 0; y < m_VerticalDimensional; ++y)
	{
		for (uint32_t x = 0; x < m_HorizontalDimensional; ++x)
		{
			result += to_string((*this)[{y, x}]) + ", ";
		}

		result.pop_back();
		result.pop_back();
		result += "\n";
	}

	result += " }\n";

	return result;
}

template <typename ElementType,	class DerivedMatrixT,	class IsEqual /*= equal_to<ElementType>*/,	class IsEqualZero /*= equal_to_zero<ElementType> */>
std::string BaseMatrix<ElementType, DerivedMatrixT, IsEqual, IsEqualZero>::AsLatexCode() const
{
	std::string result;
	result += "\\left( \\begin{array}{" + string(m_HorizontalDimensional, 'c') + "}\n";

	for (uint32_t y = 0; y < m_VerticalDimensional; ++y)
	{
		for (uint32_t x = 0; x < m_HorizontalDimensional; ++x)
		{
			result += to_latex((*this)[{y, x}]) +	"&"; 			
		}

		result.pop_back();		
		result += "\\\\\n";
	}

	result += "\\end{array}\\right)";	

	return result;
}


template <typename ElementType, class DerivedMatrixT, class IsEqual, class IsEqualZero>
DerivedMatrixT BaseMatrix<ElementType,DerivedMatrixT,IsEqual,IsEqualZero>::Inverse() const
{
	//	vector<ElementType> bottomMinors;
	ElementType det = Det(/*&bottomMinors*/);

	if (m_IsEqualZero(det))
	{
		throw EXCEPTION;
	}
	

	DerivedMatrixT result(m_HorizontalDimensional, m_VerticalDimensional);

	if (m_HorizontalDimensional == m_VerticalDimensional && m_VerticalDimensional == 1)
	{
		result[{0, 0}] = 1.f / this->operator[]({ 0,0 });
		return result;
	}

	//ElementType sign = 1.f;
	//for (uint32_t x = 0; x < m_HorizontalDimensional; ++x, sign *= -1.f )
	//{
	//	result[{x, 0}] += sign * bottomMinors[x] / det;
	//}


	ElementType signY = 1.f;
	
	DerivedMatrixT minor;
	minor.SetDimensionals(m_HorizontalDimensional - 1, m_HorizontalDimensional - 1);

	for (uint32_t y = 0; y < m_VerticalDimensional; ++y)
	{
		ElementType signX = signY;
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

template <typename ElementType, class DerivedMatrixT, class IsEqual, class IsEqualZero>
DerivedMatrixT BaseMatrix<ElementType,DerivedMatrixT,IsEqual,IsEqualZero>::Transpose() const

{
	DerivedMatrixT result;

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

template <typename ElementType, class DerivedMatrixT, class IsEqual, class IsEqualZero>
ElementType BaseMatrix<ElementType,DerivedMatrixT,IsEqual,IsEqualZero>::Trace() const
{
	if (m_HorizontalDimensional != m_VerticalDimensional)
		throw EXCEPTION;

	ElementType result = 0.f;

	for (uint32_t y = 0; y < m_VerticalDimensional; ++y)
	{
		result += (*this)[{y, y}];
	}

	return result;
}

template <typename ElementType, class DerivedMatrixT, class IsEqual, class IsEqualZero>
std::string BaseMatrix<ElementType,DerivedMatrixT,IsEqual,IsEqualZero>::AsStringRanked(RankMethod rankMethod /*= RankMethod::LeftTop*/) const
{
	vector<ElementIndex> linearlyIndependent;
	Rank(rankMethod, &linearlyIndependent);

	
	std::string result = "{\n";

	for (uint32_t y = 0; y < m_VerticalDimensional; ++y)
	{
		for (uint32_t x = 0; x < m_HorizontalDimensional; ++x)
		{
			if (std::find(linearlyIndependent.begin(), linearlyIndependent.end(), make_pair(y, x)) != linearlyIndependent.end())
			{
				result += to_string((*this)[{y, x}]) + ", ";
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


template <typename ElementType,
	class DerivedMatrixT,
	class IsEqual /*= equal_to<ElementType>*/,
	class IsEqualZero /*= equal_to_zero<ElementType> */>
	std::string BaseMatrix<ElementType, DerivedMatrixT, IsEqual, IsEqualZero>::AsLatexCodeRanked(RankMethod rankMethod /*= RankMethod::LeftTop*/) const
{
	vector<ElementIndex> linearlyIndependent;
	Rank(rankMethod, &linearlyIndependent);
	

	std::string result;
	result += "\\left( \\begin{array}{" + string(m_HorizontalDimensional, 'c') + "}\n";

	for (uint32_t y = 0; y < m_VerticalDimensional; ++y)
	{
		for (uint32_t x = 0; x < m_HorizontalDimensional; ++x)
		{
			if (std::find(linearlyIndependent.begin(), linearlyIndependent.end(), make_pair(y, x)) != linearlyIndependent.end())
			{
				
				result += "\\textbf{" + to_latex((*this)[{y, x}]) + "}&";
			}
			else
			{
				result += " &";				
			}			
		}

		result.pop_back();
		result += "\\\\\n";
	}

	result += "\\end{array}\\right)";

	return result;
}

template <typename ElementType, class DerivedMatrixT, class IsEqual, class IsEqualZero>
uint32_t BaseMatrix<ElementType,DerivedMatrixT,IsEqual,IsEqualZero>::Rank(RankMethod rankMethod /*= RankMethod::LeftTop*/, vector<ElementIndex>* linearlyIndependent /*= nullptr*/) const
{
	if (m_VerticalDimensional == 0 || m_HorizontalDimensional == 0)
	{
		throw EXCEPTION;
	}

	if (m_VerticalDimensional == 1 || m_HorizontalDimensional == 1)
	{
		for (uint32_t y = 0; y < m_VerticalDimensional; ++y)
		{
			for (uint32_t x = 0; x < m_HorizontalDimensional; ++x)
			{
				if (!m_IsEqualZero((*this)[{y, x}]))
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


	ElementType result = 0;

	DerivedMatrixT t = static_cast<const DerivedMatrixT&>(*this);

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
		throw EXCEPTION;
		break;
	}
			

	t.RankInternal_Step1();
	uint32_t rank = t.RankInternal_Step2(linearlyIndependent);


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
					e.second = m_HorizontalDimensional - 1 - e.second;
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
			throw EXCEPTION;
			break;
		}
	}

	return rank;
}

template <typename ElementType, class DerivedMatrixT, class IsEqual, class IsEqualZero>
ElementType BaseMatrix<ElementType,DerivedMatrixT,IsEqual,IsEqualZero>::Det() const
{
	if (m_HorizontalDimensional != m_VerticalDimensional || m_HorizontalDimensional == 0)
	{
		throw EXCEPTION;
	}

	if (m_HorizontalDimensional == 1)
	{
		return m_Data[0];
	}


	if (m_HorizontalDimensional == 2)
	{
		return ((*this)[{ 0, 0 }] * (*this)[{ 1, 1 }]) - ((*this)[{ 0, 1 }] * (*this)[{ 1, 0 }]);
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
		ElementType det = 0;

		DerivedMatrixT minor;
		minor.SetDimensionals(m_HorizontalDimensional - 1, m_HorizontalDimensional - 1);
		ElementType sign = 1.f;
		for (uint32_t x = 0; x < m_HorizontalDimensional; x++, sign *= -1.f)
		{
			ElementType elementValue = (*this)[{ 0, x }];
			ElementType minorDet;

			if (!m_IsEqualZero(elementValue))
			{
				ExtractMinor(0, x, minor);
				ElementType minorDet = sign * elementValue * minor.Det();
				det += minorDet;
			}
			else
				minorDet = 0.f;

		}

		return det;
	}
}

template <typename ElementType, class DerivedMatrixT, class IsEqual, class IsEqualZero>
void BaseMatrix<ElementType,DerivedMatrixT,IsEqual,IsEqualZero>::ExtractMinor(uint32_t verticalIndex, uint32_t horizontalIndex, DerivedMatrixT& minor) const
{
	if (m_HorizontalDimensional != m_VerticalDimensional)
	{
		throw EXCEPTION;
	}

	if (m_HorizontalDimensional < 2)
	{
		throw EXCEPTION;
	}

	if (m_HorizontalDimensional <= verticalIndex || m_HorizontalDimensional <= horizontalIndex)
	{
		throw EXCEPTION;
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

template <typename ElementType, class DerivedMatrixT, class IsEqual, class IsEqualZero>
Vector<ElementType> BaseMatrix<ElementType,DerivedMatrixT,IsEqual,IsEqualZero>::GetColumnAsVector(uint32_t horizontalIndex) const
{
	Vector<ElementType> result(m_VerticalDimensional);
	for (uint32_t c = 0; c < m_VerticalDimensional; c++)
	{
		result[c] = (*this)[{c, horizontalIndex}];
	}
	return result;
}

template <typename ElementType, class DerivedMatrixT, class IsEqual, class IsEqualZero>
Vector<ElementType> BaseMatrix<ElementType,DerivedMatrixT,IsEqual,IsEqualZero>::GetRowAsVector(uint32_t verticalIndex) const
{
	Vector<ElementType> result(m_HorizontalDimensional);

	for (uint32_t c = 0; c < m_HorizontalDimensional; c++)
	{
		result[c] = (*this)[{verticalIndex, c}];
	}
	return result;
}

template <typename ElementType, class DerivedMatrixT, class IsEqual, class IsEqualZero>
DerivedMatrixT BaseMatrix<ElementType,DerivedMatrixT,IsEqual,IsEqualZero>::GetSubMatrix(uint32_t y, uint32_t x, uint32_t verticalDimensional, uint32_t horizontalDimensional) const

{
	if (y + verticalDimensional > m_VerticalDimensional)
	{
		throw EXCEPTION;
	}

	if (x + horizontalDimensional > m_HorizontalDimensional)
	{
		throw EXCEPTION;
	}

	DerivedMatrixT result;
	result.SetDimensionals(verticalDimensional, horizontalDimensional);

	for (uint32_t c = 0; c < verticalDimensional; ++c)
	{
		Vector<ElementType> row = GetRowAsVector(y + c);

		auto src_begin = row.begin() + x;
		auto src_end = src_begin + horizontalDimensional;

		auto dest_begin = result.m_Data.begin() + horizontalDimensional * c;
		std::copy(src_begin, src_end, dest_begin);

	}
	return result;
}

template <typename ElementType, class DerivedMatrixT, class IsEqual, class IsEqualZero>
uint32_t BaseMatrix<ElementType,DerivedMatrixT,IsEqual,IsEqualZero>::FindColumn(const DerivedMatrixT& srcColumn, uint32_t offset_x /*= 0*/) const
{
	if (offset_x >= m_HorizontalDimensional)
		throw EXCEPTION;

	for (uint32_t x = offset_x; x < m_HorizontalDimensional; x++)
	{
		DerivedMatrixT column = GetSubMatrix(0, x, m_VerticalDimensional, 1);

		if (srcColumn == column)
			return x;
	}
	return npos;
}

template <typename ElementType, class DerivedMatrixT, class IsEqual, class IsEqualZero>
bool BaseMatrix<ElementType,DerivedMatrixT,IsEqual,IsEqualZero>::IsIdentityMatrix() const
{
	for (uint32_t y = 0; y < m_VerticalDimensional; ++y)
	{
		for (uint32_t x = 0; x < m_HorizontalDimensional; ++x)
		{
			if (y == x)
			{
				if ( !m_IsEqual( (*this)[{y, x}], 1.0f) )
					return false;
			}
			else
			{
				if (!m_IsEqualZero((*this)[{y, x}]))
					return false;
			}
		}
	}
	return true;
}

template <typename ElementType, class DerivedMatrixT, class IsEqual, class IsEqualZero>
bool BaseMatrix<ElementType,DerivedMatrixT,IsEqual,IsEqualZero>::IsZeroMatrix() const
{
	return all_of(m_Data.begin(), m_Data.end(), m_IsEqualZero);
}









template <typename ElementType>
ElementType DotProduct(const Vector<ElementType>& a, const Vector<ElementType>& b)
{
	if (a.size() != b.size())
	{
		throw EXCEPTION;
	}

	ElementType result = 0;

	for (size_t c = 0, size = a.size(); c < size; c++)
	{
		result += a[c] * b[c];
	}

	return result;
}







template <class DerivedMatrixT>
DerivedMatrixT IdentityMatrix(const DerivedMatrixT& A)
{
	if (A.m_VerticalDimensional != A.m_HorizontalDimensional)
	{
		throw EXCEPTION;
	}

	DerivedMatrixT result = ZeroMatrix(A);

	for (uint32_t c = 0; c < A.m_VerticalDimensional; c++)
	{
		result[{c, c}] = 1.0f;
	}

	return result;
}


template <class DerivedMatrixT>
DerivedMatrixT ZeroMatrix(const DerivedMatrixT& A)
{
	DerivedMatrixT result(A);
	for (auto& e : result.m_Data)
	{
		e = 0.0f;
	}
	return result;
}



template <class DerivedMatrixT>
DerivedMatrixT PermutationColumnsMatrix(const DerivedMatrixT& src, const DerivedMatrixT& dest)
{
	if (src.m_HorizontalDimensional != dest.m_HorizontalDimensional ||
		src.m_VerticalDimensional != dest.m_VerticalDimensional)
	{
		throw EXCEPTION;
	}

	vector<uint32_t> usedColumns;
	usedColumns.reserve(src.m_HorizontalDimensional);

	DerivedMatrixT result(src.m_HorizontalDimensional, src.m_VerticalDimensional);

	for (uint32_t src_x = 0; src_x < src.m_HorizontalDimensional; src_x++)
	{
		DerivedMatrixT srcColumn = src.GetSubMatrix(0, src_x, src.m_VerticalDimensional, 1);


		uint32_t dest_x = dest.FindColumn(srcColumn);

		if (dest_x == DerivedMatrixT::npos)
		{
			throw EXCEPTION;
		}

		while (find(usedColumns.begin(), usedColumns.end(), dest_x) != usedColumns.end())
		{
			dest_x = dest.FindColumn(srcColumn, dest_x + 1);

			if (dest_x == DerivedMatrixT::npos)
			{
				throw EXCEPTION;
			}
		}


		result[{src_x, dest_x}] = 1.f;

		usedColumns.push_back(dest_x);
	}

	return result;
}


//template <
//	class charT, class traits,
//	typename ElementType, class DerivedMatrixT, class IsEqual /*= equal_to<ElementType>*/, class IsEqualZero /*= equal_to_zero<ElementType>
//																											 */>
//	std::basic_ostream<charT, traits>& operator<<(basic_ostream<charT, traits>& latex, const BaseMatrix<ElementType, DerivedMatrixT, IsEqual, IsEqualZero>& a)
//{
//
//}

template <class charT, class traits,
			typename ElementType, class DerivedMatrixT, class IsEqual = equal_to<ElementType>, class IsEqualZero = equal_to_zero<ElementType> >
std::basic_ostream<charT, traits>& operator<<(basic_ostream<charT, traits>& latex, const BaseMatrix<ElementType, DerivedMatrixT, IsEqual, IsEqualZero>& a)
{
	latex << a.AsLatexCode();	
	return latex;
}