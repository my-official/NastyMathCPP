#include "stdafx.h"
#include "Examples.h"
#include "Symbolic.h"
#include "Symbolic\LatexContext.h"



//void PrintType(const GiNaC::ex& expr)
//{
//	using namespace GiNaC;
//	if (is_a<add>(expr))
//		cout << "Type is add" << endl;
//
//	if (is_a<mul>(expr))
//		cout << "Type is mul" << endl;
//}


void GinacTest()
{
	//Sum expanded = "a*c + a*d + b*c + b*d";	
	//cout << expanded.AsString() << endl;

	//vector<Sum> factors;

	//if ( !expanded.ToCompositionOfSums_Factor(factors) )
	//{
	//	cout << "can't factor" << endl;
	//	return;
	//}

	//cout << "factorized:  ";

	//for (auto& factor : factors)
	//{
	//	cout << "(" + factor.AsString() << ") * " << endl;
	//}
	//
	//cout << endl;
	//return;
		



//
//
//	Sum a("a"); Sum b("b");  Sum c("c");
//	Sum d("d"); Sum e("e");  Sum f("f");
//	Sum g("g"); Sum h("h");  Sum i("i");
//
//	Sum src = "a*c + a*d + b*c + b*d";
//	cout << "my is "<< src.AsString() << endl;	
//	
//	using namespace GiNaC;
//	
//	
//	lst syms;
//	syms.append( symbol(a.AsString()) );
//	syms.append( symbol(b.AsString()) );	
//	syms.append( symbol(c.AsString()) );
//	syms.append( symbol(d.AsString()) );	
//	syms.append(symbol(a.AsString()));
////	syms.append(symbol(a.AsString()));
//	ex exp1 = 0;
//
//	
//	exp1 = ex("a*c", lst({ symbol("a"), symbol("c") })) + ex("a*d", lst({ symbol("a"), symbol("d") }));	
//	
//	
//	/*exp1 += ex("b*c", lst({ symbol("b"), symbol("c") }));
//	exp1 += ex("b*d", lst({ symbol("b"), symbol("d") }));*/
//	
//
////	PrintType(exp1);
//
//	
//	cout << "exp1 is " << exp1 << endl;
//
////	cout << "nops" << exp1.nops() << endl;
//
//	cout << "RTTI name " << exp1.return_type_tinfo().tinfo->name() << endl;
//
//	for (auto& el : exp1)
//	{
//		print_dflt print_ctx(cout);
//		el.print(print_ctx);
//
//		//el
//		cout << endl;
//		//factorized.push_back(  )
//	}
//	
//
//		exp1 = GiNaC::factor(exp1, factor_options::all);
//
//		cout << "RTTI name " << exp1.return_type_tinfo().tinfo->name() << endl;
//
//		cout << "nops" << exp1.nops() << endl;
//
//		cout << "factorized is " << exp1 << endl;
//
//		PrintType(exp1);
//
//		ostringstream buffer;
//
//		Sum factorized;
//
//		for (auto& el : exp1)
//		{
//			print_dflt print_ctx(cout);
//			el.print(print_ctx);			
//
//			//el
//			cout << endl;
//			//factorized.push_back(  )
//		}
//
//		//	cout << endl;
//
//
}


void dxPerturbed()
{
	//MatrixSymbolDef(E_d);
	//MatrixSymbolDef(E_n_d);
	//MatrixSymbolDef(J_1);
	//MatrixSymbolDef(J_2);
	//	

	//MatrixSymbol O = MatrixSymbol::CreateAppropriateZeroMatrix();

	//MatrixSymbol A_wave =
	//{
	//	O, O,
	//	E_d, O
	//};

	//MatrixSymbol B_wave =
	//{
	//	J_1, E_n_d,
	//	J_2, O
	//};

	//MatrixSymbol RC[2], Delta[2], DQ[2];
	//	
	//DAE<> eq;

	//eq[1] = A_wave + RC[1] * Delta[1] * DQ[1];
	//eq[0] = B_wave + RC[0] * Delta[0] * DQ[0];
	//		

	//OperatorR operatorR = OperatorR::CreateFromOrder(1);

	//DAE<> eqAppliedR = operatorR.ApplyTo(eq);
}







void ArticleExample()
{
	RealMatrix A =
	{
		1, 0, -1,
		0, 0, -1,
		0, 0, 0
	};

	RealMatrix B({
		2, -1, -2,
		0, -1, 2,
		0, 0, 1
	});

	cout << "Input Data :" << endl;
	cout << endl;

	cout << "A = " << A.AsString() << endl;
	cout << "A.Rank() = " << A.Rank() << endl;

	cout << "B = " << B.AsString() << endl;
	cout << "B.Rank() = " << B.Rank() << endl;

	cout << endl;
	cout << "-------------" << endl;
	cout << "Compute_Dr" << endl;
	cout << "-------------" << endl;
	cout << endl;


	DAE<> dae(A, B);

	auto desc = dae.ConvertToESF();

	PrintESFDesc(desc);

	cout << "-------------" << endl;
	cout << "Structural form:" << endl << endl;

	cout << dae.AsString() << endl << endl;
}

void Test()
{
	RealMatrix A =
	{
		1, 0, -1,
		0, 0, -1,
		0, 0, 0
	};

	int num = 0;
	int prevNum = 100000;
	int numUsed = 0;

	int numNotIdenticalQ = 0;

	RealMatrix::ForAllMatrix(3, 3, 1.f, 4.f, 1.f,
		[&num, &prevNum, &numUsed, &numNotIdenticalQ, &A](const RealMatrix& B)
	{
		num++;
		//cout << m.AsString() << endl;

		if (num > prevNum)
		{
			cout << "progress!!!! num == " << num
				<< " numUsed == " << numUsed
				<< " numNotIdenticalQ == " << numNotIdenticalQ << endl;
			prevNum += 100000;

		}

		if (IsFZero(B.Det()))
		{
			return;
		}

		if (IsFZero((A + B).Det()))
		{
			return;
		}


		DAE<> dae(A, B);

		auto desc = dae.ConvertToESF();


		if (desc.R.m_Rs.size() == 3)
		{
			if (!desc.R.m_Rs[2].IsZeroMatrix())
				throw EXCEPTION;
		}


		if (!desc.Q.IsIdentityMatrix())
		{
			++numNotIdenticalQ;
		}

		if (dae.GetOrder() > 1)
		{
			throw EXCEPTION;
		}

		numUsed++;


	});

	cout << "Test success!!!! num == " << num
		<< " numUsed == " << numUsed
		<< " numNotIdenticalQ == " << numNotIdenticalQ << endl;
}


void SymbolicTest()
{
	Sum a = 1;
	a /= 1;
	a /= 1;

	a.Simplify();

	cout << a.AsString() << endl;



	//
	//Sum a("a"); Sum b("b");  Sum c("c");
	//Sum d("d"); Sum e("e");  Sum f("f");
	//Sum g("g"); Sum h("h");  Sum i("i");
	//

	//auto src = a / (b + c / d);
	//auto dest = a*d / (b*d + c);



	//Sum s1 = ( a*(c + b) )/ (c + b); 

	//Sum s2 = (a*c + a*b) / (c + b);


	//cout << "s1 = " << s1.AsString() << endl;
	//cout << "s2 = " << s2.AsString() << endl;

	//cout << "Test " << Passed(s1 == s2) << endl;

	//cout << "s1 = " << s1.ToCanonicalView().AsString() << endl;
	//cout << "s2 = " << s2.ToCanonicalView().AsString() << endl;



	//
	//cout << "Complex Test " << endl;


	////a,					b,					c
	////(d - d/a * a),	(e - d/a * b),    (f - d/a * b)
	////(g - g/a * a),	(h - g/a * b),    (i - g/a * b)

	///*Sum subExpr =  - (h - g / a * b);

	//cout << "subExpr == " << subExpr.AsString() << endl;*/
	//		
	//Sum expr2 = (h - g/a * b) - (h - g / a * b) / (e - d / a * b) * (e - d / a * b);

	//cout << "expr2 == " << expr2.AsString() << endl;	

	//cout << "Test " << Passed(expr2 == 0) << endl;

	//cout << "expr2 == " << expr2.ToCanonicalView().AsString() << endl;
}

