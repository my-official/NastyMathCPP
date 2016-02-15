#pragma once
#include "Matricies.h"

//////////////////////////////////////////////////////////////////////////
//0.12222222f * 2 / 3 * Pi*exp(t)*t*f(t)*sin(cos(f(t ^ 2)))*e * 3 ^ (2 / 3)*sqrt(4)*sqrt(3)*tg(Pi)


template <typename Type>
struct Ratio
{
	Type Numerator;
	Type Denominator;
};


class Constant;
class Varrible;
class ExplicitFunc;
class ImplicitFunc;
class Symbolic;

class SymbolicValue
{
public:
	deque<int> m_Numbers;
	set<Constant> m_Consts;
	vector<Varrible> m_Varribles;
	vector<ExplicitFunc> m_ExplicitFuncs;
	vector<ImplicitFunc> m_ImplicitFuncs;
	unique_ptr< Symbolic > m_Denominator;
};


using SymbolicRatio = Ratio<SymbolicValue>;
using FloatRatio = Ratio<float>;


class Symbolic : public SymbolicValue
{
public:
	Symbolic() {}
	//Symbolic(char symbol);
	Symbolic(const string& expression);
	
	string m_Name;

	//vector< Symbolic > m_Summands;
	

	//SymbolicValue

	vector< Ratio< > m_Summands;


	vector< SymbolicValue > m_Summands;

	vector< SymbolicValue > m_Summands;
	unique_ptr< Symbolic > m_Denominator;



	//deque<int> m_Numbers;
	//set<Constant> m_Consts;
	//vector<Varrible> m_Varribles;
	//vector<ExplicitFunc> m_ExplicitFuncs;
	//vector<ImplicitFunc> m_ImplicitFuncs;
	//unique_ptr< Symbolic > m_Denominator;
	//vector< Symbolic > m_Summands;
		

	Symbolic operator+(const Symbolic& rhs) const;
	Symbolic operator*(const Symbolic& rhs) const;
	

	bool IsSymbolic();
	vector<string> GetSymbols();

	Symbolic Substitute(const string& name, const Symbolic& value);
	Symbolic Approximate();
	string AsString() const;
	void Simplify();	

	
	//SymbolicValue* m_Value;

	///
	//SymFunc m_Expression;
	/*vector<SymFunc> m_Summand;
	string m_Expression;*/
	///
	
	//summand
private:
	

	enum ParserSymbolType
	{
		Digit,
		Operator,
		Char,
		WhiteSpace,
		OpenBreaket,
		CloseBreaket,
		Point,
		Comma,
		Unknown	
	};

	ParserSymbolType TypeOfSymbol(char sym);
	void Parse(const string& expression);
	void ParseNumber(const char* first, const char*& sym, SymbolicValue& summand);
	void ParseRealNumber(const char* first, const char* start, const char*& sym, SymbolicValue& summand);
	
	void ParseCharacters(const char* first, const char*& sym, SymbolicValue& summand);
	void ParseBinaryOperator(const char* first, const char*& sym, vector<SymbolicValue>& summands, SymbolicValue& summand);
	void ParseFirstSummand(const char* first, const char*& sym, SymbolicValue& summand);
	void ParseSecondOperand(const char* first, const char*& sym, SymbolicValue& secondSummand);
	void ParseDenominator(const char* first, const char*& sym, SymbolicValue& denominator);
};

class ParseError : std::logic_error
{
public:
	ParseError(const char* first, const char* sym);
};

//
//class SymbolicValue
//{
//public:
//	virtual AsString() const = 0;
//	virtual void Substitute(const string& name, const Symbolic& value);
//	virtual Symbolic operator() const = 0;
//
//	virtual ~SymbolicValue() {}
//};


//
//class SymFunc
//{
//public:
//	//vector<Symbolic> m_Args;
//	virtual AsString() const = 0;
//	virtual SymFunc Substitute(const string& name, const Symbolic& value) = 0;
//	virtual Symbolic operator() const = 0;
//};

//
//class Plus : public Symbolic
//{
//public:
//	Plus(Symbolic a, Symbolic b)
//
//	virtual AsString() const override;
//	virtual Symbolic Substitute(const string& name, const Symbolic& value) override;
//	virtual Symbolic operator() const;
//};

//
//class Power
//{
//public:
//	vector<Symbolic> m_Args;
//	virtual AsString() const;
//	virtual SymFunc Substitute(const string& name, const Symbolic& value);
//	virtual Symbolic operator() const;
//};


enum class SymbolicType
{
	Real,
	Complex,
	Ration,
	SymFunc,
	ExplicitFunc,
	ImplicitFunc,
	Varrible
};



//struct MatrixBlock
//{
//	ElementIndex LeftTop;
//	ElementIndex RightBottom;
//};
//
//m.SplitIntoBlocks( Horizontal( 2, 3, 2),
//					Vertical(
//						1,
//						2
//					));
//
//A(1x2), L(1x3), F(1x2)"
//"2x2, 2x3, 2x2"
//
//m.GetMatrixBlock("L");
//
//
//class SymbolicMatrix : public Symbolic, public Matrix
//{
//public:
//	SymbolicMatrix(const string& matrixName, const string& elementNameMask = "%m_%y,%x");	
//	virtual ~SymbolicMatrix() {}
//	
//	string m_Name;
//	string m_ElementNameMask; // = "%m_%y,%x";
//	//Матрица имен элементов
//	vector<string> m_ElementNames;		
//};
//



