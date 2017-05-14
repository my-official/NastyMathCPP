#include "stdafx.h"
#include "RealMatrix.h"







//RealMatrix::RealMatrix(BaseMatrix<float>&& rhs) : RealMatrix( static_cast<BaseMatrix<float>&&>(rhs))
//{
//
//}

Eigen::MatrixXf RealMatrix::AsEigenMatrix() const
{
	Eigen::MatrixXf m(m_VerticalDimensional, m_HorizontalDimensional);
	for (uint32_t y = 0; y < m_VerticalDimensional; ++y)
	{
		for (uint32_t x = 0; x < m_HorizontalDimensional; ++x)
		{
			m(y, x) = (*this)[{y, x}];
		}
	}
	return m;
}


std::vector<complexF> RealMatrix::Eigenvalues() const
{
	if (m_HorizontalDimensional != m_VerticalDimensional)
		throw EXCEPTION;

	vector<complexF> result;

	using namespace Eigen;

	
	EigenSolver<MatrixXf> solver(AsEigenMatrix());

	for (uint32_t c = 0, size = (uint32_t)solver.eigenvalues().size(); c < size; c++)
	{
		result.push_back(solver.eigenvalues()[c]);
	}	

	return result;
}

std::vector<float> RealMatrix::SingularValues() const
{
	std::vector<float> result;

	using namespace Eigen;
	
	JacobiSVD<MatrixXf> solver(AsEigenMatrix());

	for (uint32_t c = 0, size = (uint32_t)solver.singularValues().size(); c < size; c++)
	{
		result.push_back(solver.singularValues()[c]);
	}

	return result;
}
//
//std::vector<complexF> RealMatrix::Eigenvalues2() const
//{
//	if (m_HorizontalDimensional != m_VerticalDimensional)
//		throw EXCEPTION;
//
//	vector<complexF> result;
//	switch (m_HorizontalDimensional)
//	{
//		case 1:
//		{
//			result.emplace_back( m_Data[0], 0.f);
//		}
//		break;
//		case 2:
//		{
//			//lambda = 0.5f * A.trace() +- 0.5f * sqrt( sqr(A.trace()) - 4 * A.Det() ) )
//			
//			float trace = Trace();
//			float det = Det();
//						
//			complexF t(trace*trace - 4.f * det, 0.f);
//			t = sqrt(t);
//
//			complexF eigenvalue1 = 0.5f * (trace + t);
//			complexF eigenvalue2 = 0.5f * (trace - t);
//
//			result.push_back(eigenvalue1);
//			if (!FCmp( eigenvalue1,  eigenvalue2) )
//			{
//				result.push_back(eigenvalue2);
//			}
//		}
//		break;
//		case 3:
//		{
//			//y^3 + p*y + q = 0
//
//			//a*x^3 + b*x^2 + c*x + d = 0
//
//			//x = y - b / (3*a)
//			
//			//p = (3*a*c-b*b) / ( 3*a*a)
//
//			//q = (2*b^3) / ( 3*a*a)
//
//
//			//x^3 + a*x^2 + b*x + c = 0
//
//			///k3
//
//			RealMatrix A = *this;
//			float trace = A.Trace();
//
//			float a = -Trace();
//			float b = -0.5f*( (A*A).Trace() - trace*trace);
//			float c = -Det();
//
//			float Q = (a*a - 3*b) / 9;
//			float R = (2*a*a*a - 9*a*b + 27*c) / 54;
//			float S = Q*Q*Q-R*R;
//
//			float a_div_3 = a / 3.f;
//
//			if ( IsFZero(S) )
//			{				
//				complexF eigenvalue1( -2.f*cbrt(R) - a_div_3, 0.f);
//				complexF eigenvalue2( cbrt(R) - a_div_3, 0.f);
//
//				result.push_back(eigenvalue1);
//				result.push_back(eigenvalue2);
//			}
//			else
//			{
//				if (S > 0.0f)
//				{
//					float phi = (1.f/3.f)*acos( R / sqrt(Q*Q*Q) );
//					const float Pi = 3.1416f;
//
//					complexF eigenvalue1( -2.f * sqrt(Q) * cos(phi) - a_div_3, 0.f);
//					complexF eigenvalue2( -2.f * sqrt(Q) * cos(phi + (2.f / 3.f)*Pi) - a_div_3, 0.f);
//					complexF eigenvalue3( -2.f * sqrt(Q) * cos(phi - (2.f / 3.f)*Pi) - a_div_3, 0.f);
//
//					result.push_back(eigenvalue1);
//					result.push_back(eigenvalue2);
//					result.push_back(eigenvalue3);
//				}
//				else
//				{
//					if (IsFZero(Q))
//					{		
//						float x1 = -cbrt(c - a*a*a / 27.f) - a_div_3;
//						complexF eigenvalue1(x1, 0.f);
//
//						
//						float rePart = -0.5f*(a + x1);
//						float imPart = 0.5f* sqrt(std::abs( (a - 3.f* x1) * (a + x1) - 4.f*b) );
//
//						complexF eigenvalue2(rePart, imPart);
//						complexF eigenvalue3(rePart, -imPart);						
//
//						result.push_back(eigenvalue1);
//						result.push_back(eigenvalue2);
//						result.push_back(eigenvalue3);
//					}
//					else
//					{
//						if (Q > 0.0f)
//						{
//							float phi = (1.f / 3.f)*acosh( std::abs(R) / sqrt(Q*Q*Q));
//
//							complexF eigenvalue1(-2.f * sgn(R) * sqrt(Q) * cosh(phi) - a_div_3, 0.f);
//							complexF eigenvalue2(sgn(R) * sqrt(Q) * cosh(phi) - a_div_3,  sqrt(3.f)*sqrt(Q)*sinh(phi));
//							complexF eigenvalue3(eigenvalue2.real(), -eigenvalue2.imag());
//
//							result.push_back(eigenvalue1);
//							result.push_back(eigenvalue2);
//							result.push_back(eigenvalue3);
//						}
//						else
//						{
//							float phi = (1.f / 3.f)*asinh(std::abs(R) / sqrt( std::abs(Q*Q*Q) ) );
//
//							complexF eigenvalue1(-2.f * sgn(R) * sqrt(std::abs(Q)) * sinh(phi) - a_div_3, 0.f);
//							complexF eigenvalue2(sgn(R) * sqrt(std::abs(Q)) * sinh(phi) - a_div_3, sqrt(3.f)*sqrt(std::abs(Q))*cosh(phi));
//							complexF eigenvalue3(eigenvalue2.real(), -eigenvalue2.imag());
//
//							result.push_back(eigenvalue1);
//							result.push_back(eigenvalue2);
//							result.push_back(eigenvalue3);
//						}
//					}
//				}
//			}
//
//
//
//		}
//		break;
//		default:
//			throw EXCEPTION;
//		break;
//	}
//	return result;
//}