void NewFeaturesTest()
{
	



	//{
	//	Sum expr1 = "(a + b)*((a + a) - (c + b)) + (c + a)";



	//	//2a - c - b

	//	//2aa - ac - ab + 2ab - bc - bb + c + a

	//	//	a + 2aa + ab - ac -bb - bc + c


	//	cout << "source is " << expr1.AsString() << endl;

	//	cout << "ToCanonicalView is " << expr1.ToCanonicalView().AsString() << endl;

	//	expr1.Substitute("a", 5);

	//	cout << "Simplify() is " << expr1.ToCanonicalView().AsString() << endl;

	//	expr1.Substitute("b", 2);

	//	cout << "Simplify() is " << expr1.ToCanonicalView().AsString() << endl;

	//	expr1.Substitute("c", -1);

	//	cout << "Simplify() is " << expr1.ToCanonicalView().AsString() << endl;

	//}
	//return;

	//{
	//	SymbolicMatrix A(3, 3, "a");

	//	//A[{0, 0}] = A[{1, 1}] = 0;

	//	cout << "A = " << A.AsString() << endl;

	//	cout << "A.Rank = " << A.AsStringRanked() << endl;

	//	return;

	//	SymbolicMatrix B(2, 2, "b");

	//	SymbolicMatrix AB = B*A;

	//	//cout << "AB ==  A*B + A*C is " << AB.ToCanonicalViewElementWise().AsString() << endl;



	//	SymbolicMatrix C(2, 2, "c");


	//	cout << "A = " << A.AsString() << endl;
	//	cout << "A = " << A.Det().AsString() << endl;

	//	cout << "B = " << B.AsString() << endl;
	//	cout << "B = " << B.Det().AsString() << endl;

	//	cout << "C = " << C.AsString() << endl;
	//	cout << "C = " << C.Det().AsString() << endl;


	//	SymbolicMatrix Left = A*B;
	//	SymbolicMatrix Right = B*A;
	//	Right = Right.Transpose();

	//	Left.ToCanonicalViewElementWise();
	//	Right.ToCanonicalViewElementWise();
	//	//Left.ToCanonicalViewElementWise();
	//	//Right.ToCanonicalViewElementWise();

	//	cout << "Left = " << Left.AsString() << endl << endl;
	//	//cout << "Det Left = " << Left.Det().ToCanonicalView().AsString() << endl;

	//	cout << "Right = " << Right.AsString() << endl << endl;
	//	//cout << "Det Right = " << Right.Det().ToCanonicalView().AsString() << endl;

	//	cout << "Result is " << (Left == Right) << endl;
	//}
	return;

	///Новая задача

	/*SymbolicMatrix A(3,3, "A" ), B(3, 3, "B");

	A[{0, 0}] = A[{1, 1}] = A[{2, 2}] = 0;
	

	DAE<> dae(A, B);

	auto desc = dae.ConvertToESF();

	PrintESFDesc(desc);

	cout << "-------------" << endl;
	cout << "Structural form:" << endl << endl;

	cout << dae.AsString() << endl << endl;

	


	Sum s;

	s = */


	//MatrixSymbol J("J");	

	//SymbolicSum DetJ = Det(J);

	//SymbolicMatrix Jm = DetJ.ToElementWise();


	//MatrixSymbol F("F");

	//SymbolicSum JF = J*F;

	//cout << "JF is " << JF.AsString() << endl;

	//SymbolicSum DetJF = Det(JF);

	//SymbolicMatrix JFm = DetJF.ToElementWise();



	//SymbolicSum DetJ = Det(F);


	/*SymbolicMatrix A = {
		"a", "b",
		"c", "d"
	};*/
/*
	SymbolicMatrix A(2, 2, "a");

	SymbolicMatrix B =
	{
		"J", "D",
		"F", "E"
	};

	cout << "A is " << A.AsString() << endl;

	cout << "Det A is " << A.Det().AsString() << endl;

	cout << "B is " << B.AsString() << endl;

	SymbolicMatrix C = A*B;

	cout << "C is " << C.AsString() << endl;

	cout << "Det C is " << C.Det().AsString() << endl;


	cout << "C inv is " << C.Inverse().AsString() << endl;


*/

	/*1/ ( sup  || G(j * omega) || )
		omega


	Symbolic omega("o");

	complexS s(0, omega);

	SymbolicMatrix I = IdentyMatrix(A);

	SymbolicMatrix sI = s*I;

	SymbolicMatrix W = (sI - A).Inverse();


	RealMatrix G = "C*W*B";
	G.Substitute('W', W);

	Symbolic j = SupBy( 1 / G.NormSpectral(), omega);

	j.AsString();

	cout << "a is " << j.AsString() << endl;




	G( complexF(0, omega) );

	1 /	SupBy(  NormSpectral(  G ), omega)


	constexpr int i = (1 + 5)*80 / ((4 + 1*3 + 1) * 6);

	cout << std::boolalpha;

	string expr = "(a + b)*g/ (f + c*d + e)*c - 11*c / h";

	Symbolic a(expr);

	cout << "a is " << a.AsString() << endl;

	cout << Passed(a.AsString() == "c*( a + b )/( f + c*d + e ) - 11*c )" && a.size() == 1 && a.front().m_Multipliers.size() == 1 && a.front().m_Dividers.size() == 1
	&& a.front().m_Multipliers.front().size() == 2
	&& a.front().m_Dividers.front().size() == 4
	) << endl;*/


	//Symbolic b("a+b");

	//cout << " is " << a.AsString() << endl;

	//cout << Passed(a.AsString() == "a+b") << endl;




	//Symbolic b("(b+c)*a");

	//cout << "b is " << b.AsString() << endl;







	////a += "0.5 / (5 + 15) * (a + b) / (a*c - b)";

	//cout << "expression is " << a.AsString() << endl;

	//return;

	////a += ""
	//Symbolic b("b");
	//Symbolic c("c + 1");

	//auto d = a + b * c + a - 0.33;

	//cout << "d == " << d.AsString() << endl;

	////d.Simplify();

	////cout << "Simplify " << d.AsString() << endl;

	//d.Substitute('a', 5);

	//cout << "d == " << d.AsString() << endl;

	//d.Substitute('b', 2);

	//cout << "d == " << d.AsString() << endl;

	//cout << std::boolalpha;

	//cout << "d == " << d.IsSymbolic() << endl;

	//d.Substitute('c', 1);

	//cout << "d == " << d.AsString() << endl;

	//cout << std::boolalpha;

	//cout << "d == " << d.IsSymbolic() << endl;





	

	/*SymbolicMatrix A("A", 3,3);	
	A[{2, 0}] = 0;
	A[{2, 1}] = "cos(t)";
	A[{2, 2}] = ratio<1,3>();

	SymbolicMatrix B("B");
	B[{2, 2}] = SymbolicFunction("f(t)");
	B[{2, 2}] = 12;

	B.SetDimensionals(3,3);


	SymbolicMatrix C = A*B;

	auto det = C.Det();

	auto CInv = C.Inverse();

	auto normSpectral = CInv.NormSpectral();


	CInv.Substitute("t", 0);

	auto r1 = sup(normSpectral, "f");*/




	//A[{1,1}] = 


	//A.SetBlockPartiton( 1,   );




	/*RealMatrix A =
	{
		2, 1, -0.5, 55,
		-4, 3, 2, -88,
		0, -2, 8, 4,
		1, 5, 2, 1
	};


	auto eigenvalues = A.Eigenvalues();

	for (auto& eigenvalue : eigenvalues)
	{
		cout << "eigenvalues == " << eigenvalue << endl;
	}

	cout << "Spectral Norm == " << A.NormSpectral() << endl;

	auto singularValues = A.SingularValues();

	for (auto& singularValue : singularValues)
	{
		cout << "singularValue == " << singularValue << endl;
	}*/
}



