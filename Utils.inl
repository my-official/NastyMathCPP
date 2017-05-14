#pragma once


template <class TYPE>
bool IN_RANGE(const TYPE x, const TYPE a, const TYPE b)
{
	return (((a) <= (x)) && ((x) <= (b)));
}

template <class TYPE>
bool OUT_RANGE(const TYPE x, const TYPE a, const TYPE b)
{
	//TYPE z = x;
	return (((a) > (x)) || ((x) > (b)));
}

template <typename RealT>
bool IsFZero(RealT value)
{
	return FCmp<RealT>(value, 0.f);
}

template <typename RealT>
RealT sgn(RealT value)
{
	if (value > 0.f)
		return 1.f;
	else
		return -1.f;
}



template <typename RealT>
bool FCmp(RealT a, RealT b)
{
	return std::abs(a - b) < 0.001f/*numeric_limits<RealT>::epsilon()*/;
}









template <typename IntT>
IntT gcd(IntT a, IntT b)
{
	while (b) {
		a %= b;
		swap(a, b);
	}
	return a;
}

template <typename IntT>
void ReduceRatio(IntT& numerator, IntT& denominator)
{
	IntT c = gcd(numerator, denominator);
	numerator /= c;
	denominator /= c;
}


template <typename RealT>
std::pair<int, int> GetRatio(RealT realValue)
{
	auto value = to_string(realValue);

	const char* start = value.c_str();

	auto pos = value.find('.');

	assert(pos != string::npos && pos + 1 < value.length());

	const char* frac = start + pos + 1;
	const char* sym = &value[value.length()];


	int intPart = strtoul(start, nullptr, 10);

	int fracPart = 1;

	int fracLength = int(sym - frac);

	if (fracLength > 0)
	{
		int denominator = lrint(pow(10, fracLength));

		fracPart = strtoul(frac, nullptr, 10);

		int numerator = intPart * denominator + fracPart;

		ReduceRatio(numerator, denominator);

		return make_pair(numerator, denominator);


	}
	else
	{

		return make_pair(intPart, 1);
	}

	



	/*RealT intPart;
	RealT fracPart = std::abs(std::modf(realValue, &intPart));

	if (!IsFZero(fracPart))
	{
		int n = int(std::abs(log10(fracPart)));

		int denominator = 1;

		for (int c = 0; c < n; c++)
		{
			fracPart *= 10;
			denominator *= 10;
		}

		int numerator = (int)intPart * denominator + (int)fracPart;

		ReduceRatio(numerator, denominator);

		return make_pair(numerator, denominator);
	}
	else
	{
		return make_pair((int)intPart, 1);

	}*/


	/*

	RealT intPart;
	RealT fracPart = std::abs(std::modf(realValue, &intPart));

	int denominator = 1;


	while (fracPart < 0)
	{
	fracPart *= 10;
	denominator *= 10;
	}

	int numerator = (int)intPart * denominator + (int)fracPart;

	ReduceRatio(numerator, denominator);

	return make_pair(numerator, denominator);*/
}



