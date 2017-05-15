#pragma once

#include "BaseMatrix.h"





template <typename MatrixElementType>	
class BlockMatrix : public BaseMatrix< MatrixElementType, BlockMatrix<MatrixElementType> >
{
public:
	using BaseMatrix::BaseMatrix;
	static BlockMatrix<MatrixElementType> Split(const MatrixElementType& m, uint32_t splitY, uint32_t splitX);

	ElementIndex GetElementCoord(uint32_t blockY, uint32_t blockX, uint32_t y, uint32_t x) const;

	friend MatrixElementType& DeBlock(MatrixElementType& lhs, const BlockMatrix<MatrixElementType>& rhs);
};



template <typename MatrixElementType>
MatrixElementType& DeBlock(MatrixElementType& lhs, const BlockMatrix<MatrixElementType>& rhs);






#include "BlockMatrix.inl"