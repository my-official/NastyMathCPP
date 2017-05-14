#pragma once


template <class MatrixT /*= MatrixT*/>
void PrintESFDesc(const ESFDesc<MatrixT>& desc )
{
	cout << "n == " << desc.n << endl;

	cout << "Index == " << desc.Index << endl;

	cout << "d == " << desc.d << endl;

	cout << "Dr = " << desc.Dr.Print() << endl;

	cout << "Rank Dr = " << desc.Dr.Rank() << endl;

	cout << "Ranked Dr = " << desc.Dr.PrintRankedMatrix() << endl;

	cout << "Gamma = " << desc.Gamma.AsString() << endl;

	cout << "Q = " << desc.Q.AsString() << endl;

	cout << "R = " << desc.R.AsString() << endl;
}

template <class MatrixT /*= Matrix*/>
string PrintESFDescAsLatexCode(const ESFDesc<MatrixT>& desc )
{
	string result;

	result += "$n=" + to_string(desc.n) + ", r=" + to_string(desc.Index) + ", d=" + to_string(desc.d) + ".$\n";


	result += "\\begin{equation}";
	
	result += "D_" + to_string(desc.Index) + "=" + desc.Dr.AsLatexCodeRanked();

	result += "\\end{equation}\n";


	result += "$Rank D_" + to_string(desc.Index) + "=" + to_string(desc.Dr.Rank()) + "$\n";
	


	result += "\\begin{equation}";

	result += "\\Gamma=" + desc.Gamma.AsLatexCode();

	result += "\\end{equation}\n";



	result += "\\begin{equation}";

	result += "Q=" + desc.Q.AsLatexCode();

	result += "\\end{equation}\n";



	result += "\\begin{equation}";

	result += "R=" + desc.R.AsLatexCode();

	result += "\\end{equation}\n";	

	return result;
}



template <class MatrixT /*= Matrix*/>
MatrixDr<MatrixT>::MatrixDr(const MatrixT Br, const MatrixT Ar, const MatrixT Lambdar) : MatrixT(1, 3, { Br, Ar, Lambdar }),
m_BrOrigin({ 0,0 }),
m_ArOrigin({ 0,Br.m_HorizontalDimensional }),
m_LambdarOrigin({ 0,Br.m_HorizontalDimensional + Ar.m_HorizontalDimensional }),
m_Br(Br),
m_Ar(Ar),
m_Lambdar(Lambdar),
m_D(0),
m_Q(Ar.m_HorizontalDimensional, Ar.m_HorizontalDimensional)

{
	FullComputation();
}


template <class MatrixT /*= Matrix*/>
std::string MatrixDr<MatrixT>::Print(bool separated /*= true*/) const
{
	std::string result = "{\n";

	for (uint32_t y = 0; y < m_VerticalDimensional; ++y)
	{
		for (uint32_t x = 0; x < m_HorizontalDimensional; ++x)
		{
			if (separated)
			{
				if (x == m_ArOrigin.second)
				{
					result += " | ";
				}

				if (x == m_LambdarOrigin.second)
				{
					result += " | ";
				}
			}

			result += to_string((*this)[{y, x}]) + ", ";
		}

		result.pop_back();
		result.pop_back();
		result += "\n";
	}

	result += " }\n";

	return result;
}

template <class MatrixT /*= Matrix*/>
std::string MatrixDr<MatrixT>::PrintRankedMatrix(bool separated /*= true*/) const
{
	vector<uint32_t> widths(m_HorizontalDimensional);

	std::string result = "{\n";

	for (uint32_t y = 0; y < m_VerticalDimensional; ++y)
	{
		for (uint32_t x = 0; x < m_HorizontalDimensional; ++x)
		{
			if (separated)
			{
				if (x == m_ArOrigin.second)
				{
					result += " | ";
				}

				if (x == m_LambdarOrigin.second)
				{
					result += " | ";
				}
			}


			if (std::find(m_SpecialLinearlyIndependent.begin(), m_SpecialLinearlyIndependent.end(), make_pair(y, x)) != m_SpecialLinearlyIndependent.end())
			{
				auto value = to_string((*this)[{y, x}]);

				widths[x] = std::max<uint32_t>(widths[x], (uint32_t)value.length());
				result += value + ", ";
			}
			else
			{
				result += "********, ";
			}
		}

		result.pop_back();
		result.pop_back();
		result += "\n";
	}

	result += " }\n";

	return result;
}