void FindNewExamples()
{
	//dx - Bx = 0

	RealMatrix A =
	{
		1, 0, -1, 0,
		0, 0, -1, 0,
		0, 0, 0, 1,
		0, 0, 0, 2
	};

	cout << "A.Det = " << A.Det() << endl;
	cout << "A.Ranked = " << A.AsStringRanked() << endl;

		

	RealMatrix B =
	{
		-1, -2, -2, 0,
		-2, -1, -2, 0,
		-2, 0, -2, 0,
		 0, 0, 0, 1
	};

	cout << "B.Det = " << B.Det() << endl;
	cout << "B.Ranked = " << B.AsStringRanked() << endl;





	if (IsFZero(B.Det()))
	{
		cout << "IsFZero(B.Det())" << endl;
		return;
	}

	if (IsFZero((A + B).Det()))
	{
		cout << "IsFZero((A + B).Det())" << endl;
		return;
	}


	LatexFile texFile("tex\\in.tex", "tex\\out.tex");

	auto texSection = texFile.SeekToSection("Автораздел");
	auto& tex = *texSection;

	DAE<RealMatrix> dae(A, B);

	tex << "Исходное уравнение" << endl;
	tex << dae.AsLatexCode() << endl;

	auto desc = dae.ConvertToESF();	

	tex << "Эквивалентная структурная форма" << endl;	
	tex << dae.AsLatexCode() << endl;

	
	


	PrintESFDesc(desc);


	RealMatrix C({
		7, -2, 1, 4,
		5, 7, 1, 1,
		1, 2, 3, 4,
		0, 0, 0, 0
	});

	RealMatrix D({
		5, 0, 0, 0,
		8, 0, 0, 0,
		1, 0, 0, 0,
		1, 0, 0, 0
	});

	DAE<> daeWithPerturbation(A, B - C * D);

	tex << "Возмущенное уравнение" << endl;
	tex << daeWithPerturbation.AsLatexCode() << endl;

	DAE<> esfPerturbed = desc.R.ApplyTo(daeWithPerturbation);

	tex << "Эквивалентная структурная форма для возмущенного уравнения" << endl;
	tex << esfPerturbed.AsLatexCode() << endl;
		
	cout << esfPerturbed.AsString() << endl << endl;

	texFile.Write();
	
	//RealMatrix::ForAllMatrix(4, 4, -4.f, -1.f, 1.f,
	//	[&A](const RealMatrix& B)
	//{
	//	if (IsFZero(B.Det()))
	//	{
	//		return;
	//	}

	//	if (IsFZero((A + B).Det()))
	//	{
	//		return;
	//	}


	//	DAE<> dae(A, B);

	//	auto desc = dae.ConvertToESF();

	//	if (dae.GetOrder() > 1)
	//	{
	//		static int counter = 0;
	//		counter++;
	//		if (counter > 500)
	//			throw EXCEPTION;
	//		return;
	//		
	//	}


	//	if (desc.Index >= 2)
	//	{
	//		cout << B.AsString() << endl;

	//		PrintESFDesc(desc);
	//		throw EXCEPTION;
	//	}
	//}
	//);
	
	cout << "Finished" << endl;


	char symbol;
	cin >> symbol;
	//cout << B.IsStable() << endl;
}

