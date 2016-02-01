// DetMinor.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include "Matricies.h"



const uint32_t MAX_INDEX = 500;




void Compute_Dr(const Matrix& A, const Matrix& B)
{
	////результаты
	Matrix Dr, Gamma;
//	OperatorR R;
	Matrix Q;
	///
	///плюс информация о строках, которые вошли в разрешающий минор

	Matrix Br = B;
	Matrix Ar = A;
//	Matrix Lambdar = BuilFirstLambdarMatrix(A, B);

	for (uint32_t c_index = 1, size_index = MAX_INDEX; c_index <= size_index; ++c_index)
	{
		//GrowBrOrArMatrix(Br);
		//GrowBrOrArMatrix(Ar);
		//GrowLambdarMatrix(Lambdar, A, B);
		//
		//MatrixDr Curr_Dr( Br,  Ar, Lambdar	);

		//MatrixGamma Curr_Gamma;
		//if (Curr_Dr.CheckConditionAndComputeMatrixGamma(Curr_Gamma))
		//{
		//	break;
		//}
	}


}

int main()
{
	try
	{
		Matrix A =
		{
			1, 0, 0,
			0, 1, 0,
			0, 0, 1
		};

		Matrix B = {
			1,2,3,
			4,5,6,
			7,8,8
		};

		cout << "B = " << B.AsString() << endl;
		cout << "B.Det() = " << B.Det() << endl;

		cout << "A = " << A.AsString() << endl;
		cout << "A.Det() = " << A.Det() << endl;

		

		cout << "A * B = " << (A * B).AsString() << endl;

		cout << (A * B).Det() << endl;





		//Compute_Dr(A, B);

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

