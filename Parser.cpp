#include "stdafx.h"
#include "Parser.h"
#include "Symbolic.h"
#include "Utils.inl"







Parser::SymbolType Parser::TypeOfSymbol(char sym)
{
	switch (sym)
	{
		case '.': return Point;
		case ',': return Comma;

		case '^':
		case '*':
		case '/':
		case '+':
		case '-': return Operator;

		case '(': return OpenBreaket;
		case ')':  return CloseBreaket;		
		case '_':  return LowLine;

		default:
		{
			if (IN_RANGE(sym, '0', '9'))
				return Digit;

			if (IN_RANGE(sym, 'A', 'Z'))
				return Char;

			if (IN_RANGE(sym, 'a', 'z'))
				return Char;

			if (sym <= ' ')
				return WhiteSpace;			

			return Unknown;
		}

	}
}

void Parser::Parse(const string& expression, Sum& symbolic)
{
	if (expression.empty())
		return;	

	const char* first = expression.c_str();
	const char* sym = first;

	try
	{
		symbolic.emplace_back();
		ParseFirstOperand(sym, symbolic);

		while (*sym)
		{
			switch (TypeOfSymbol(*sym))
			{
				case WhiteSpace: ++sym; break;
				case Operator: ParseBinaryOperator(sym, symbolic); break;
				default: throw ParseError(sym);
			}
		}

	}
	catch (ParseError&)
	{
		throw ParseError(first, sym);
	}
}

void Parser::ParseNumber(const char*& sym, Composition& summand)
{
	const char* start = sym;

	while (*sym)
	{
		switch (TypeOfSymbol(*sym))
		{
			case Digit: sym++;	continue;
			case Point:	ParseRealNumber(start, sym, summand); return;

			case CloseBreaket:
			case Operator:
			case WhiteSpace: break;

			default: throw ParseError(sym);
		}
		break;
	}

	uint32_t number = strtoul(start, nullptr, 10);
	summand.m_Scalar.m_Numbers.emplace_back(number);
}

void Parser::ParseRealNumber(const char* start, const char*& sym, Composition& summand)
{
	const char* frac = ++sym;

	while (*sym)
	{
		switch (TypeOfSymbol(*sym))
		{
			case Digit: sym++;	continue;

			case Operator:
			case WhiteSpace: break;

			default: throw ParseError(sym);
		}
		break;
	}

	auto ratio = GetRatio(strtod(start, nullptr));

	summand.m_Scalar.m_Numbers.emplace_back(ratio.first);
	summand.m_Dividers.emplace_back(ratio.second);


	/*	uint32_t intPart = strtoul(start, nullptr, 10);

	uint32_t fracPart = 1;

	uint32_t fracLength = sym - frac;

	if (fracLength > 0)
	{
	uint32_t denominator = lrint(pow(10, fracLength));

	fracPart = strtoul(frac, nullptr, 10);

	uint32_t numerator = intPart * denominator + fracPart;

	summand.m_Numbers.emplace_back(numerator);
	summand.m_Dividers.emplace_back(denominator);
	}
	else
	{
	summand.m_Numbers.emplace_back(intPart);
	}*/
}

void Parser::ParseCharacters(const char*& sym, Composition& summand)
{
	const char* start = sym++;

	while (*sym)
	{
		switch (TypeOfSymbol(*sym))
		{
		case LowLine:
		case Digit: 
		case Char: sym++;	continue;

		case CloseBreaket:
		case Operator:
		case WhiteSpace: break;

		default: throw ParseError(sym);
		}

		break;		
		
	}
	
	Varrible var = { string( start,  sym - start) };

	summand.m_Scalar.m_Varribles.push_back(var);
	return;
}

void Parser::ParseBinaryOperator(const char*& sym, Sum& symbolic)
{
	switch (*sym)
	{
		case '+':
		{
			symbolic.emplace_back();
			ParseSecondOperand(++sym, symbolic);
		}
		break;
		case '-':
		{
			Sum s;
			s.emplace_back();
			ParseSecondOperand(++sym, s);
			s *= -1;
			copy(s.begin(), s.end(), back_inserter(symbolic));
		}
		break;
		case '*':
		{
			Sum s;
			s.emplace_back();
			ParseSecondOperand(++sym, s);

			if (s.size() == 1)
			{
				symbolic.back() *= s[0];
			}
			else
			{
				symbolic.back().m_Multipliers.push_back(s);
			}
		}
		break;
		case '/':
		{
			Sum s;
			s.emplace_back();
			ParseDenominator(++sym, s);
			symbolic.back().m_Dividers.push_back(s);
		}
		break;
		default: throw ParseError(sym);
	}
}


void Parser::ParseFirstOperand(const char*& sym, Sum& symbolic)
{
	bool minus = false;
	while (*sym)
	{
		switch (TypeOfSymbol(*sym))
		{
			case WhiteSpace: ++sym; continue;

			case Operator:
			{
				if (minus)
					throw ParseError(sym);

				if (*sym != '-')
					throw ParseError(sym);

				minus = true;
				++sym;
			}
			continue;


			case OpenBreaket:
			{
				Sum s;
				s.emplace_back();
				ParseBreaketExpression(++sym, s);

				if (minus)
					s *= -1;

				if (s.size() == 1)
				{
					symbolic.back() = std::move(s.front());
				}
				else
				{
					symbolic.back().m_Multipliers.emplace_back(s);
				}

				return;
			}

			default:
			{
				//emplace_back();
				ParseSecondOperand(sym, symbolic);

				if (minus)
					symbolic *= -1;
				return;
			}
		}

		throw ParseError(sym);
	}
}

void Parser::ParseSecondOperand(const char*& sym, Sum& symbolic)
{
	while (*sym)
	{
		switch (TypeOfSymbol(*sym))
		{
			case WhiteSpace: ++sym; continue;

			case Digit:
			{
				//	SymbolicValue value;
				ParseNumber(sym, symbolic.back());
				//	push_back(value);
				return;
			}
			case Char:
			{
				//SymbolicValue value;
				ParseCharacters(sym, symbolic.back());
				//	push_back(value);
				return;
			}

			case OpenBreaket:
			{
				ParseBreaketExpression(++sym, symbolic);
				return;
			}

			default: throw ParseError(sym);
		}
	}

	throw ParseError(sym);
}

void Parser::ParseDenominator(const char*& sym, Sum& symbolic)
{
	ParseSecondOperand(sym, symbolic);
}

void Parser::ParseBreaketExpression(const char*& sym, Sum& symbolic)
{
	ParseFirstOperand(sym, symbolic);

	while (*sym)
	{
		switch (TypeOfSymbol(*sym))
		{
			case WhiteSpace: ++sym; continue;

			case Operator:
			{
				ParseBinaryOperator(sym, symbolic);
				continue;
			}

			case CloseBreaket:
			{
				++sym;
				return;
			}
			break;

			default: throw ParseError(sym);
		}
		break;
	}

	throw ParseError(sym);
}


ParseError::ParseError(const char* first, const char* sym) : logic_error("ParseError at " + to_string(sym - first) + ": " + string(sym, std::min<size_t>(strlen(sym), 32)))
{

}

ParseError::ParseError(const char* sym) : logic_error("ParseError")
{

}