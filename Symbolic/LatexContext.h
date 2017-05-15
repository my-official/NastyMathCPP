#pragma once
#include "..\Exceptions.h"

class LatexFile;

class LatexContext : public stringstream
{
public:
	LatexContext(size_t  line);
	virtual ~LatexContext();	
	size_t Line();
private:
	size_t m_Line;
};

class LatexFile
{
public:
	static const string sm_MikTexPath;

	LatexFile(const char* srcFile, const char* destFile);
	virtual ~LatexFile();
	void Load();

	shared_ptr<LatexContext> SeekToSection(const char* sectionName);
	void Write();
	void Texify(bool runViewer);
private:
	string m_SrcFile;
	string m_DestFile;

	deque<string> m_Lines;
	list< shared_ptr<LatexContext> > m_ProducedLines;	
};


class LatexError : public Exception
{
public:
	using Exception::Exception;
};




#define LatexEXCEPTION	LatexError(__FUNCTION__, __FILE__, __LINE__)