void NewExamples()
{
	RealMatrix A =
	{
		1, 2, 3,
		4, 5, 6,
		7, 8, 9
	};

		
	RealMatrix::ForAllMatrix(3, 3, 0, 4, 1, [&A](const RealMatrix B)
	{
		if (IsFZero(B.Det()))
			return;

		DAE<> dae(A, B);
		auto desc = dae.ConvertToESF();

		if (desc.Index > 1 && desc.d == 2 && desc.R.m_Rs.size() == 3 && dae.m_Coefficients.size() == 2)
		{
			cout << B.AsString() << endl << endl;
			throw EXCEPTION;
		}
		//else
		//{
		//	return;
		//}

		//RealMatrix O = ZeroMatrix(A);

		//RealMatrix Lambdar(2, 1, {
		//	O,
		//	A,
		//});

		//uint32_t n = A.m_HorizontalDimensional;
		//uint32_t rankLambdar = Lambdar.Rank();

		//uint32_t index = 1;
		//for (; index < MAX_INDEX; ++index)
		//{
		//	RealMatrix LambdarNext = Lambdar;
		//	DAE<>::GrowLambdarMatrix(LambdarNext, A, B);
		//	

		//	uint32_t rankLambdarNext = LambdarNext.Rank();

		//	/*	cout << "trying index " << index << endl;
		//	cout << "Rank Lambdar = " << rankLambdar << endl;
		//	cout << "Rank rankLambdarNext = " << rankLambdarNext << endl;*/


		//	if (rankLambdarNext - rankLambdar == n)
		//	{
		//		if (index > 1)
		//		{
		//			cout << B.AsString() << endl << endl;
		//			throw EXCEPTION;
		//		}
		//		else
		//		{
		//			return;
		//		}
		//	}

		//	Lambdar = LambdarNext;
		//	rankLambdar = Lambdar.Rank();
		//}

	});


	cout << "Finish" << endl;
	return;

	//Индекс 2
	RealMatrix B{
		4, 3, 1,
		2, 1, 0,
		1, 0, 0
	};


	//Индекс 1
	//RealMatrix B({
	//	2, -1, -2,
	//	0, -1, 2,
	//	0, 1, 1
	//});


	//Индекс 3
	//RealMatrix B({
	//	-1, -2, -2,
	//	-2, -1, -2,
	//	-2, 0, -2
	//});

	
	
	float detA = A.Det();
	float detB = B.Det();

	cout << "Input Data :" << endl;
	cout << endl;

	cout << "A = " << A.AsString() << endl;
	cout << "A.Det() = " << A.Det() << endl;
	cout << "A.Rank() = " << A.Rank() << endl;
	
	cout << "B = " << B.AsString() << endl;
	cout << "B.Rank() = " << B.Rank() << endl;
	cout << "B.Det() = " << B.Det() << endl;

	
	if (!IsFZero(detA))
	{
		return;
	}
	
	if (IsFZero(detB))
	{
		return;
	}

	cout << endl;
	cout << "-------------" << endl;
	cout << "Compute_Dr" << endl;
	cout << "-------------" << endl;
	cout << endl;

	DAE<> dae(A, B);

	DAE<> esf = dae;

	auto desc = esf.ConvertToESF();



	PrintESFDesc(desc);

	if (desc.Index < 2)
	{
		cout << "desc.Index < 2" << endl << endl;
		//cout << "A*A=" << (A*A).AsString() << endl << endl;
		return;
	}

	cout << "-------------" << endl;
	cout << "Structural form:" << endl << endl;

	cout << esf.AsString() << endl << endl;
	
	RealMatrix Delta({ 1000.f, 1000.f, 1000.f,
		1000.f, 1000.f, 1000.f,
		1000.f, 1000.f, 1000.f });
	
	RealMatrix::ForAllMatrix(3, 6, 0, 2, 1, [A, B, Delta, dae, esf, desc](const RealMatrix& CD)
	{
	/*	if (CD.IsZeroMatrix())
			return;*/

		RealMatrix C = CD.GetSubMatrix(0, 0, 3, 3);
		RealMatrix D = CD.GetSubMatrix(0, 3, 3, 3);			

		if (C.IsZeroMatrix())
			return;

		if (D.IsZeroMatrix())
			return;


		RealMatrix Phi_0_1 = (desc.R[0] * C * Delta * D).GetSubMatrix(0, 0, 2, 1);
		RealMatrix Phi_0_2 = (desc.R[0] * C * Delta * D).GetSubMatrix(0, 1, 2, 2);
		RealMatrix Phi_0_3 = (desc.R[0] * C * Delta * D).GetSubMatrix(2, 0, 1, 1);
		RealMatrix Phi_0_4 = (desc.R[0] * C * Delta * D).GetSubMatrix(2, 1, 1, 2);

		RealMatrix Phi_1_1 = (desc.R[1] * C * Delta * D).GetSubMatrix(0, 0, 2, 1);
		RealMatrix Phi_1_2 = (desc.R[1] * C * Delta * D).GetSubMatrix(0, 1, 2, 2);
		RealMatrix Phi_1_3 = (desc.R[1] * C * Delta * D).GetSubMatrix(2, 0, 1, 1);
		RealMatrix Phi_1_4 = (desc.R[1] * C * Delta * D).GetSubMatrix(2, 1, 1, 2);


		RealMatrix Phi_2 = desc.R[2] * C*Delta*D;

		if (!Phi_2.IsZeroMatrix())
			return;

		if (!Phi_1_2.IsZeroMatrix() || !Phi_1_4.IsZeroMatrix())
			return;

		if (Phi_1_3.NormSpectral() > 1)
			return;

		if (Phi_1_3.NormSpectral() > 1)
			return;

		RealMatrix E_n_d = IdentityMatrix(Phi_1_3);

		if ((Phi_0_2 - Phi_1_1*(E_n_d + Phi_1_3).Inverse() * Phi_0_4).NormSpectral() > 1)
			return;

		cout << C.AsString() << endl << endl;
		cout << D.AsString() << endl << endl;
		throw EXCEPTION;
	});

	cout << "Finish" << endl;
	return;


	


	/*DAE<RealMatrix> perturbedDae(A, B + C *Delta*D);

	perturbedDae = desc.R.ApplyTo(perturbedDae);

	cout << "perturbedDae.AsString()" << endl;
	cout << perturbedDae.AsString() << endl;*/








	//cout << "-------------" << endl;
	//cout << "Introduction perturbation" << endl;
	//cout << "-------------" << endl;

	//RealMatrix Delta({
	//	0.4f, 0, 0,
	//	0, 0.4f, 0,
	//	0, 0, 0.4f
	//});

	//cout << "Delta.Rank ==" << Delta.Rank() << endl;

	//RealMatrix C({
	//	7, -2, 1,		
	//	5, 7, 1,
	//	0, 0, 0
	//});

	//cout << "C.Rank ==" << C.Rank() << endl;
	//cout << "C.Det ==" << C.Det() << endl;

	//cout << "C*R1 = " <<  (desc.R.m_Rs[1]*C).AsString() << endl;
	//


	////	cout << "C.Rank ==" << C.Rank() << endl;

	//RealMatrix D({
	//	5, 0, 0,
	//	8, 0, 0,
	//	1, 0, 0
	//});

	//DAE<> daeWithPerturbation(A, B - C * Delta * D);

	//DAE<> esfPerturbed = desc.R.ApplyTo(daeWithPerturbation);

	//cout << esfPerturbed.AsString() << endl << endl;


	//


	//cout << "-------------" << endl;
	//cout << "Robust stability" << endl;
	//cout << "-------------" << endl;

	//RealMatrix J2 = dae.m_Coefficients[0].GetSubMatrix(2, 0, 1, 1);

	//cout << "J2 == " << J2.AsString() << endl;
	//cout << "J2.IsStable() ==  " << J2.IsStable() << endl;
	//
	//cout << "R0*C == " << (desc.R.m_Rs[0] * C).AsString() << endl;


	//cout << "C.Norm == " << C.NormSpectral() << endl;
	//cout << "D.Norm == " << D.NormSpectral() << endl;
	//cout << "Delta.Norm == " << Delta.NormSpectral() << endl;
	//cout << "(R0*C).Norm == " << (desc.R.m_Rs[0] * C).NormSpectral() << endl;

	//cout << "(C*Delta*D).Norm == " << (C*Delta*D).NormSpectral() << endl;
	//



	//return;
	//cout << endl;
	//cout << "Perturbation Only" << endl;
	//cout << "-------------" << endl;


	//DAE<> perturbationOnly(ZeroMatrix(A), C * Delta * D);


	//DAE<> esfPerturbationOnly = desc.R.ApplyTo(perturbationOnly);

	//cout << esfPerturbationOnly.AsString() << endl << endl;


	//RealMatrix Theta = esfPerturbationOnly.m_Coefficients[0].GetSubMatrix(2, 0, 1, 1);

	//cout << "Theta == " << Theta.AsString() << endl;

	//float normTheta = Theta.NormSpectral();

	//cout << "|| Theta || == " << normTheta << endl;
}


void TestEx()
{
	RealMatrix A =
	{
		1, 4, 4,
		3, 1, 1,
		1, 1, 1
	};

	RealMatrix B =
	{
		4, 3, 1,
		2, 1, 1,
		1, 1, 1
	};

	//auto eigenvalues = (A + B).Eigenvalues();

	RealMatrix pencil = A + B.Scale(2);

	if (IsFZero((pencil).Det()))
		return;

	if (!IsFZero(A.Det()))
		return;

	if (IsFZero(B.Det()))
		return;

	DAE<> dae(A, B);

	DAE<> esf = dae;

	ESFDesc<> desc = esf.ConvertToESF();
	PrintESFDesc(desc);
}


void teacher()
{
	LatexFile texFile("tex\\in.tex", "tex\\out.tex", true);

	auto doit = [&](const SymbolicMatrix& A, const SymbolicMatrix& B, const string& sectionName)
	{
		auto texSection = texFile.SeekToSection(sectionName.c_str());

		auto& tex = *texSection;

		DAE<SymbolicMatrix> dae(A, B);

		tex << "Рассмотрим уравнение: " << endl;

		tex << dae.AsLatexCode() << endl;


		tex << "$det A =" << A.Det().AsLatexCode() << "$" << endl;

		tex << "$det B =" << B.Det().AsLatexCode() << "$" << endl;

		auto esf = dae;		
		auto desc = esf.ConvertToESF();

		tex << endl << endl << "Приведем это уравнение к структурной форме. Для этого построим матрицу Dr:" << endl;
		tex << PrintESFDescAsLatexCode(desc) << endl;

		tex << "Структурная форма" << endl;

		tex << esf.AsLatexCode() << endl;
	};


	//RealMatrix A =
	//{
	//	1, 0, -1,
	//	0, 0, -1,
	//	0, 0, 0
	//};

	//RealMatrix B({
	//	2, -1, -2,
	//	0, -1, 2,
	//	0, 0, 1
	//});



	Sum zero = Sum(0);
	Sum one = Sum(1);
	Sum m_one = Sum(-1);

	
	SymbolicMatrix A =
	{
		one, zero, m_one,
		zero, zero, m_one,
		zero, zero, zero
	};

	//SymbolicMatrix B(3, 3, "b");
	SymbolicMatrix B = {
		Sum(Varrible("b_{11}")), Sum(Varrible("b_{12}")), Sum(Varrible("b_{13}")),
		Sum(0), Sum(Varrible("b_{22}")), Sum(Varrible("b_{23}")),
		Sum(0), Sum(0), Sum(Varrible("b_{33}"))
	};

	doit(A, B, "Пример 1");	


	texFile.Write();
	/*texFile.Texify(true);*/

	cout << "Finished " << endl;
}

