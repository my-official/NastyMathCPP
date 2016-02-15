#include "stdafx.h"
#include "Examples.h"
#include "Symbolic.h"


void ArticleExample()
{
	Matrix A =
	{
		1, 0, -1,
		0, 0, -1,
		0, 0, 0
	};

	Matrix B({
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


	DAE dae(A, B);

	auto desc = dae.ConvertToESF();

	PrintESFDesc(desc);

	cout << "-------------" << endl;
	cout << "Structural form:" << endl << endl;

	cout << dae.AsString() << endl << endl;
}

void Test()
{
	Matrix A =
	{
		1, 0, -1,
		0, 0, -1,
		0, 0, 0
	};

	int num = 0;
	int prevNum = 100000;
	int numUsed = 0;

	int numNotIdenticalQ = 0;

	Matrix::ForAllMatrix(3, 3, 1.f, 4.f, 1.f,
		[&num, &prevNum, &numUsed, &numNotIdenticalQ, &A](const Matrix& B)
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


		DAE dae(A, B);

		auto desc = dae.ConvertToESF();


		if (desc.R.m_Rs.size() == 3)
		{
			if (!desc.R.m_Rs[2].IsZeroMatrix())
				throw logic_error("Test failed B = " + B.AsString());
		}


		if (!desc.Q.IsIdentityMatrix())
		{
			++numNotIdenticalQ;
		}

		if (dae.GetOrder() > 1)
		{
			throw logic_error("Test failed B = " + B.AsString());
		}

		numUsed++;


	});

	cout << "Test success!!!! num == " << num
		<< " numUsed == " << numUsed
		<< " numNotIdenticalQ == " << numNotIdenticalQ << endl;
}


///////////////////

class Expression;

class Symbolic
{
	vector<Expression> m_Expressions;



};

class Summand
{
	vector<Symbolic> m_Multipliers;
};


class Expression
{
public:
	vector<Summand> m_Summands;
};


void NewFeaturesTest()
{
	Symbolic a("a");
	Symbolic b("b");

	auto c = a + b;

	cout << "c == " << c.AsString() << endl;

	c.Substitute("a", 5);

	cout << "c == " << c.AsString() << endl;

	c.Substitute("b", 2);

	cout << "c == " << c.AsString() << endl;

	cout << std::boolalpha;

	cout << "c == " << c.IsSymbolic() << endl;

	

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




	/*Matrix A =
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

	Matrix A =
	{
		1, 0, -1, 2,
		0, 0, -1, 5,
		0, 0, 0, 1,
		8, 0, 7,1
	};

	cout << "A.Det = " << A.Det() << endl;
	cout << "A.Ranked = " << A.AsStringRanked() << endl;


	Matrix B =
	{
		1, 1, 4, 1,
		1, 4, 1, 1,
		2, 1, 1, 1,
		1, 1, 1,2
	};

	cout << "B.Det = " << B.Det() << endl;
	cout << "B.Ranked = " << B.AsStringRanked() << endl;


	DAE dae(A, B);

	auto desc = dae.ConvertToESF();



	Matrix::ForAllMatrix(4, 4, 1.f, 4.f, 1.f,
		[&A](const Matrix& B)
	{
		if (IsFZero(B.Det()))
		{
			return;
		}

		if (IsFZero((A + B).Det()))
		{
			return;
		}


		DAE dae(A, B);

		auto desc = dae.ConvertToESF();

		if (dae.GetOrder() > 1)
		{
			throw logic_error("Test failed B = " + B.AsString());
		}


		if (desc.Index >= 2)
		{
			throw logic_error("You matrix B = " + B.AsString());
		}
	}
	);
	
	cout << "Finished" << endl;
	//cout << B.IsStable() << endl;
}

void NewExamples()
{
	Matrix A =
	{
		1, 0, -1,
		0, 0, -1,
		0, 0, 0
	};

	//Индекс 2
	Matrix B({
		2, -1, -2,
		0, -1, 2,
		0, 0, 1
	});


	//Индекс 1
	//Matrix B({
	//	2, -1, -2,
	//	0, -1, 2,
	//	0, 1, 1
	//});


	//Индекс 3
	//Matrix B({
	//	-1, -2, -2,
	//	-2, -1, -2,
	//	-2, 0, -2
	//});

	


	cout << "Input Data :" << endl;
	cout << endl;

	cout << "A = " << A.AsString() << endl;
	cout << "A.Rank() = " << A.Rank() << endl;

	cout << "B = " << B.AsString() << endl;
	cout << "B.Rank() = " << B.Rank() << endl;
	cout << "B.Det() = " << B.Det() << endl;

	cout << endl;
	cout << "-------------" << endl;
	cout << "Compute_Dr" << endl;
	cout << "-------------" << endl;
	cout << endl;

	DAE dae(A, B);

	auto desc = dae.ConvertToESF();

	PrintESFDesc(desc);

	cout << "-------------" << endl;
	cout << "Structural form:" << endl << endl;

	cout << dae.AsString() << endl << endl;


	cout << "-------------" << endl;
	cout << "Introduction perturbation" << endl;
	cout << "-------------" << endl;

	Matrix Delta({
		0.4f, 0, 0,
		0, 0.4f, 0,
		0, 0, 0.4f
	});

	cout << "Delta.Rank ==" << Delta.Rank() << endl;

	Matrix C({
		7, -2, 1,		
		5, 7, 1,
		0, 0, 0
	});

	cout << "C.Rank ==" << C.Rank() << endl;
	cout << "C.Det ==" << C.Det() << endl;

	cout << "C*R1 = " <<  (desc.R.m_Rs[1]*C).AsString() << endl;
	


	//	cout << "C.Rank ==" << C.Rank() << endl;

	Matrix D({
		5, 0, 0,
		8, 0, 0,
		1, 0, 0
	});

	DAE daeWithPerturbation(A, B - C * Delta * D);

	DAE esfPerturbed = desc.R.ApplyTo(daeWithPerturbation);

	cout << esfPerturbed.AsString() << endl << endl;


	


	cout << "-------------" << endl;
	cout << "Robust stability" << endl;
	cout << "-------------" << endl;

	Matrix J2 = dae.m_Coefficients[0].GetSubMatrix(2, 0, 1, 1);

	cout << "J2 == " << J2.AsString() << endl;
	cout << "J2.IsStable() ==  " << J2.IsStable() << endl;
	
	cout << "R0*C == " << (desc.R.m_Rs[0] * C).AsString() << endl;


	cout << "C.Norm == " << C.NormSpectral() << endl;
	cout << "D.Norm == " << D.NormSpectral() << endl;
	cout << "Delta.Norm == " << Delta.NormSpectral() << endl;
	cout << "(R0*C).Norm == " << (desc.R.m_Rs[0] * C).NormSpectral() << endl;

	cout << "(C*Delta*D).Norm == " << (C*Delta*D).NormSpectral() << endl;
	



	return;
	cout << endl;
	cout << "Perturbation Only" << endl;
	cout << "-------------" << endl;


	DAE perturbationOnly(ZeroMatrix(A), C * Delta * D);


	DAE esfPerturbationOnly = desc.R.ApplyTo(perturbationOnly);

	cout << esfPerturbationOnly.AsString() << endl << endl;


	Matrix Theta = esfPerturbationOnly.m_Coefficients[0].GetSubMatrix(2, 0, 1, 1);

	cout << "Theta == " << Theta.AsString() << endl;

	float normTheta = Theta.NormSpectral();

	cout << "|| Theta || == " << normTheta << endl;

	

}
