#pragma once


template <class TYPE>
inline bool IN_RANGE(const TYPE x, const TYPE a, const TYPE b)
{
	return (((a) <= (x)) && ((x) <= (b)));
}

template <class TYPE>
inline bool OUT_RANGE(const TYPE x, const TYPE a, const TYPE b)
{
	//TYPE z = x;
	return (((a) > (x)) || ((x) > (b)));
}