void TestRank()
{
	LatexFile texFile("tex\\in.tex", "tex\\out.tex", true);
	auto texSection = texFile.SeekToSection("Пример 1");
	auto& tex = *texSection;


	Sum o = Sum(0);
	Sum one = Sum(1);
	Sum m_one = Sum(-1);

	SymbolicMatrix Dr (9,12,{
		Sum(Varrible("b_{11}")),Sum(Varrible("b_{12}")),Sum(Varrible("b_{13}")),1 , o , -1 , o , o , o , o , o , o,
		o , Sum(Varrible("b_{22}")),Sum(Varrible("b_{23}")),o , o , -1 , o , o , o , o , o , o,
		o , o , Sum(Varrible("b_{33}")),o , o , o , o , o , o , o , o , o,
		o , o , o , Sum(Varrible("b_{11}")),Sum(Varrible("b_{12}")),Sum(Varrible("b_{13}")),1 , o , -1 , o , o , o,
		o , o , o , o , Sum(Varrible("b_{22}")),Sum(Varrible("b_{23}")),o , o , -1 , o , o , o,
		o , o , o , o , o , Sum(Varrible("b_{33}")),o , o , o , o , o , o,
		o , o , o , o , o , o , Sum(Varrible("b_{11}")),Sum(Varrible("b_{12}")),Sum(Varrible("b_{13}")),1 , o , -1,
		o , o , o , o , o , o , o , Sum(Varrible("b_{22}")),Sum(Varrible("b_{23}")),o , o , -1,
		o , o , o , o , o , o , o , o , Sum(Varrible("b_{33}")),o , o , o
	});

	tex << "$$" << Dr << "$$" << endl;
	Dr.FlipHorizontal();	
	tex << "$$" << Dr << "$$" << endl;
	Dr.FlipHorizontal();

	vector<ElementIndex> rightTopLIE;
	auto rightTopRank = Dr.Rank(RankMethod::RightTop, &rightTopLIE);

	vector<ElementIndex> leftTopLIE;
	auto leftTopRank = Dr.Rank(RankMethod::LeftTop, &leftTopLIE);	
	

	vector<ElementIndex> leftBottomLIE;
	auto leftBottomRank = Dr.Rank(RankMethod::LeftBottom, &leftBottomLIE);

	vector<ElementIndex> rightBottomLIE;
	auto rightBottomRank = Dr.Rank(RankMethod::RightBottom, &rightBottomLIE);

	bool passed = (leftTopRank == rightTopRank) &&
		(leftBottomRank == rightBottomRank) &&
		(rightTopRank == leftBottomRank);
	
	passed = passed && (leftTopLIE.size() == rightTopLIE.size()) &&
		(leftBottomLIE.size() == rightBottomLIE.size()) &&
		(rightTopLIE.size() == leftBottomLIE.size());
		

	if (!passed)
	{
		throw EXCEPTION;
	}
}

void FindNewExamplesEx()
{
	//RealMatrix A =
	//{
	//	1, 2, 3,
	//	4, 5, 6,
	//	7, 8, 9
	//};

	mutex mtxSuitableB;
	int numB = 0;

	deque<RealMatrix> suitableB;


	RealMatrix::ForAllMatrixThreaded(3, 6, 1, 4, 1, [/*&suitableB,*/ &mtxSuitableB, &numB](const RealMatrix& AB)
	{
		RealMatrix A = AB.GetSubMatrix(0, 0, 3, 3);
		RealMatrix B = AB.GetSubMatrix(0, 3, 3, 3);

	/*	if (count(A.m_Data.begin(), A.m_Data.end(), 0.0f) > 5)
			return;*/

		if (!IsFZero(A.Det()))
			return;

		if (IsFZero(B.Det()))
			return;

		DAE<> dae(A, B);

		DAE<> esf = dae;

		ESFDesc<> desc;
		try
		{
			 desc = esf.ConvertToESF();
		}
		catch (...)
		{
			return;
			/*lock_guard<mutex> lock(mtxSuitableB);
			cout << "catched" << endl;
			cout << "A =" << A.AsString() << endl << endl;
			cout << "B =" << B.AsString() << endl << endl;
			throw;*/
		}

		if (!(desc.Index == 2 && desc.d == 2 && desc.R.m_Rs.size() == 3 && esf.m_Coefficients.size() == 2))
		{
			return;
		}

		AdmisablePerturbationDesc addmisablePerturbation = AdmisablePerturbationDesc::AdmisableABPerturbation(desc);

		SymbolicMatrix& C_1 = addmisablePerturbation.C[1];
		SymbolicMatrix& D_1 = addmisablePerturbation.D[1];


		if (C_1.IsZeroMatrix() || D_1.IsZeroMatrix())
		{
			return;
		}

		SymbolicMatrix& C_0 = addmisablePerturbation.C[0];
		SymbolicMatrix& D_0 = addmisablePerturbation.D[0];
		//SymbolicMatrix& Delta = addmisablePerturbation.Delta[0];


		if (C_0.IsZeroMatrix() || D_0.IsZeroMatrix())
		{
			return;
		}

		lock_guard<mutex> lock(mtxSuitableB);
		cout << "new matrix" << endl;
		cout << "A =" << A.AsString() << endl << endl;
		cout << "B =" << B.AsString() << endl << endl;
		++numB;

		if (numB >= 10)
			throw EXCEPTION;
	});
}

void NewArticleBlockMatrixMultiplication()
{
	Sum E_d(Varrible("E_{d}"));
	Sum E_n_d(Varrible("E_{n-d}"));	


	Sum Phi_0_1( Varrible("\\Phi_{01}") );
	Sum Phi_0_2( Varrible("\\Phi_{02}") );
	Sum Phi_0_3( Varrible("\\Phi_{03}") );
	Sum Phi_0_4( Varrible("\\Phi_{04}") );
	
	Sum Phi_1_1( Varrible("\\Phi_{11}") );
	Sum Phi_1_2( Varrible("\\Phi_{12}") );
	Sum Phi_1_3( Varrible("\\Phi_{13}") );
	Sum Phi_1_4( Varrible("\\Phi_{14}") );
			

	Sum S1( Varrible("S_1^{-1}") );// S_1 = (E_n_d + Phi_1_3) 
	Sum S2(Varrible("S_2^{-1}"));// S_2 = (E_d + Phi_0_2 - Phi_1_1 * S_1^(-1) * Phi_0_4)

	SymbolicMatrix K1({
	E_d,					0,
	-S1 * Phi_0_4,			E_n_d
	});

	SymbolicMatrix K2({
	S2,		0,
	0,	E_n_d
	});

	SymbolicMatrix K3({
	E_d,	-Phi_1_1,
	0,	E_n_d
	});

	SymbolicMatrix K4({
	E_d,	0,
	0,	S1
	});

	SymbolicMatrix K = K1 * K2 * K3 * K4;

	K.Substitute(Varrible("E_{d}"),1);
	K.Substitute(Varrible("E_{n-d}"), 1);

	K.Simplify();
	
	LatexFile texFile("tex\\in.tex", "tex\\out.tex");

	auto texSection = texFile.SeekToSection("Пример 1");
	auto& tex = *texSection;

	tex << "$$" << "K1 = " << K1.AsLatexCode() << "$$" << endl << endl;

	tex << "$$" << "K2 = " << K2.AsLatexCode() << "$$" << endl << endl;

	tex << "$$" << "K3 = " << K3.AsLatexCode() << "$$" << endl << endl;

	tex << "$$" << "K4 = " << K4.AsLatexCode() << "$$" << endl << endl;

	tex << "$$" << "K = " << K.AsLatexCode() << "$$" << endl << endl;
	
	texFile.Write();
}

