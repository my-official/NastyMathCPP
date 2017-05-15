#pragma once




template <class Iterator>
Sum& Sum::Substitute(Iterator first, Iterator last, const Sum& value)
{
	auto it = first;

	while (it != last)
	{
		Substitute(*it, value);		

		++it;
	}
	return *this;
}
