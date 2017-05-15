#include "stdafx.h"
#include "DAE.h"
#include "Matrix\SymbolicMatrix.h"




ODE1::ODE1(const RealMatrix& B) : m_A(IdentityMatrix(B)), m_B(B)
{

}

ODE1::ODE1(const RealMatrix& A, const RealMatrix& B) : m_A(IdentityMatrix(B)), m_B(B)
{
	if (IsFZero(A.Det()))
	{
		throw EXCEPTION;
	}

	auto AInv = A.Inverse();



}

void ODE1::CheckStability(uint32_t numIterations /*= 1000*/, float t0 /*= 0.f*/)
{
	//float delta = numeric_limits<float>::max() / float(numIterations);

	//float t = t0;

	//for (uint32_t c = 0, size = numIterations / 2; c < size; c++)
	//{
	//	float x = exp( - t)
	//	

	//	t += delta;
	//}
}

uint32_t ODE1::GetMatrixDimensional() const
{
	return m_A.m_HorizontalDimensional;
}

std::string ODE1::AsString() const
{
	string result;
	result += m_A.AsString() + " * " + "dx + " + m_B.AsString() + " * x = O";

	return result;
}



void AdmisablePerturbationDesc::GetVarriblesForPassZeroConditions(deque<Varrible>& varribles, SymbolicMatrix m, const vector<string>& varNamesForExclusion)
{
	using VarCounterMap = map<Varrible, uint32_t, bool(*)(const Varrible&, const Varrible&) >;
	//struct Var2Counter
	//{
	//	VarCounterMap Counters;
	//	VarCounterMap::iterator MaxElem_it;
	//	Var2Counter() : Counters(Varrible::alphabetical_less)
	//	{
	//	}

	//};

	SymbolicMatrix O = ZeroMatrix(m);	

	while (m != O)
	{		
		VarCounterMap varCounters(Varrible::alphabetical_less);
		//using VarGroups = map<string, Var2Counter>;
		//VarGroups varGroups;

		//for (auto&  varName : varNamesForExclusion)
		//{
		//	varGroups.emplace(varName, Var2Counter());
		//}
				
				
		for (auto& elem : m.m_Data)
		{
			vector<Varrible> vars;
			elem.GetVarribles(vars);

			for (auto& var : vars)
			{
				for (auto& varName : varNamesForExclusion)
				{
					if (var.m_Name.find(varName) == 0)
					{
						varCounters[var] += 1;						
					}
				}
			}
		}

		if (varCounters.empty())
			throw EXCEPTION;// значит задано так, что можно занулять не те переменные, которые остались

			

		auto var_it = max_element(varCounters.begin(), varCounters.end(), [](const pair<Varrible, uint32_t>& lhs, const pair<Varrible, uint32_t>& rhs)->bool
		{
			return lhs.second < rhs.second;
		});

		
		const Varrible& var = var_it->first;
		varribles.push_back(var);

		m.Substitute(var, 0);
		m.Simplify();		
	}
}





