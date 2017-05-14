#pragma once


class Exception : public std::logic_error
{
public:
	Exception(const char* func, const char* file, int line) : logic_error(string(file) + "(" + to_string(line) + "): " + file)
	{

	}
	virtual ~Exception(){}

};





#define EXCEPTION	Exception(__FUNCTION__,__FILE__,__LINE__)