template <class MatrixT /*= Matrix*/>
MatrixT MatrixDr<MatrixT>::GetMatrixQ() const
{
	return m_Q;
}

template <class MatrixT /*= Matrix*/>
MatrixT MatrixDr<MatrixT>::GetMatrixGamma() const
{
	return m_Gamma;
}

template <class MatrixT /*= Matrix*/>
OperatorR<MatrixT> MatrixDr<MatrixT>::GetOperatorR() const
{
	MatrixT Gamma = GetMatrixGamma();

	MatrixT R = EnOOOMatrix(Gamma) * Gamma.Transpose() * (Gamma * Gamma.Transpose()).Inverse();

	OperatorR<MatrixT> result;

	for (uint32_t x = 0, size = R.m_HorizontalDimensional; x < size; x += m_Ar.m_HorizontalDimensional)
	{
		auto Ri = R.GetSubMatrix(0, x, m_Ar.m_HorizontalDimensional, m_Ar.m_HorizontalDimensional);

		if (!Ri.IsZeroMatrix())
			result.m_Rs.push_back(Ri);
	}

	return result;
}



template <class MatrixT /*= RealMatrix*/>
OperatorR<MatrixT> OperatorR<MatrixT>::operator*(const MatrixT& rhs) const
{
	OperatorR<MatrixT> result = *this;

	for (auto& coef : result.m_Rs)
	{
		coef *= rhs;
	}

	return result;
}


template <class MatrixT /*= Matrix*/>
string MatrixDr<MatrixT>::AsLatexCode() const
{
	string result;
			
	result += "\\left(\\begin{array}{";		

	uint32_t n = m_Ar.m_HorizontalDimensional;


	result += string(n, 'c') + "|";

	result += string(n, 'c') + "|";

	result += string(m_Lambdar.m_HorizontalDimensional, 'c');

	result += "}\n";
	
	
	for (uint32_t y = 0; y < m_VerticalDimensional; ++y)
	{		
		uint32_t x = 0;
		uint32_t size_x = (y / n) * n;
		for (; x < size_x; ++x)
			result += " &";
		
		size_x = 2 * n;
		for (; x < size_x; ++x)
			result += to_latex((*this)[{y, x}]) + "&";

		size_x = m_VerticalDimensional;
		for (; x < size_x; ++x)
			result += " &";

		result.pop_back();
		result += "\\\\\n";
	}	

	result += "\\end{array}\\right)";
	
	return result;
}


template <class MatrixT /*= Matrix*/>
string MatrixDr<MatrixT>::AsLatexCodeRanked(RankMethod rankMethod /*= RankMethod::LeftTop*/) const
{	
	string result;

	result += "\\left(\\begin{array}{";

	uint32_t n = m_Ar.m_HorizontalDimensional;


	result += string(n, 'c') + "|";

	result += string(n, 'c') + "|";

	result += string(m_Lambdar.m_HorizontalDimensional, 'c');

	result += "}\n";


	for (uint32_t y = 0; y < m_VerticalDimensional; ++y)
	{
		uint32_t x = 0;
		uint32_t size_x = (y / n) * n;
		for (; x < size_x; ++x)
			result += " &";

		size_x += 2 * n;
		for (; x < size_x; ++x)
		{
			bool inResolvingMinor = std::find(m_SpecialLinearlyIndependent.begin(), m_SpecialLinearlyIndependent.end(), make_pair(y, x)) != m_SpecialLinearlyIndependent.end();
					
			if (inResolvingMinor)
				result += "\\textbf{";

			result += to_latex((*this)[{y, x}]);
			
			if (inResolvingMinor)
				result += "}";

			result += "&";
		}

		size_x = m_HorizontalDimensional;
		for (; x < size_x; ++x)
			result += " &";

		result.pop_back();
		result += "\\\\\n";
	}

	result += "\\end{array}\\right)";

	return result;
}


