// DetMinor.cpp: ���������� ����� ����� ��� ����������� ����������.
//

#include "stdafx.h"
#include "Matrix\RealMatrix.h"
#include "DAE.h"
#include "Examples.h"










int main()
{
	try
	{
		cout << std::boolalpha;
		ArticleExample();		
	}
	catch (std::exception& e)
	{
		cerr << "std::exception: " << e.what() << endl;		
	}
	catch (...)
	{
		cerr << "unknown exception" << endl;		
	}

	cout << endl << endl << "All finished !!!" << endl << endl;

	char symbol;
	cin >> symbol;
	

    return 0;
}


