#pragma once

#include "Matricies.h"
#include "DAE.h"


const uint32_t MAX_INDEX = 5;



//
//void Compute_ESF(const Matrix& A, const Matrix& B)
//{
//	using namespace std::rel_ops;
//	
//	uint32_t n = A.m_HorizontalDimensional;
//
//
//	Matrix O = ZeroMatrix(A);
//
//	Matrix Br = B;
//
//	Br.GrowBottom(O);
//
//	Matrix Ar(A);
//	Ar.GrowBottom(B);
//
//	Matrix Lambdar(2, 1, {
//		O,
//		A,
//	});
//
//	uint32_t rankLambdar = Lambdar.Rank();
//
//	uint32_t index = 1;
//	for (; index < MAX_INDEX; ++index)
//	{
//		cout << "checking index " << index << ": " << endl;
//
//
//		Matrix LambdarNext = Lambdar;
//		GrowLambdarMatrix(LambdarNext, A, B);
//
//		uint32_t rankLambdarNext = LambdarNext.Rank();
//		cout << "LambdarNext.Rank() = " << LambdarNext.Rank() << endl;
//
//		if (rankLambdarNext - rankLambdar == n)
//		{
//
//			break;
//		}
//
//		Br.GrowBottom(O);
//		Ar.GrowBottom(O);
//
//		Lambdar = LambdarNext;
//		rankLambdar = Lambdar.Rank();
//
//	}
//
//	if (index == MAX_INDEX)
//	{
//		throw logic_error("more then MAX_INDEX");
//	}
//
//
//	cout << "-------------" << endl;
//	cout << "!!!!!" << endl;
//	cout << "-------------" << endl;
//
//	cout << "Result Index is " << index << endl;
//
//	MatrixDr Dr(Br, Ar, Lambdar);
//
//	cout << "Dr = " << Dr.Print() << endl;
//
//	cout << "Rank Dr = " << Dr.Rank() << endl;
//
//	cout << "Ranked Dr = " << Dr.PrintRankedMatrix() << endl;
//
//	cout << "-------------" << endl;
//	cout << "d = " << Dr.m_D << " " << "rank Lambda = " << Dr.m_Lambdar.Rank() << endl;
//
//
//
//
//	Matrix Q = Dr.GetMatrixQ();
//	cout << "Q = " << Q.AsString() << endl;
//
//	Matrix Gamma = Dr.GetMatrixGamma();
//	cout << "Gamma = " << Gamma.AsString() << endl;
//
//	OperatorR R = Dr.GetOperatorR();
//
//	cout << "R = " << R.AsString() << endl;
//
//
//	cout << "-------------" << endl;
//	cout << "Structural form:" << endl << endl;
//
//
//	DAE esf = R.ApplyTo(A, B, Q);
//
//
//	cout << esf.AsString() << endl;
//
//	cout << "Without zeros:" << endl << endl;
//
//	esf.EraseZeroMatrix();
//
//	cout << esf.AsString() << endl;
//}




void ArticleExample();
void Test();
void NewFeaturesTest();

void FindNewExamples();
void NewExamples();