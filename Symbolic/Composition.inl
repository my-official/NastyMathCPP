#pragma once



template <class Functor>
void Composition::ForEachMultipliersAndDividers(Functor functor)
{
	for (auto& sym : m_Multipliers)
	{
		functor(sym);
	}
	for (auto& sym : m_Dividers)
	{
		functor(sym);
	}
}


template <class Functor>
void Composition::ForEachMultipliersAndDividers(Functor functor) const
{
	for (auto& sym : m_Multipliers)
	{
		functor(sym);
	}
	for (auto& sym : m_Dividers)
	{
		functor(sym);
	}
}


template <class Functor>
bool Composition::ForEachMultipliersAndDividersWhileTrue(Functor functor) const
{
	for (auto& sym : m_Multipliers)
	{
		if (!functor(sym))
			return false;
	}
	for (auto& sym : m_Dividers)
	{
		if (!functor(sym))
			return false;
	}
	return true;
}

template <class Functor>
bool Composition::ForEachMultipliersAndDividersWhileFalse(Functor functor) const
{
	for (auto& sym : m_Multipliers)
	{
		if (functor(sym))
			return true;
	}
	for (auto& sym : m_Dividers)
	{
		if (functor(sym))
			return true;
	}
	return false;
}