void AdmisablePerturbationDesc::AdmisableBPerturbation_Internal(const ESFDesc<RealMatrix>& desc)
{
	static_cast< ESFDesc<RealMatrix>& >(*this) = desc;
		
	C[0] = SymbolicMatrix(n, n, "c");
	Delta[0] = SymbolicMatrix(n, n, "\\delta");
	D[0] = SymbolicMatrix(n, n, "d");
	

		
	//RealMatrix E(n, n);
	//E = IdentityMatrix(E);

	//RealMatrix E_d = E.GetSubMatrix(0, 0, d, d);
	//RealMatrix E_n_d = E.GetSubMatrix(0, 0, n - d, n - d);

	if (R.m_Rs.empty())
		throw EXCEPTION;
	
	Phi.resize(R.m_Rs.size());

	for (uint32_t c = 0, size = R.m_Rs.size(); c < size; c++)
	{
		Phi[c] = (R[c] * C[0] * Delta[0] * D[0] * Q).Simplify();
	}
	
				

	deque<Varrible> varribles;
	//Cond 1  Phi_j = 0 где j >=2 

	for (uint32_t c = 2, size = R.m_Rs.size(); c < size; c++)
	{		
		Phi[c].Substitute(varribles.begin(), varribles.end(), 0);
		Phi[c].Simplify();
		GetVarriblesForPassZeroConditions(varribles, Phi[c]);					
	}	

	Phi.back().Substitute(varribles.begin(), varribles.end(), 0);
	Phi.back().Simplify();

	//SymbolicMatrix Phi_0_1 = GetSubPhi(0, 1);
	//SymbolicMatrix Phi_0_2 = GetSubPhi(0, 2);
	//SymbolicMatrix Phi_0_3 = GetSubPhi(0, 3);
	//SymbolicMatrix Phi_0_4 = GetSubPhi(0, 4);

	//SymbolicMatrix Phi_1_1 = GetSubPhi(1, 1);
	SymbolicMatrix Phi_1_2 = GetSubPhi(1, 2);
	//SymbolicMatrix Phi_1_3 = GetSubPhi(1, 3);
	SymbolicMatrix Phi_1_4 = GetSubPhi(1, 4);
		
	//Cond 2. Phi_1_2 = 0 и Phi_1_4 = 0
	
	GetVarriblesForPassZeroConditions(varribles, Phi_1_2);


	Phi_1_4.Substitute(varribles.begin(), varribles.end(), 0);
	Phi_1_4.Simplify();
		
	GetVarriblesForPassZeroConditions(varribles, Phi_1_4);

	//Phi_1_3.Substitute(varribles.begin(), varribles.end(), 0);
	//Phi_1_3.Simplify();

	//Cond 3 ||Phi_1_3|| < 1
	/*GetVarriblesForPassZeroConditions(varribles, Phi_1_3);

	Phi_0_2.Substitute(varribles.begin(), varribles.end(), 0);
	Phi_0_2.Simplify();

	Phi_1_1.Substitute(varribles.begin(), varribles.end(), 0);
	Phi_1_1.Simplify();

	Phi_0_4.Substitute(varribles.begin(), varribles.end(), 0);
	Phi_0_4.Simplify();*/


	//Cond 4 ||Phi_0_2 - Phi_1_1 * S1 * Phi_0_4|| < 1

	
	//SymbolicMatrix Cond4Matrix = Phi_0_2 - Phi_1_1 * (E_n_d + Phi_1_3).Inverse() * Phi_0_4;
	/*SymbolicMatrix Cond4Matrix = Phi_0_2 - Phi_1_1 * Phi_0_4;

	GetVarriblesForPassZeroConditions(varribles, Cond4Matrix);*/

	for (auto& phi : Phi)
	{
		phi.Substitute(varribles.begin(), varribles.end(), 0);
		phi.Simplify();
	}

	C[0].Substitute(varribles.begin(), varribles.end(), 0);
	C[0].Simplify();

	D[0].Substitute(varribles.begin(), varribles.end(), 0);
	D[0].Simplify();	


	//Cond 3.
	Cond3 = GetSubPhi(1, 3);
	Cond3.Simplify();

	//Cond 4.
	Cond4 = GetSubPhi(0, 2) + GetSubPhi(1, 1)*(IdentityMatrix(GetSubPhi(1, 3))  + GetSubPhi(1, 3)).Inverse() * GetSubPhi(0, 4);
	Cond4.Simplify();
}


OperatorR<SymbolicMatrix> AdmisablePerturbationDesc::CreateOperatorR_BPerturbed(const ESFDesc<RealMatrix>& desc)
{
	RealMatrix E(n, n);
	E = IdentityMatrix(E);

	RealMatrix E_d = E.GetSubMatrix(0, 0, d, d);
	RealMatrix E_n_d = E.GetSubMatrix(0, 0, n - d, n - d);

	SymbolicMatrix Phi_0_1 = GetSubPhi(0, 1);
	SymbolicMatrix Phi_0_2 = GetSubPhi(0, 2);
	SymbolicMatrix Phi_0_3 = GetSubPhi(0, 3);
	SymbolicMatrix Phi_0_4 = GetSubPhi(0, 4);

	SymbolicMatrix Phi_1_1 = GetSubPhi(1, 1);
	SymbolicMatrix Phi_1_2 = GetSubPhi(1, 2);
	SymbolicMatrix Phi_1_3 = GetSubPhi(1, 3);
	SymbolicMatrix Phi_1_4 = GetSubPhi(1, 4);


	SymbolicMatrix S1 = (E_n_d + Phi_1_3).Inverse().Simplify();

	SymbolicMatrix S2 = (E_d + Phi_0_2 - Phi_1_1 * S1 * Phi_0_4).Inverse().Simplify();

	SymbolicMatrix O_m1 = ZeroMatrix(SymbolicMatrix(d, n - d));
	SymbolicMatrix O_m2 = ZeroMatrix(SymbolicMatrix(n - d, d));


	SymbolicMatrix K1({
		E_d,					O_m1,
		-S1 * Phi_0_4,			E_n_d
	});

	SymbolicMatrix K2({
		S2,		O_m1,
		O_m2,	E_n_d
	});

	SymbolicMatrix K3({
		E_d,	-Phi_1_1,
		O_m2,	E_n_d
	});

	SymbolicMatrix K4({
		E_d,	O_m1,
		O_m2,	S1
	});


	SymbolicMatrix K = K1 * K2 * K3 * K4;

	K.Simplify();

	OperatorR<SymbolicMatrix> result;
	
	for (auto& rs : R.m_Rs)
	{
		result.m_Rs.push_back((K * rs).Simplify());
	}

	return result;
}

