#pragma once


class Sum;
class Composition;

class Parser
{
public:
	Parser() {}
	~Parser() {}
	
	void Parse(const string& expression, Sum& symbolic);
private:
	Sum* m_Symbolic;
	
	enum SymbolType
	{
		Digit,
		Operator,
		Char,
		WhiteSpace,
		OpenBreaket,
		CloseBreaket,
		Point,
		Comma,
		LowLine,
		Unknown
	};

	SymbolType TypeOfSymbol(char sym);
	
	void ParseNumber(const char*& sym, Composition& summand);
	void ParseRealNumber(const char* start, const char*& sym, Composition& summand);

	void ParseCharacters(const char*& sym, Composition& summand);


	void ParseBinaryOperator(const char*& sym, Sum& symbolic);

	void ParseFirstOperand(const char*& sym, Sum& symbolic);
	void ParseSecondOperand(const char*& sym, Sum& symbolic);
	void ParseDenominator(const char*& sym, Sum& symbolic);
	void ParseBreaketExpression(const char*& sym, Sum& symbolic);
};




class ParseError : public std::logic_error
{
public:
	ParseError(const char* first, const char* sym);
	ParseError(const char* sym);
};

