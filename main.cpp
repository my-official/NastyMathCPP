// DetMinor.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include "Matrix\RealMatrix.h"
#include "DAE.h"
#include "Examples.h"










int main()
{
	int result = 0;
	try
	{
		cout << std::boolalpha;
		//Test();
		//ArticleExample();
		
		
		//SymbolicTest();
		//NewFeaturesTest();
		//FindNewExamples();
		//LatexTest();

		//		GinacTest();


		//NewExamples();		
		//ExampleB();
		//ExampleAB();
		//teacher();
		//TestRank();
		t2();
		//NewArticleBlockMatrixMultiplication();
	//	FindNewExamplesEx();
	//	TestEx();

		//ParallelForEachMatrixTest();
		//SymbolicMatrixTest();
		//FindNewExamplesEx();
	//	LatexNewArticleExamples();
	
		
	}
	catch (std::exception& e)
	{
		cerr << "std::exception: " << e.what() << endl;				
		result = __LINE__;
	}
	catch (...)
	{
		cerr << "unknown exception" << endl;		
		result = __LINE__;		
	}

	if (result != 0)
	{
		char symbol;
		cin >> symbol;
	}	

    return result;
}


