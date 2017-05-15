#include "stdafx.h"
#include "LatexContext.h"
#include "..\Utils.h"

const std::string LatexFile::sm_MikTexPath = R"(D:\Program_Files\Full_Tex\texmf\miktex\bin\)";

LatexContext::LatexContext(size_t  line) : m_Line(line)
{

}

LatexContext::~LatexContext()
{

}


size_t LatexContext::Line()
{
	return m_Line;
}



LatexFile::LatexFile(const char* srcFile, const char* destFile) : m_SrcFile(srcFile), m_DestFile(destFile)
{
	Load();
}


LatexFile::~LatexFile()
{
}

void LatexFile::Load()
{
	ifstream f(m_SrcFile);

	while (!f.eof())
	{
		m_Lines.emplace_back();				
		getline(f, m_Lines.back());		
	}
}

std::shared_ptr<LatexContext> LatexFile::SeekToSection(const char* sectionName)
{
	for (int c = 0, size = m_Lines.size(); c < size; c++ )
	{
		string& line = m_Lines[c];
		if (line == "\\section{" + string(sectionName) + "}")
		{
			auto latexContext = make_shared<LatexContext>(c + 1);
			m_ProducedLines.emplace_back(latexContext);
			return latexContext;
		}
	}
		
	throw LatexEXCEPTION;	
}

void LatexFile::Write()
{
	//deque<string> lines(m_Lines.size() + m_ProducedLines.size());

	ofstream f(m_DestFile, std::ofstream::trunc);

	size_t startLine = 0;

	for (auto& newLines : m_ProducedLines)
	{	
		for (size_t c = startLine, size = newLines->Line(); c < size; c++)
		{			
			f << m_Lines[c] << endl;
		}
		startLine += newLines->Line();
		
		while (!newLines->eof())
		{
			string s;
			getline(  *newLines , s);
			f << s << endl;
		}		
	}
			
	for (int c = startLine, size = m_Lines.size(); c < size; c++)
	{
		f << m_Lines[c] << endl;
	}

	m_SrcFile = m_DestFile;

	Load();
}

void LatexFile::Texify(bool runViewer)
{
	AddPath(sm_MikTexPath);
	//AddPath(R"(D:\Program_Files\Full_Tex\texmf\miktex\bin\texworks-plugins)");
	/*if (_putenv_s("PATH", (string("%PATH%;") + sm_MikTexPath).c_str() ) != 0)
		throw EXCEPTION;

	if (_putenv_s("PATH", (string("%PATH%;") + R"(d:\Program_Files\Full_Tex\texmf\miktex\bin\texworks-plugins\)").c_str()) != 0)
		throw EXCEPTION;*/
	

	if (system(nullptr) == 0)
		throw EXCEPTION;

	string texifyCmd = "texify -c -q -p -b ";

	if (runViewer)	
		texifyCmd += "--run-viewer ";	
	
	texifyCmd += m_DestFile;

	if (system(texifyCmd.c_str()) != 0)		
		throw EXCEPTION;
	
}

