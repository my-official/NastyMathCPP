#include "stdafx.h"
#include "Symbolic.h"
#include "Utils.h"
//-0.1f*0.5 * 2 / 3 * Pi*exp(t)*t*f(t)*sin(cos(f(t ^ 2)))*e * 3 ^ (2 / 3)*sqrt(4)*sqrt(3)*tg(Pi)
//sup ( NormSpectral(A), omega )



Symbolic::Symbolic(const string& expression)
{
	Parse(expression);
}


Symbolic::ParserSymbolType Symbolic::TypeOfSymbol(char sym)
{
	switch (sym)
	{
	case '.': return Digit;
	case ',': return Comma;

	case '^':
	case '*':
	case '/':
	case '+':
	case '-': return Operator;

	case '(': return OpenBreaket;
	case ')':  return CloseBreaket;

	default:
	{
		if (sym < ' ')
			return WhiteSpace;

		if (IN_RANGE(sym, '0', '9'))
			return Digit;

		if (IN_RANGE(sym, 'A', 'Z'))
			return Char;

		if (IN_RANGE(sym, 'a', 'z'))
			return Char;

		return Unknown;
	}

	}
}

void Symbolic::Parse(const string& expression)
{
	if (expression.empty())
		return;
		

	const char* first = expression.c_str();
	const char* sym = expression.c_str();

	SymbolicValue summand;

	ParseFirstSummand(first, sym, summand);

	m_Summands.emplace_back(summand);
	summand = SymbolicValue();
	
	while (*sym)
	{	
		if (TypeOfSymbol(*sym) != Operator)
		{
			throw ParseError(first, sym);
		}

		ParseBinaryOperator(first, sym, summand);
	}
	


}

void Symbolic::ParseNumber(const char* first, const char*& sym, SymbolicValue& summand)
{
	const char* start = sym;	

	while (*sym)
	{
		switch (TypeOfSymbol(*sym))
		{
			case Digit: sym++;	continue;
			case Point:	ParseRealNumber(first, start, sym, summand); return;

			case Operator:
			case WhiteSpace: break;			
			
			default: throw ParseError(first, sym);
		}
		break;
	}

	uint32_t number = strtoul(start, nullptr, 10);
	summand.Numerator.m_Numbers.emplace_back(number);	
}

void Symbolic::ParseRealNumber(const char* first, const char* start, const char*& sym, SymbolicValue& summand)
{	
	const char* frac = ++sym;	

	while (*sym)
	{
		switch (TypeOfSymbol(*sym))
		{
			case Digit: sym++;	continue;			

			case Operator:
			case WhiteSpace: break;

			default: throw ParseError(first, sym);
		}
		break;
	}	


	uint32_t intPart = strtoul(start, nullptr,10);	

	uint32_t fracPart = 1;

	uint32_t fracLength = sym - frac;

	if (fracLength > 0)
	{
		uint32_t denominator = std::pow(10, fracLength);

		fracPart = strtoul(frac, nullptr, 10);
			
		uint32_t numerator = intPart * denominator + fracPart;

		summand.Numerator.m_Numbers.push_back(numerator);
		summand.Denominator.m_Numbers.push_back(denominator);		
	}
	else
	{
		summand.Numerator.m_Numbers.push_back(intPart);
	}
}

void Symbolic::ParseCharacters(const char* first, const char*& sym, SymbolicValue& summand)
{
	throw std::logic_error("The method or operation is not implemented.");
}

void Symbolic::ParseBinaryOperator(const char* first, const char*& sym, SymbolicValue& summand)
{
	switch (*sym)
	{
		case '+':
		{
			SymbolicValue secondSummand;

			ParseSecondOperand(first, ++sym, secondSummand);

			m_Summands.emplace_back(summand);
			m_Summands.emplace_back(secondSummand);
			summand = SymbolicValue();
		}
		break;
		case '-':
		{
			SymbolicValue secondSummand;
			secondSummand.Numerator.m_Numbers.emplace_back(-1);

			ParseSecondOperand(first, ++sym, secondSummand);

			m_Summands.emplace_back(summand);
			m_Summands.emplace_back(secondSummand);
			summand = SymbolicValue();
		}
		break;
		case '*':
		{
			ParseSecondOperand(first, ++sym, summand);
		}
		break;
		case '/':
		{
			ParseDenominator(first, ++sym, summand.Denominator);
		}
		break;
		default: throw ParseError(first, sym);
	}
}


void Symbolic::ParseFirstSummand(const char* first, const char*& sym, SymbolicValue& summand)
{
	throw std::logic_error("The method or operation is not implemented.");
}

void Symbolic::ParseSecondOperand(const char* first, const char*& sym, SymbolicValue& secondSummand)
{
	throw std::logic_error("The method or operation is not implemented.");
}

void Symbolic::ParseDenominator(const char* first, const char*& sym, SymbolicValue& denominator)
{
	while (*sym)
	{
		switch (TypeOfSymbol(*sym))
		{
			case WhiteSpace: ++sym; continue;

			case Digit: 
			case Char:
			case OpenBreaket:
			

			default: throw ParseError(first, sym);
		}
		break;
	}

	throw ParseError(first, sym);
}

Symbolic Symbolic::operator+(const Symbolic& rhs) const
{
	Symbolic result(m_Expression + " + " + rhs.m_Expression);
	return result;
}


void Symbolic::Simplify()
{

}

ParseError::ParseError(const char* first, const char* sym) : logic_error("ParseError at " + to_string(sym - start) + ": " + string(sym, std::min<size_t>(strlen(sym), 32) ) )
{

}
