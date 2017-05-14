#pragma once

#include "BaseMatrix.h"





struct FCmpPred
{
	bool operator() (float x, float y) const
	{
		return FCmp(x, y);
	}
	typedef float first_argument_type;
	typedef float second_argument_type;
	typedef bool result_type;
};

struct IsFZeroPred
{
	bool operator() (float x) const
	{
		return IsFZero(x);
	}
	typedef float first_argument_type;
	typedef bool result_type;
};


class RealMatrix : public BaseMatrix<float, RealMatrix, FCmpPred, IsFZeroPred>
{
public:

	using BaseMatrix::BaseMatrix;
	virtual ~RealMatrix()
	{
	}

	virtual Eigen::MatrixXf AsEigenMatrix() const;

	virtual vector<complexF> Eigenvalues() const;
	virtual vector<float> SingularValues() const;
	virtual float NormSpectral() const;

	virtual bool IsStable() const;

	template <typename Fn>
	static void ForAllMatrix(uint32_t vertical, uint32_t horizontal, float beginValue, float endValue, float step, Fn fn);

	using MatrixHandlerFuncThreaded = function<void(const RealMatrix&)>;
	static void ForAllMatrixThreaded(uint32_t vertical, uint32_t horizontal, float beginValue, float endValue, float step, const MatrixHandlerFuncThreaded& fn);
	
private:
	class ThreadManager
	{
	public:
		ThreadManager();
		void AddMatrix(const MatrixHandlerFuncThreaded& fn, const RealMatrix& m);
		void Exec(const MatrixHandlerFuncThreaded&  fn);
	private:
		size_t m_CurrentThread = 0;
		size_t m_CurrentSize = 0;
		using Matricies = vector<RealMatrix>;

		static const size_t JobSize;

		struct Job
		{
			Matricies m_Matricies;
			future<void> m_Future;
		};
		vector<Job> m_Job2Threads;

		void ExecPerOne(const MatrixHandlerFuncThreaded& fn, const Matricies& matricies);		
	};

	template <typename Fn>
	static void ForAllMatrixInternal(RealMatrix& M, uint32_t target_idx, float beginValue, float endValue, float step, Fn fn);
	static void ForAllMatrixThreadedInternal(ThreadManager& threadManager, RealMatrix& M, uint32_t target_idx, float beginValue, float endValue, float step, const MatrixHandlerFuncThreaded&  fn);
	static void ForAllMatrixInternalRefill(RealMatrix& M, uint32_t target_idx, float beginValue);
};








//////////////////////////////////////////////////////
//////////////////////////////////////////////////////





template <typename Fn>
void RealMatrix::ForAllMatrix(uint32_t vertical, uint32_t horizontal, float beginValue, float endValue, float step, Fn fn)
{
	if (vertical == 0 || horizontal == 0)
		throw EXCEPTION;

	RealMatrix M(vertical, horizontal);

	for (auto& value : M.m_Data)
	{
		value = beginValue;
	}

	fn(M);

	ForAllMatrixInternal(M, (uint32_t)M.m_Data.size(), beginValue, endValue, step, fn);
}

template <typename Fn>
void RealMatrix::ForAllMatrixInternal(RealMatrix& M, uint32_t target_idx, float beginValue, float endValue, float step, Fn fn)
{

	for (uint32_t idx = 0; idx < target_idx; ++idx)
	{
		//M.m_Data[idx] += step;

		for (float c = beginValue + step; c <= endValue; c += step)
		{
			M.m_Data[idx] = c;
			ForAllMatrixInternalRefill(M, idx, beginValue);
			fn(M);

			ForAllMatrixInternal(M, idx, beginValue, endValue, step, fn);
			//	ForAllMatrixInternal(M, idx, beginValue, endValue, step, fn);
		}
	}

}