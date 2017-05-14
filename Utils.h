#pragma once


template <class TYPE>
bool IN_RANGE(const TYPE x, const TYPE a, const TYPE b);

template <class TYPE>
bool OUT_RANGE(const TYPE x, const TYPE a, const TYPE b);


template <typename RealT = float>
bool FCmp(RealT a, RealT b);

template <typename RealT = float>
bool IsFZero(RealT value);



template <typename RealT = float>
RealT sgn(RealT value);


typedef std::complex<float> complexF;

template <>
bool FCmp(complexF a, complexF b);


template <typename IntT = int>
IntT gcd(IntT a, IntT b);


template <typename IntT = int>
void ReduceRatio(IntT& numerator, IntT& denominator);


template <typename RealT = float>
pair<int, int> GetRatio(RealT realValue);


string to_latex(float);

void AddPath(const string& newpath);



inline string Passed(bool expr)
{
	return expr ? "Passed" : "Failed";
}


#include "Utils.inl"






