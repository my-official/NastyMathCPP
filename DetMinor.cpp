// DetMinor.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include "Matricies.h"

using namespace std::rel_ops;



const uint32_t MAX_INDEX = 5;







void Compute_Dr(const Matrix& A, const Matrix& B)
{
	uint32_t n = A.m_HorizontalDimensional;
		

	Matrix O = ZeroMatrix(A);

	Matrix Br = B;

	Br.GrowBottom(O);

	Matrix Ar( A );
	Ar.GrowBottom(B);

	Matrix Lambdar(2,1, { 
		O,
		A,
	});

	uint32_t rankLambdar = Lambdar.Rank();
		
	uint32_t index = 1;
	for (; index < MAX_INDEX; ++index)
	{
		cout << "checking index " << index << ": " << endl;

		
		Matrix LambdarNext = Lambdar;
		GrowLambdarMatrix(LambdarNext, A, B);
		
		uint32_t rankLambdarNext = LambdarNext.Rank();
		cout << "LambdarNext.Rank() = " << LambdarNext.Rank() << endl;

		if (rankLambdarNext - rankLambdar == n)
		{
			
			break;
		}
		
		Br.GrowBottom(O);
		Ar.GrowBottom(O);

		Lambdar = LambdarNext;
		rankLambdar = Lambdar.Rank();

	}

	if (index == MAX_INDEX)
	{
		throw logic_error("more then MAX_INDEX");
	}


	cout << "-------------" << endl;
	cout << "!!!!!" << endl;
	cout << "-------------" << endl;

	cout << "Result Index is " << index << endl;

	MatrixDr Dr(Br, Ar, Lambdar);

	cout << "Dr = " << Dr.Print() << endl;

	cout << "Rank Dr = " << Dr.Rank() << endl;

	cout << "Ranked Dr = " << Dr.PrintRankedMatrix() << endl;

	cout << "-------------" << endl;
	cout << "d = " << Dr.m_D << " "<< "rank Lambda = " << Dr.m_Lambdar.Rank() << endl;
	



	Matrix Q = Dr.GetMatrixQ();
	cout << "Q = " << Q.AsString() << endl;

	Matrix Gamma = Dr.GetMatrixGamma();
	cout << "Gamma = " << Gamma.AsString() << endl;

	OperatorR R = Dr.GetOperatorR();
		
	cout << "R = " << R.AsString() << endl;


	cout << "-------------" << endl;
	cout << "Structural form:" << endl << endl;
	

	DAE esf = R.ApplyTo(A,B, Q);

	
	cout << esf.AsString() << endl;

	cout << "Without zeros:" << endl << endl;

	esf.EraseZeroMatrix();

	cout << esf.AsString() << endl;





	////результаты
	//MatrixDr Dr, Gamma;
	////	OperatorR R;
	//Matrix Q;
	///
	///плюс информация о строках, которые вошли в разрешающий минор
	

}

int main()
{
	try
	{

		//int i = 0;

		//Matrix::ForAllMatrix(2,2, -5.f, 5.f, 1.f,
		//[&i](const Matrix& m)
		//{ 	
		//	i++;
		//	//cout << m.AsString() << endl;

		//	

		//	if (IsFZero( m.Det() ))
		//	{
		//		return;
		//	}

		//	if ((m.Inverse() * m) != IdentityMatrix(m))
		//		throw logic_error( ("assert1 inverse " + m.AsString()).c_str() );

		//	if ((m.Inverse() * m) != (m * m.Inverse()))
		//		throw logic_error(("assert2 inverse " + m.AsString()).c_str());
		//});

		//cout << "Test success!!!! i == " << i << endl;
		
	
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

			Compute_Dr(A, B);


	}
	catch (const std::exception& e)
	{
		cerr << "std::exception: " << e.what() << endl;
	}
	catch (...)
	{
		cerr << "unknown exception" << endl;
	}

	char symbol;
	cin >> symbol;

    return 0;
}