float RealMatrix::NormSpectral() const
{
	return AsEigenMatrix().operatorNorm();	
}

bool RealMatrix::IsStable() const
{
	auto eigenvalues = Eigenvalues();

	for (auto& eigenvalue : eigenvalues)
	{
		if (eigenvalue.real() <= 0.f)
			return false;
	}
	return true;
}

void RealMatrix::ForAllMatrixThreaded(uint32_t vertical, uint32_t horizontal, float beginValue, float endValue, float step, const MatrixHandlerFuncThreaded& fn)
{
	assert(fn);

	if (vertical == 0 || horizontal == 0)
		throw EXCEPTION;

	RealMatrix M(vertical, horizontal);

	for (auto& value : M.m_Data)
	{
		value = beginValue;
	}

	fn(M);

	ThreadManager threadManager;

	ForAllMatrixThreadedInternal(threadManager, M, (uint32_t)M.m_Data.size(), beginValue, endValue, step, fn);
	threadManager.Exec(fn);
}

void RealMatrix::ForAllMatrixThreadedInternal(ThreadManager& threadManager, RealMatrix& M, uint32_t target_idx, float beginValue, float endValue, float step, const MatrixHandlerFuncThreaded& fn)
{
	for (uint32_t idx = 0; idx < target_idx; ++idx)
	{
		//M.m_Data[idx] += step;

		for (float c = beginValue + step; c <= endValue; c += step)
		{
			M.m_Data[idx] = c;
			ForAllMatrixInternalRefill(M, idx, beginValue);

			threadManager.AddMatrix(fn, M);
			

			ForAllMatrixThreadedInternal(threadManager, M, idx, beginValue, endValue, step, fn);
			//	ForAllMatrixInternal(M, idx, beginValue, endValue, step, fn);
		}
	}	
}

//
//void RealMatrix::Svd(RealMatrix& U, RealMatrix& Sigma, RealMatrix& V) const
//{
//
//}


void RealMatrix::ForAllMatrixInternalRefill(RealMatrix& M, uint32_t target_idx, float beginValue)
{
	for (uint32_t idx = 0; idx < target_idx; ++idx)
	{
		M.m_Data[idx] = beginValue;		
	}
}

RealMatrix::ThreadManager::ThreadManager() : m_Job2Threads(thread::hardware_concurrency())
{
	assert(thread::hardware_concurrency() > 0);
	for (auto& job : m_Job2Threads)
	{
		job.m_Matricies.reserve(JobSize);
	}
}

void RealMatrix::ThreadManager::AddMatrix(const MatrixHandlerFuncThreaded& fn, const RealMatrix& m)
{
	m_Job2Threads[m_CurrentThread].m_Matricies.push_back(m);
	++m_CurrentThread;
	if (m_CurrentThread >= m_Job2Threads.size())
	{
		m_CurrentThread = 0;
		++m_CurrentSize;

		if (m_CurrentSize >= JobSize)
		{
			Exec(fn);			
			m_CurrentSize = 0;
		}
		
	}
}

void RealMatrix::ThreadManager::Exec(const MatrixHandlerFuncThreaded& fn)
{
	for (auto& job : m_Job2Threads)
	{
		auto triple = std::mem_fn(&ThreadManager::ExecPerOne);
		job.m_Future = async(launch::async, triple, this, fn, job.m_Matricies);
	}
	
	exception_ptr e;

	for (auto& job : m_Job2Threads)
	{
		try
		{
			job.m_Future.get();
		}
		catch (...)
		{
			if (!e)
			{
				e = current_exception();
			}
		}
		
		job.m_Matricies.clear();
	}	

	if (e)
	{
		rethrow_exception(e);
	}
}

const size_t RealMatrix::ThreadManager::JobSize = 10000;

void RealMatrix::ThreadManager::ExecPerOne(const MatrixHandlerFuncThreaded& fn, const Matricies& matricies)
{
	for (auto& m : matricies)
	{
		fn(m);
	}
}