template <class MatrixT /*= Matrix*/>
void MatrixDr<MatrixT>::FullComputation()
{
	vector<ElementIndex> generalLinearlyIndependent;
	auto rank = MatrixT::Rank(RankMethod::RightTop, &generalLinearlyIndependent);

	vector<ElementIndex> lambdaLinearlyIndependent;
	auto rankLambdar = m_Lambdar.Rank(RankMethod::RightTop, &lambdaLinearlyIndependent);

	uint32_t n = m_Ar.m_HorizontalDimensional;

	if (rank < n + rankLambdar)
		throw EXCEPTION;

	m_D = rank - n - rankLambdar;


	vector<ElementIndex> aLinearlyIndependent;
	auto rankAr = m_Ar.Rank(RankMethod::RightBottom, &aLinearlyIndependent);

	vector<ElementIndex> bLinearlyIndependent = RankComputationForBr();



	for (auto& idx : aLinearlyIndependent)
	{
		idx.first += m_ArOrigin.first;
		idx.second += m_ArOrigin.second;
	}

	for (auto& idx : lambdaLinearlyIndependent)
	{
		idx.first += m_LambdarOrigin.first;
		idx.second += m_LambdarOrigin.second;
	}

	m_SpecialLinearlyIndependent.insert(m_SpecialLinearlyIndependent.end(), bLinearlyIndependent.begin(), bLinearlyIndependent.end());
	m_SpecialLinearlyIndependent.insert(m_SpecialLinearlyIndependent.end(), aLinearlyIndependent.begin(), aLinearlyIndependent.end());
	m_SpecialLinearlyIndependent.insert(m_SpecialLinearlyIndependent.end(), lambdaLinearlyIndependent.begin(), lambdaLinearlyIndependent.end());

	ComputeMatrixQ(bLinearlyIndependent);

	//m_Q = IdentityMatrix(m_Q);


	m_Gamma = *this;
	m_Gamma.ExcludeColumnsExcept(m_SpecialLinearlyIndependent);

}

template <class MatrixT /*= Matrix*/>
vector<ElementIndex> MatrixDr<MatrixT>::RankComputationForBr()
{
	auto Br = m_Br;

	vector<ElementIndex> bLinearlyIndependent;

	auto rankBr = Br.Rank(RankMethod::RightTop, &bLinearlyIndependent);

	bLinearlyIndependent.resize(m_D);

	//Br.FlipHorizontal();
	//Br.RankInternal_Step1(linearlyIndependent, 0, rankBr - m_D);


	//for (auto& e : bLinearlyIndependent)
	//{
	//	e.second = Br.m_HorizontalDimensional - 1 - e.second;
	//}
	return move(bLinearlyIndependent);
}

template <class MatrixT /*= Matrix*/>
void MatrixDr<MatrixT>::ComputeMatrixQ(vector<ElementIndex>& bLinearlyIndependent)
{
	if (bLinearlyIndependent.size() != m_D)//скорее assert
	{
		throw EXCEPTION;
	}

	//другой алгоритм
	uint32_t reverse_iteration_exclude = m_Br.m_HorizontalDimensional - m_D - 1;

	uint32_t reverse_iteration_include = m_Br.m_HorizontalDimensional - 1;

	uint32_t x = m_Br.m_HorizontalDimensional - 1;
	do
	{
		if (std::find_if(bLinearlyIndependent.begin(), bLinearlyIndependent.end(), [x](const ElementIndex& lhs)->bool
		{
			return lhs.second == x;
		}) != bLinearlyIndependent.end())
		{
			m_Q[{x, reverse_iteration_include}] = 1.0f;
			--reverse_iteration_include;
		}
		else
		{
			m_Q[{x, reverse_iteration_exclude }] = 1.0f;
			--reverse_iteration_exclude;
		}
	} while (x--);
}

template <class MatrixT /*= Matrix*/>
MatrixT MatrixDr<MatrixT>::EnOOOMatrix(const MatrixT& gamma) const
{
	MatrixT result(m_Ar.m_HorizontalDimensional, gamma.m_HorizontalDimensional);

	MatrixT En(m_Ar.m_HorizontalDimensional, m_Ar.m_HorizontalDimensional);
	En = IdentityMatrix(En);

	result.SetSubMatrix(0, 0, En);

	return result;
}

template <class MatrixT /*= Matrix*/>
OperatorR<MatrixT>::OperatorR(initializer_list<MatrixT> il)
{
	for (auto& m : il)
	{
		m_Rs.push_back(m);
	}
}


template <class MatrixT /*= Matrix*/>
MatrixT& OperatorR<MatrixT>::operator[](uint32_t i)
{
	return	m_Rs.at(i);
}

template <class MatrixT /*= Matrix*/>
MatrixT OperatorR<MatrixT>::operator[](uint32_t i) const
{
	return	m_Rs.at(i);
}

template <class MatrixT /*= Matrix*/>
std::string OperatorR<MatrixT>::AsString() const
{
	string result;

	for (uint32_t c = 0, size = (uint32_t)m_Rs.size(); c < size; c++)
	{
		result += "R_" + to_string(c) + " = " + m_Rs[c].AsString() + "\n";
	}

	return result;
}