void ParallelForEachMatrixTest()
{
	vector<RealMatrix>	A1;
	recursive_mutex mutex;
	vector<RealMatrix>	A2;

	A1.reserve(10000);
	A2.reserve(10000);



	RealMatrix::ForAllMatrixThreaded(2, 2, -1, 1, 1, [&A1, &mutex](const RealMatrix& A)
	{
		mutex.lock();
		A1.push_back(A);
		mutex.unlock();
	});


	RealMatrix::ForAllMatrix(2, 2, -1, 1, 1, [&A2](const RealMatrix& A)
	{
		A2.push_back(A);
	});

	

	if (A1.size() != A2.size())
	{
		cout << Passed(false);
		return;
	}

	cout << Passed( is_permutation(A1.begin(), A1.end(), A2.begin()) );
}

void SymbolicMatrixTest()
{
	RealMatrix::ForAllMatrixThreaded(2, 4, -2, 2, 1, [](const RealMatrix& AB)
	{
		RealMatrix A = AB.GetSubMatrix(0, 0, 2, 2);
		RealMatrix B = AB.GetSubMatrix(0, 2, 2, 2);

		SymbolicMatrix symbolicA = A;
		SymbolicMatrix symbolicB = B;

		if (A*B != symbolicA*symbolicB)
		{
			cout << "A = " << A.AsString() << endl << endl;
			cout << "B = " << B.AsString() << endl << endl;
			throw EXCEPTION;
		}
	});

	cout << "Successfully finished!!!" << endl;


	//RealMatrix A =
	//{
	//	-2,-2,
	//	-2,-2
	//};

	//RealMatrix B{
	//	-2,-2,
	//	0,-2
	//};

	//RealMatrix AB = A*B;

	//cout << "A*B= " << AB.AsString() << endl << endl;

	//SymbolicMatrix symbolicA = A;
	//SymbolicMatrix symbolicB = B;

	//cout << "symbolicA == A" << (symbolicA == A) << endl;
	//cout << "symbolicB == B" << (symbolicB == B) << endl;

	//SymbolicMatrix symbolicAB = symbolicA * symbolicB;

	//cout << "symbolicAB= " << symbolicAB.AsString() << endl << endl;
	//cout << "symbolicAB.Simplify()= " << symbolicAB.Simplify().AsString() << endl << endl;
	//cout << Passed(symbolicAB == AB)  << endl;

}

void ExampleB()
{
	RealMatrix A =
	{
		1, 2, 3,
		4, 5, 6,
		7, 8, 9
	};

	RealMatrix B{
		4, 3, 1,
		2, 1, 0,
		1, 0, 0
	};

	/*RealMatrix A =
	{
	1.000000, 2.000000, 1.000000,
	1.000000, 2.000000, 1.000000,
	1.000000, 1.000000, 1.000000
	};

	RealMatrix B{
	3.000000, 3.000000, 2.000000,
	2.000000, 1.000000, 1.000000,
	1.000000, 1.000000, 1.000000
	};*/


	DAE<> dae(A, B);

	DAE<> esf = dae;
	ESFDesc<> desc = esf.ConvertToESF();



	AdmisablePerturbationDesc addmisablePerturbation = AdmisablePerturbationDesc::AdmisableBPerturbation(desc);

	SymbolicMatrix& C_0 = addmisablePerturbation.C[0];
	SymbolicMatrix& D_0 = addmisablePerturbation.D[0];
	SymbolicMatrix& Delta_0 = addmisablePerturbation.Delta[0];


	DAE<SymbolicMatrix> daePerturbed(A , B + C_0 * Delta_0 * D_0);

	OperatorR<SymbolicMatrix> RPerturbed = addmisablePerturbation.CreateOperatorR_BPerturbed(desc);




	DAE<SymbolicMatrix> esfPerturbed = RPerturbed.ApplyTo(daePerturbed);

	

	esfPerturbed[1].Simplify();
	esfPerturbed[0].Simplify();


	LatexFile texFile("tex\\in.tex", "tex\\out.tex");

	auto texSection = texFile.SeekToSection("Пример 1");
	auto& tex = *texSection;


	tex << "Исходное уравнение: " << endl << endl;

	tex << dae.AsLatexCode() << endl;

	tex << "Преобразование " << endl << endl;

	tex << PrintESFDescAsLatexCode(desc) << endl;

	tex << "Оператор R: " << endl << endl;

	tex << "$$" << desc.R.AsLatexCode() << "$$" << endl;

	tex << "Преобразованное уравнение: " << endl << endl;

	tex << esf.AsLatexCode() << endl;


	tex << "Возмущенное уравнение: " << endl << endl;

	tex << daePerturbed.AsLatexCode() << endl;

	tex << "в нем" << endl << endl;

	tex << "$$" << "C_2 = " << C_0.AsLatexCode() << "$$" << endl;

	tex << "$$" << "\\Delta_2 = " << Delta_0.AsLatexCode() << "$$" << endl;

	tex << "$$" << "D_2 = " << D_0.AsLatexCode() << "$$" << endl;
		
	tex << "$$" << "\\Phi_0 = " << addmisablePerturbation.Phi[0].AsLatexCode() << "$$" << endl;

	tex << "$$" << "\\Phi_1 = " << addmisablePerturbation.Phi[1].AsLatexCode() << "$$" << endl;

	tex << "$$" << "\\Phi_2 = " << addmisablePerturbation.Phi[2].AsLatexCode() << "$$" << endl;

	tex << "Условие 3:" << endl;
	tex << "$$" << "\\Vert " << addmisablePerturbation.Cond3.AsLatexCode() << "\\Vert < 1" << "$$" << endl;

	tex << "Условие 4:" << endl;
	tex << "$$" << "\\Vert " << addmisablePerturbation.Cond4.AsLatexCode() << "\\Vert < 1" << "$$" << endl;

	//tex << "$$" << "\\Upsilon_0 = " << addmisablePerturbation.Ypsilon[0].AsLatexCode() << "$$" << endl;

	//tex << "$$" << "\\Upsilon_1 = " << addmisablePerturbation.Ypsilon[1].AsLatexCode() << "$$" << endl;

	//tex << "$$" << "\\Upsilon_2 = " << addmisablePerturbation.Ypsilon[2].AsLatexCode() << "$$" << endl;


	tex << "Оператор R для возмущенного: " << endl << endl;

	tex << "$$" << RPerturbed.AsLatexCode() << "$$" << endl;

	tex << "Преобразованное возмущенное уравнение: " << endl << endl;

	tex << esfPerturbed.AsLatexCode() << endl;

	SymbolicMatrix C(3,3,"c"), D(3, 3, "d"), Delta(3, 3, "\\delta");

	D[{0, 2}] = D[{1, 2}] = D[{2, 2}] = 0;

	SymbolicMatrix R2_C_Delta_D =  desc.R.m_Rs[1] * C * Delta * D;
	R2_C_Delta_D.Simplify();

	tex << "Поэлементное умножение: " << endl << endl;
	tex << "$$" << R2_C_Delta_D.AsLatexCode() << "$$" << endl << endl;


	texFile.Write();
}

