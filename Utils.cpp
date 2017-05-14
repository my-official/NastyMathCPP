#include "stdafx.h"
#include "Utils.h"


template <>
bool FCmp(complexF a, complexF b)
{
	return FCmp(a.real(), b.real()) && FCmp(a.imag(), b.imag());
}



std::string to_latex(float value)
{
	string result = to_string(value);
	while (result.back() == '0')
	{
		result.pop_back();
	}

	if (result.back() == '.')
	{
		result.pop_back();
	}

	return result;
}



void AddPath(const string& newpath)
{
	char* path = nullptr;

	if (_dupenv_s(&path, nullptr, "PATH") != 0)
		throw EXCEPTION;

	if (!path)
		throw EXCEPTION;


	if (_putenv_s("PATH", (string(path) + ";" + newpath).c_str()) != 0)
		throw EXCEPTION;

	free(path);
}