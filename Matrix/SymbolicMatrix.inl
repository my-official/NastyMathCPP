#pragma once



template <class Iterator>
SymbolicMatrix& SymbolicMatrix::Substitute(Iterator first, Iterator last, const Sum& value)
{
	for (auto& elem : m_Data)
	{
		elem.Substitute(first, last, value);
	}
	return *this;
}