template <class MatrixT /*= Matrix*/>
string OperatorR<MatrixT>::AsLatexCode() const
{
	string result;

	auto derivate = [](uint32_t order)->string
	{
		switch (order)
		{
			case 0:	return "";			
			case 1:
			{
				return "\\frac{d}{dt}";
			}
			break;
			default:
			{
				string result;				
				result += "\\frac{d^{(" + to_string(order) + ")}}{dt^{" + to_string(order) + "}}";
				return result;
			}
			break;
		};
	};

	if (!m_Rs.empty())
	{
		for (uint32_t c = 0, size = (uint32_t)m_Rs.size(); c < size; c++)
		{
			result += m_Rs[c].AsLatexCode() + derivate(c) + "+\n";
		}

		result.pop_back();
		result.pop_back();
	}
	else
	{
		result = "0";
	}

	return result;
}

template <class MatrixT /*= Matrix*/>
DAE<MatrixT> OperatorR<MatrixT>::ApplyTo(const MatrixT& A, const MatrixT& B, const MatrixT& QInv) const
{
	//A *dx + B * x = 0;

	uint32_t order = (uint32_t)m_Rs.size() - 1;

	if (order == 0)
		throw EXCEPTION;

	DAE<MatrixT> result;

	do
	{
		auto Ri = m_Rs[order];
		result[1 + order] += Ri*A * QInv;
		result[order] += Ri*B * QInv;
	} while (order--);


	return result;

}

template <class MatrixT /*= Matrix*/>
DAE<MatrixT> OperatorR<MatrixT>::ApplyTo(const DAE<MatrixT>& dae) const
{
	DAE<MatrixT> result;

	for (uint32_t order = 0, size = (uint32_t)m_Rs.size(); order < size; order++)
	{
		auto Ri = m_Rs[order];

		for (auto& coef : dae.m_Coefficients)
		{
			result[coef.first + order] += Ri*coef.second;
		}
	}


	result.EraseZeroMatricies();

	return result;
}

template <class MatrixT /*= MatrixT*/>
DAE<MatrixT>::DAE(const MatrixT& A, const MatrixT& B)
{
	m_Coefficients.insert(make_pair(1, A));
	m_Coefficients.insert(make_pair(0, B));
}


template <class MatrixT /*= MatrixT*/>
uint32_t DAE<MatrixT>::GetOrder() const
{
	if (!m_Coefficients.empty())
		return (--m_Coefficients.end())->first;
	else
		return 0;

}

template <class MatrixT /*= MatrixT*/>
uint32_t DAE<MatrixT>::GetMatrixDimensional() const
{
	if (!m_Coefficients.empty())
	{
		return m_Coefficients.begin()->second.m_HorizontalDimensional;
	}
	else
	{
		return 0;
	}
}

template <class MatrixT /*= MatrixT*/>
MatrixT& DAE<MatrixT>::operator[](uint32_t order)
{
	return m_Coefficients[order];
}

template <class MatrixT /*= MatrixT*/>
MatrixT DAE<MatrixT>::operator[](uint32_t order) const
{
	auto it = m_Coefficients.find(order);
	if (it != m_Coefficients.end())
	{
		return it->second;
	}
	else
	{
		return ZeroMatrix(MatrixT(GetMatrixDimensional(), GetMatrixDimensional()));
	}
}

template <class MatrixT /*= MatrixT*/>
std::string DAE<MatrixT>::AsString() const
{
	string result;
	for (auto it = m_Coefficients.rbegin(); it != m_Coefficients.rend(); ++it)
	{
		result += it->second.AsString() + " * " + "d" + to_string(it->first) + "x +";
	}

	if (!result.empty())
	{
		result.replace(result.length() - 1, 1, "= O");
	}

	return result;
}

template <class MatrixT /*= MatrixT*/>
std::string DAE<MatrixT>::AsLatexCode() const
{
	if (m_Coefficients.empty())
		return "";

	auto derivate = [](auto order)->string
	{
		string result = "x";
		for (decltype(order) c = 0; c < order; c++)
			result += 'Т';
		result += "(t)";
		return result;

	};

	string result;

	result = "\\begin{equation}";

	for (auto it = m_Coefficients.rbegin(); it != m_Coefficients.rend(); ++it)
	{		
		auto order = it->first;
		result += it->second.AsLatexCode() + " "+ derivate(it->first) + "+";
	}

	//result.pop_back();
	result.replace(result.length() - 1, 1, "=0");
	result += "\\end{equation}";
	

	return result;
}