void ExampleAB()
{
	RealMatrix A =
	{
		1.000000, 2.000000, 1.000000,
		1.000000, 2.000000, 1.000000,
		1.000000, 1.000000, 1.000000
	};

	RealMatrix B{
		3.000000, 3.000000, 2.000000,
		2.000000, 1.000000, 1.000000,
		1.000000, 1.000000, 1.000000
	};


	DAE<> dae(A, B);

	DAE<> esf = dae;
	ESFDesc<> desc = esf.ConvertToESF();

	
	
	AdmisablePerturbationDesc addmisablePerturbation = AdmisablePerturbationDesc::AdmisableABPerturbation(desc);

	SymbolicMatrix& C_0 = addmisablePerturbation.C[0];
	SymbolicMatrix& D_0 = addmisablePerturbation.D[0];
	SymbolicMatrix& Delta_0 = addmisablePerturbation.Delta[0];

	SymbolicMatrix& C_1 = addmisablePerturbation.C[1];
	SymbolicMatrix& D_1 = addmisablePerturbation.D[1];
	SymbolicMatrix& Delta_1 = addmisablePerturbation.Delta[1];



	DAE<SymbolicMatrix> daePerturbed(A + C_1 * Delta_1 * D_1 , B + C_0 * Delta_0 * D_0);

	OperatorR<SymbolicMatrix> RPerturbed = addmisablePerturbation.CreateOperatorR_ABPerturbed(desc);

	cout << std::boolalpha;

	cout << "R and RPerturbed is equal? " << (desc.R == RPerturbed) << endl;
		
	DAE<SymbolicMatrix> esfPerturbed = RPerturbed.ApplyTo(daePerturbed);

	esfPerturbed[1].Simplify();
	esfPerturbed[0].Simplify();
	



//	SymbolicMatrix Phi_2 = R[2] * C * Delta * D;
	
	
//	cout << Phi_2.AsString() << endl << endl;

//	Phi_2.Simplify();

	//cout << "Simplified: " << endl << endl;

	//cout << Phi_2.AsString() << endl << endl;
	

	LatexFile texFile("tex\\in.tex", "tex\\out.tex");

	auto texSection = texFile.SeekToSection("Пример 1");
	auto& tex = *texSection;


	tex << "Исходное уравнение: " << endl << endl;

	tex << dae.AsLatexCode() << endl;

	tex << "Преобразование " << endl << endl;

	tex << PrintESFDescAsLatexCode(desc) << endl;

	tex << "Оператор R: " << endl << endl;

	tex << "$$" << desc.R.AsLatexCode() << "$$" << endl;

	tex << "Преобразованное уравнение: " << endl << endl;

	tex << esf.AsLatexCode() << endl;


	tex << "Возмущенное уравнение: " << endl << endl;

	tex << daePerturbed.AsLatexCode() << endl;

	tex << "в нем" << endl << endl;

	tex << "$$" << "C_0 = " << C_0.AsLatexCode() << "$$" << endl;

	tex << "$$" << "\\Delta_0 = " << Delta_0.AsLatexCode() << "$$" << endl;

	tex << "$$" << "D_0 = " << D_0.AsLatexCode() << "$$" << endl;

	tex << "$$" << "C_1 = " << C_1.AsLatexCode() << "$$" << endl;

	tex << "$$" << "\\Delta_1 = " << Delta_1.AsLatexCode() << "$$" << endl;

	tex << "$$" << "D_1 = " << D_1.AsLatexCode() << "$$" << endl;

	tex << "$$" << "\\Phi_0 = " << addmisablePerturbation.Phi[0].AsLatexCode() << "$$" << endl;

	tex << "$$" << "\\Phi_1 = " << addmisablePerturbation.Phi[1].AsLatexCode() << "$$" << endl;

	tex << "$$" << "\\Phi_2 = " << addmisablePerturbation.Phi[2].AsLatexCode() << "$$" << endl;

	tex << "$$" << "\\Upsilon_0 = " << addmisablePerturbation.Ypsilon[0].AsLatexCode() << "$$" << endl;

	tex << "$$" << "\\Upsilon_1 = " << addmisablePerturbation.Ypsilon[1].AsLatexCode() << "$$" << endl;

	tex << "$$" << "\\Upsilon_2 = " << addmisablePerturbation.Ypsilon[2].AsLatexCode() << "$$" << endl;	

	tex << "Условие 4:" << endl;
	tex << "$$" << "\\Vert " << addmisablePerturbation.Cond4.AsLatexCode() << "\\Vert < 1" << "$$" << endl;

	tex << "Условие 5:" << endl;
	tex << "$$" << "\\Vert " << addmisablePerturbation.Cond5.AsLatexCode() << "\\Vert < 1" << "$$" << endl;


	tex << "Оператор R для возмущенного: " << endl << endl;

	tex << "$$" << RPerturbed.AsLatexCode() << "$$" << endl;

	tex << "Преобразованное возмущенное уравнение: " << endl << endl;

	tex << esfPerturbed.AsLatexCode() << endl;
	
	texFile.Write();	
}

void LatexTest()
{
	/*LatexFile texFile("tex\\in.tex", "tex\\out.tex");
	
	auto texSection = texFile.SeekToSection("Автораздел");
	auto& tex = *texSection;	
	
	MatrixSymbol A(3, 3, "A");
	MatrixSymbol B(3, 3, "B");
	MatrixSymbol C(3, 3, "C");
	MatrixSymbol D(3, 3, "D");
	MatrixSymbol Delta(3, 3, "\\Delta");

	DAE<MatrixExpression> dae(A,B + C*Delta*D);
	
	tex << endl;
	tex << "Русский текст" << endl;
	tex << "$$" << dae.AsLatexCode() << "$$" << endl;
	
	texFile.Write();

	cout << "Finished" << endl;*/


	//texFile.Write();


	//SymbolicMatrix sA(3, 3, "a");
	//	
	//*tex << "A=" << sA << endl;
	//*tex << "DetsA=$" << sA.Det().AsLatexCode() << "$"<< endl;
		
	

	//texify -c -q -p --run-viewer out.tex
	//texify -c -q -p -b --run-viewer out.tex

//	texify - c - q - p - b --run - viewer out.tex
}

void AdmissiblePerturbations(const ESFDesc<RealMatrix>& desc, SymbolicMatrix& C, SymbolicMatrix& D)
{
	BlockSymbolicMatrix BlockD = BlockSymbolicMatrix::Split(D, desc.n - desc.d, desc.d);
	BlockSymbolicMatrix BlockC = BlockSymbolicMatrix::Split(C, desc.n - desc.d, desc.d);

	SymbolicMatrix R1C = desc.R[1] * C;

	BlockSymbolicMatrix BlockR1C = BlockSymbolicMatrix::Split(R1C, desc.n - desc.d, desc.d);

	BlockD[{0, 1}] = ZeroMatrix(BlockD[{0, 1}]);
	BlockD[{1, 1}] = ZeroMatrix(BlockD[{1, 1}]);

	if (BlockR1C[{0, 0}] != ZeroMatrix(BlockR1C[{0, 0}]) &&
		BlockR1C[{0, 1}] != ZeroMatrix(BlockR1C[{0, 1}]) )
	{
		throw EXCEPTION;
	}

	if (BlockR1C[{0, 0}] != ZeroMatrix(BlockR1C[{0, 0}]))
	{
		BlockC[{0, 0}] = ZeroMatrix(BlockC[{0, 0}]);
		BlockC[{1, 0}] = ZeroMatrix(BlockC[{1, 0}]);
	}
	else
	{
		BlockC[{0, 1}] = ZeroMatrix(BlockC[{0, 1}]);
		BlockC[{1, 1}] = ZeroMatrix(BlockC[{1, 1}]);
	}


	DeBlock<SymbolicMatrix>(C,BlockC);
	DeBlock<SymbolicMatrix>(D,BlockD);
}

