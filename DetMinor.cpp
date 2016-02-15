// DetMinor.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include "Matricies.h"
#include "DAE.h"
#include "Examples.h"








int main()
{
	try
	{
		//Test();
		//ArticleExample();
		//NewExamples();
		NewFeaturesTest();
		//FindNewExamples();


		if (0)
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


			cout << "-------------" << endl;
			cout << "Introduction perturbation" << endl;			
			cout << "-------------" << endl;

			Matrix Delta({
				101, 102, 103,
				102, 103, 104,
				103, 505, 105
			});
			
			cout << "Delta.Rank ==" << Delta.Rank() << endl;

			Matrix C({
				72, -15, 75,
				54, 86, 55,
				0, 0, 0
			});

		//	cout << "C.Rank ==" << C.Rank() << endl;

			Matrix D({
				88, 0, 0,
				76, 0, 0,
				7, 0, 0
			});

			DAE daeWithPerturbation(A, B + C* Delta * D);

			DAE esfPerturbed = desc.R.ApplyTo(daeWithPerturbation);
			
			cout << esfPerturbed.AsString() << endl << endl;


		}

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

