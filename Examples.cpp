#include "stdafx.h"
#include "Examples.h"

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
