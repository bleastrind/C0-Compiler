#ifndef __BLEASTRIND_ERROR
#define __BLEASTRIND_ERROR
#include <iostream>
#include <sstream>
#include "lexer/lexer.h"
namespace err{
	
	const int BADCHAR = 0;
	const int UNKNOWN = 1;
	const int STRING_NO_END = 2;
	const int READ_TO_NONVAR = 3;
	const int RETURN_AT_NONRETURNFUNCTION = 4;
	const int VARNAME_TOO_LONG = 5;
	const int FUNCTION_WORING_PARAMS = 6;
	
	const int SYNTAX_ERROR = 8;
	const int IDENTIFIER_NO_LEFTVALUE = 9;
	const int ZEROPREFFIX = 10;
	const int IDENTIFIER_ALLREADY_DECLARED = 11;
	const int IDENTIFIER_NOT_DECLARED = 12;
	const int NONRETURNFACTION = 13;
	class Error
	{
	public:

		static bool IsError() { return s_haveError; }
		static void init(Lexer &lexer)
		{
			s_pLexer = &lexer;
		}
		
		Error(){}
		Error(std::string s):
			m_string(s),
			m_line(s_pLexer->getLine()){}
		Error(int errorid,std::string name = "")
			:m_string( ErrorString[errorid] ),
			m_line(s_pLexer->getLine())
		{
			if ( name.length() != 0)
				m_string += " at:"+name;
		}
		Error(int errorid,Token t)
			:m_string( ErrorString[errorid] ),
			m_line(t.getLine())
		{
			if ( t.getString().length() != 0)
				m_string += " at:"+ t.getString();
		}

		
		void report()
		{
			s_haveError = true;
			std::cerr<<"Line:"<<m_line
				<<" Error:"<<m_string<<std::endl;
		}

		
	private:
		std::string m_string;
		int m_line;
		
		static char* ErrorString[];
		
		static Lexer *s_pLexer;
		static bool s_haveError;
	};
}//namespace err

namespace warning
{

	const int BADCONVERT = 0;
	class Warning
	{
	public:
		static bool IsWarning() { return s_haveWarning; }
		static void init(Lexer &lexer)
		{
			s_pLexer = &lexer;
		}
		
		Warning(){}
		Warning(std::string s):m_string(s){}
		Warning(int warningid,std::string name = "")
			:m_string( WarningString[warningid] ),
			m_line(s_pLexer->getLine())
		{
			if ( name.length() != 0)
				m_string += " at:"+name;
		}
		Warning(int warningid,Token t)
			:m_string( WarningString[warningid] ),
			m_line(t.getLine())
		{
			if ( t.getString().length() != 0)
				m_string += " at:"+ t.getString();
		}

		
		void report()
		{
			s_haveWarning = true;
			std::cerr<<"Line:"<<m_line
				<<" Warning:"<<m_string<<std::endl;
		}

		
	private:
		std::string m_string;
		int m_line;
		
		static bool s_haveWarning;
		static char* WarningString[];
		
		static Lexer *s_pLexer;
	};

}//namespace warning

#endif