template <class MatrixT /*= MatrixT*/>
void DAE<MatrixT>::EraseZeroMatricies()
{
	for (auto it = m_Coefficients.begin(); it != m_Coefficients.end();)
	{
		if (it->second.IsZeroMatrix())
		{
			it = m_Coefficients.erase(it);
		}
		else
		{
			++it;
		}
	}
}

template <class MatrixT /*= MatrixT*/>
ESFDesc<MatrixT> DAE<MatrixT>::ConvertToESF(const uint32_t MAX_INDEX /*= 5*/, uint32_t idlingCounter /*= 0*/)
{
	if (m_Coefficients.count(0) == 0)
	{
		throw EXCEPTION;
	}
	MatrixT B = m_Coefficients[0];

	MatrixT A = m_Coefficients.count(1) ? m_Coefficients[1] : ZeroMatrix(B);



	using namespace std::rel_ops;

	uint32_t n = A.m_HorizontalDimensional;


	MatrixT O = ZeroMatrix(A);

	MatrixT Br = B;

	Br.GrowBottom(O);

	MatrixT Ar(A);
	Ar.GrowBottom(B);

	MatrixT Lambdar(2, 1, {
		O,
		A,
	});

	uint32_t rankLambdar = Lambdar.Rank();

	uint32_t index = 1;
	for (; index < MAX_INDEX; ++index)
	{
		MatrixT LambdarNext = Lambdar;
		GrowLambdarMatrix(LambdarNext, A, B);

		uint32_t rankLambdarNext = LambdarNext.Rank();

		/*	cout << "trying index " << index << endl;
		cout << "Rank Lambdar = " << rankLambdar << endl;
		cout << "Rank rankLambdarNext = " << rankLambdarNext << endl;*/


		if (rankLambdarNext - rankLambdar == n)
		{
			if (idlingCounter == 0)
				break;
			--idlingCounter;
		}

		Br.GrowBottom(O);
		Ar.GrowBottom(O);

		Lambdar = LambdarNext;
		rankLambdar = Lambdar.Rank();

	}

	if (index == MAX_INDEX)
	{
		throw EXCEPTION;
	}

	/////
	//ESFDesc result2;
	//result2.Index = index;
	//
	//return result2;

	/////

	MatrixDr<MatrixT> Dr(Br, Ar, Lambdar);
	MatrixT Q = Dr.GetMatrixQ();

	MatrixT Gamma = Dr.GetMatrixGamma();

//	cout << Gamma.AsString() << endl;

	if (Gamma.Det() == 0)
	{
		ESFDesc<MatrixT> result;
		result.d = Dr.m_D;
		result.n = A.m_HorizontalDimensional;
		result.Index = index;
		result.Dr = Dr;
		result.Gamma = Gamma;		
		result.Lambdar = Lambdar;
		result.Q = Q;		
		return result;
		//throw logic_error("Gamma.Det() == 0");
	}


	OperatorR<MatrixT> R = Dr.GetOperatorR();

	DAE<MatrixT> esf = R.ApplyTo(A, B, Q);

	esf.EraseZeroMatricies();





	ESFDesc<MatrixT> result;
	result.d = Dr.m_D;
	result.n = A.m_HorizontalDimensional;
	result.Index = index;
	result.Dr = Dr;
	result.Gamma = Gamma;
	result.R = R;
	result.Lambdar = Lambdar;
	result.Q = Q;
	*this = esf;

	return result;
}


template <class MatrixT /*= MatrixT*/>
void DAE<MatrixT>::GrowLambdarMatrix(MatrixT& Lambdar, const MatrixT& A, const MatrixT& B)
{
	MatrixT O = ZeroMatrix(A);
	MatrixT T;

	if (Lambdar.m_HorizontalDimensional > A.m_HorizontalDimensional)
	{
		for (uint32_t c = 0, size = Lambdar.m_HorizontalDimensional - A.m_HorizontalDimensional; c < size; c += O.m_HorizontalDimensional)
		{
			T.GrowRight(O);
		}
	}

	T.GrowRight(B);
	T.GrowRight(A);

	O = ZeroMatrix(Lambdar.GetSubMatrix(0, 0, Lambdar.m_VerticalDimensional, A.m_HorizontalDimensional));

	Lambdar.GrowRight(O);

	Lambdar.GrowBottom(T);
}
