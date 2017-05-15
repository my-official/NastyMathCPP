#pragma once


template <typename MatrixElementType>
BlockMatrix<MatrixElementType> BlockMatrix<MatrixElementType>::Split(const MatrixElementType& m, uint32_t splitY, uint32_t splitX)
{
	if (splitY >= m.m_VerticalDimensional || 
		splitX >= m.m_HorizontalDimensional )
		throw EXCEPTION;

	auto vertical = m.m_VerticalDimensional;
	auto horizontal = m.m_HorizontalDimensional;

	BlockMatrix<MatrixElementType> result(2,2);
	
	result[{0, 0}] = m.GetSubMatrix(0, 0, splitY, splitX);						result[{0, 1}] = m.GetSubMatrix(0, splitX, splitY, horizontal - splitX);
	result[{1, 0}] = m.GetSubMatrix(splitY, 0, vertical - splitY, splitX);		result[{1, 1}] = m.GetSubMatrix(splitY, splitX, vertical - splitY, horizontal - splitX);

	return result;
}





template <typename MatrixElementType>
ElementIndex BlockMatrix<MatrixElementType>::GetElementCoord(uint32_t blockY, uint32_t blockX, uint32_t y, uint32_t x) const
{
	uint32_t resultY = y;
	uint32_t resultX = x;

	for (uint32_t cY = 0; cY < blockY; ++cY)
	{
		resultY += (*this)[{cY, 0}].m_VerticalDimensional;
	}

	for (uint32_t cX = 0; cX < blockX; ++cX)
	{
		resultX += (*this)[{0, cX}].m_HorizontalDimensional;
	}


	return{ resultY , resultX };
}


template <typename MatrixElementType>
MatrixElementType& DeBlock(MatrixElementType& lhs, const BlockMatrix<MatrixElementType>& rhs)
{
	decltype(lhs.m_VerticalDimensional) vertical = 0;
	decltype(lhs.m_HorizontalDimensional) horizontal = 0;

	for (uint32_t blockY = 0; blockY < rhs.m_VerticalDimensional; ++blockY)
	{
		vertical += rhs[{blockY, 0}].m_VerticalDimensional;
	}

	for (uint32_t blockX = 0; blockX < rhs.m_HorizontalDimensional; ++blockX)
	{
		horizontal += rhs[{0, blockX}].m_HorizontalDimensional;
	}

	if (lhs.m_VerticalDimensional != vertical || lhs.m_HorizontalDimensional != horizontal)
		throw EXCEPTION;



	for (uint32_t blockY = 0; blockY < rhs.m_VerticalDimensional; ++blockY)
	{
		for (uint32_t blockX = 0; blockX < rhs.m_HorizontalDimensional; ++blockX)
		{
			ElementIndex idx = rhs.GetElementCoord(blockY, blockX, 0, 0);

			lhs.SetSubMatrix(idx.first, idx.second, rhs[{blockY, blockX}]);
		}
	}
	return lhs;
}