void LatexNewArticleExamples()
{
	LatexFile texFile("tex\\in.tex", "tex\\out.tex");
		
	
		
	auto doit = [&](const RealMatrix& A, const RealMatrix& B, const string& sectionName)
	{
		auto texSection = texFile.SeekToSection(sectionName.c_str());

		auto& tex = *texSection;
				

		DAE<RealMatrix> dae(A, B);
				
		auto esf = dae;

		auto desc = esf.ConvertToESF();

		
		//SymbolicMatrix C(A.m_HorizontalDimensional, A.m_HorizontalDimensional, "C");
		//SymbolicMatrix D(A.m_HorizontalDimensional, A.m_HorizontalDimensional, "D");
		//SymbolicMatrix Delta(3, 3, "\\Delta");

		//AdmissiblePerturbations(desc, C, D);


				

		/*DAE<MatrixExpression> daeWithPerturbation(A, B - C * Delta * D);

		DAE<MatrixExpression> esfPerturbed = desc.R.ApplyTo(daeWithPerturbation);*/



		tex << "Рассмотрим уравнение: " << endl;

		tex << dae.AsLatexCode() << endl;
		

		tex << "$det A =" << A.Det() << "$" << endl;

		tex << "$det B =" << B.Det() << "$" << endl;


		tex << "Приведем это уравнение к эквивалентной структурной форме. Для этого построим матрицу Dr:" << endl;

		tex << PrintESFDescAsLatexCode(desc) << endl;

		tex << "Эквивалентная структурная форма" << endl;

		tex << esf.AsLatexCode() << endl;
				

		RealMatrix C({ 0.f, 0.f, 0.f,
			0.f, 1.f, 0.f,
			0.f, 0.f, 0.f });

		RealMatrix D({ 0.f, 0.f, 0.f,
						1.f, 0.f, 0.f,
						0.f, 0.f, 0.f });	

		RealMatrix Delta({ 1000.f, 1000.f, 1000.f,
			1000.f, 1000.f, 1000.f,
			1000.f, 1000.f, 1000.f });

		RealMatrix Phi_0_1 = (desc.R[0] * C * Delta * D).GetSubMatrix(0, 0, 2, 1);
		RealMatrix Phi_0_2 = (desc.R[0] * C * Delta * D).GetSubMatrix(0, 1, 2, 2);
		RealMatrix Phi_0_3 = (desc.R[0] * C * Delta * D).GetSubMatrix(2, 0, 1, 1);
		RealMatrix Phi_0_4 = (desc.R[0] * C * Delta * D).GetSubMatrix(2, 1, 1, 2);

		RealMatrix Phi_1_1 = (desc.R[1] * C * Delta * D).GetSubMatrix(0, 0, 2, 1);
		RealMatrix Phi_1_2 = (desc.R[1] * C * Delta * D).GetSubMatrix(0, 1, 2, 2);
		RealMatrix Phi_1_3 = (desc.R[1] * C * Delta * D).GetSubMatrix(2, 0, 1, 1);
		RealMatrix Phi_1_4 = (desc.R[1] * C * Delta * D).GetSubMatrix(2, 1, 1, 2);
		
		cout << "Условие 1:" << endl << endl;

		cout << (desc.R[2] * C).AsString();

		
		cout << "Условие 2:" << endl << endl;

		cout << "(desc.R[1] * C * Delta * D).AsString()" << endl;
		cout << (desc.R[1] * C * Delta * D).AsString() << endl;

		cout << "Phi_1_2" << endl;
		cout << Phi_1_2.AsString() << endl;

		cout << "Phi_1_4" << endl;
		cout << Phi_1_4.AsString() << endl;


		cout << "Условие 3:" << endl << endl;
		cout << "Phi_1_3.NormSpectral()" << endl;
		cout << Phi_1_3.NormSpectral() << endl;


		RealMatrix E_n_d = IdentityMatrix(Phi_1_3);

		cout << "Условие 4:" << endl << endl;
		cout << "expr.NormSpectral()" << endl;
		auto cond4Mrhs2 = E_n_d + Phi_1_3;
		auto cond4Mrhs = Phi_1_1*(cond4Mrhs2).Inverse() * Phi_0_4;
		auto cond4M = Phi_0_2 - cond4Mrhs;
		cout << cond4M.NormSpectral() << endl;
		
		



		DAE<RealMatrix> perturbedDae(A, B + C *Delta*D);
		
		perturbedDae = desc.R.ApplyTo(perturbedDae);

		cout << "perturbedDae.AsString()" << endl;
		cout << perturbedDae.AsString() << endl;

		tex << "Перейдем к рассмотрению возмущенного уравнения:" << endl;

		tex << perturbedDae.AsLatexCode() << endl;




		//tex << desc.Dr.AsLatexCode() << endl;

		//tex << "Следовательно " << desc.AsLatexParams() << endl;


		//tex << "Перейдем к рассмотрению возмущенного уравнения. Допустимый вид" << endl;

		//tex << "\\begin{equation}" << endl;
		//
		//tex << C <<  endl;
		//tex << D << endl;

		//tex << "\\end{equation}" << endl;

		/*tex << "Перейдем к рассмотрению возмущенного уравнения: " << daeWithPerturbation << endl;

		AdmissiblePerturbations(desc, C, D);
		daeWithPerturbation = DAE(A, B - C * Delta * D);

		tex << "Согласно лемме, уравнение с допустимым видом возмущенний имеет вид: " << daeWithPerturbation << endl;

		daeWithPerturbation = DAE(A, B - C * Delta * D);

		tex << "Теперь проиллюстрируем оценку радиуса робастной устойчивости для примера: " << daeWithPerturbation << endl;*/

		
	};
		
	//{
	//	RealMatrix A =
	//	{
	//		1, 0, -1,
	//		0, 0, -1,
	//		0, 0, 0
	//	};

	//	//Индекс 2
	//	RealMatrix B({
	//		2, -1, -2,
	//		0, -1, 2,
	//		0, 0, 1
	//	});

	//	doit(A,B, "Пример 1");
	//}



	//{
	//	RealMatrix A =
	//	{
	//		1, 0, -1,
	//		0, 0, -1,
	//		0, 0, 1
	//	};

	//	//Индекс 2
	//	RealMatrix B({
	//		2, -1, -2,
	//		0, -1, 2,
	//		0, 0, 1
	//	});

	//	doit(A, B, "Пример 2");
	//}

	{


		RealMatrix A =
		{
			1, 2, 3,
			4, 5, 6,
			7, 8, 9
		};

		//Индекс 2
		RealMatrix B{
			4, 3, 1,
			2, 1, 0,
			1, 0, 0
		};


		doit(A, B, "Пример 3");
	}


	/*texFile.Write();
	texFile.Texify(true);*/

	cout << "Finished "<< endl;



}