void AdmisablePerturbationDesc::AdmisableABPerturbation_Internal(const ESFDesc<RealMatrix>& desc)
{
	static_cast< ESFDesc<RealMatrix>& >(*this) = desc;

	C[0] = SymbolicMatrix(n, n, "c");
	Delta[0] = SymbolicMatrix(n, n, "\\delta");
	D[0] = SymbolicMatrix(n, n, "d");

	C[1] = SymbolicMatrix(n, n, "k");
	Delta[1] = SymbolicMatrix(n, n, "\\zeta");
	D[1] = SymbolicMatrix(n, n, "m");

	vector<string> varNamesForZero = { "c","d", "k","m" };


	if (R.m_Rs.empty())
		throw EXCEPTION;

	Phi.resize(R.m_Rs.size());
	Ypsilon.resize(R.m_Rs.size());
	for (uint32_t c = 0, size = R.m_Rs.size(); c < size; c++)
	{
		Phi[c] = (R[c] * C[0] * Delta[0] * D[0] * Q).Simplify();
		Ypsilon[c] = (R[c] * C[1] * Delta[1] * D[1] * Q).Simplify();
	}
		

	deque<Varrible> varribles;
	//Cond 1  Phi_j = 0 где j >=2 

	for (uint32_t c = 2, size = R.m_Rs.size(); c < size; c++)
	{
		GetVarriblesForPassZeroConditions(varribles, Phi[c], varNamesForZero);
		SubstituteZeros(varribles.begin(), varribles.end());	
	}



	//Cond 2  Ypsilon_j = 0 где j >=1 

	for (uint32_t c = 1, size = R.m_Rs.size(); c < size; c++)
	{
		GetVarriblesForPassZeroConditions(varribles, Ypsilon[c], varNamesForZero);
		SubstituteZeros(varribles.begin(), varribles.end());
	}
					

	//Cond 3.

	SymbolicMatrix PhiYpsilon_2 = (GetSubPhi(1, 2) + GetSubYpsilon(0, 2)).Simplify();
	GetVarriblesForPassZeroConditions(varribles, PhiYpsilon_2, varNamesForZero);
	SubstituteZeros(varribles.begin(), varribles.end());

	SymbolicMatrix PhiYpsilon_4 = (GetSubPhi(1, 4) + GetSubYpsilon(0, 4)).Simplify();
	GetVarriblesForPassZeroConditions(varribles, PhiYpsilon_4, {"m"});
	SubstituteZeros(varribles.begin(), varribles.end());

	//Cond 4.
	Cond4 = GetSubYpsilon(0, 3) + GetSubPhi(1, 3);
	Cond4.Simplify();

	//Cond 5.
	Cond5 = GetSubPhi(0, 2) + (GetSubYpsilon(0, 1) + GetSubPhi(1, 1))*( IdentityMatrix(GetSubYpsilon(0,3)) + GetSubYpsilon(0, 3) + GetSubPhi(1, 3)).Inverse() * GetSubPhi(0, 4);
	Cond5.Simplify();
}

OperatorR<SymbolicMatrix> AdmisablePerturbationDesc::CreateOperatorR_ABPerturbed(const ESFDesc<RealMatrix>& desc)
{
	OperatorR<SymbolicMatrix> result;

	for (auto& rs : R.m_Rs)
	{
		result.m_Rs.push_back(rs);
	}
	return result;
}

SymbolicMatrix AdmisablePerturbationDesc::GetSubYpsilon(uint32_t order, uint32_t index) const
{
	return PhiOrYpsilon(Ypsilon, order, index);
}

AdmisablePerturbationDesc AdmisablePerturbationDesc::AdmisableBPerturbation(const ESFDesc<RealMatrix>& desc)
{
	AdmisablePerturbationDesc result;
	result.AdmisableBPerturbation_Internal(desc);
	return result;
}

AdmisablePerturbationDesc AdmisablePerturbationDesc::AdmisableABPerturbation(const ESFDesc<RealMatrix>& desc)
{
	AdmisablePerturbationDesc result;
	result.AdmisableABPerturbation_Internal(desc);
	return result;
}

SymbolicMatrix AdmisablePerturbationDesc::GetSubPhi(uint32_t order, uint32_t index) const
{
	return PhiOrYpsilon(Phi, order, index);
}


SymbolicMatrix AdmisablePerturbationDesc::PhiOrYpsilon(const vector<SymbolicMatrix>& m, uint32_t order, uint32_t index) const
{
	switch (index)
	{
		case 1: return m[order].GetSubMatrix(0, 0, d, n - d);
		case 2: return m[order].GetSubMatrix(0, n - d, d, d);
		case 3: return m[order].GetSubMatrix(d, 0, n - d, n - d);
		case 4: return m[order].GetSubMatrix(d, n - d, n - d, d);
		default: throw EXCEPTION;
	}
}
