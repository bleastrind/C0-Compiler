#ifndef __BLEASTRIND_WRITESTATEMENT
#define __BLEASTRIND_WRITESTATEMENT
#include "abstractcode.h"
/******************************************************
<写语句>    ::= printf （ String，<表达式> ）|printf （ String ）|printf （<表达式>）
<写语句>    ::= printf （ String，<表达式>@param ）|printf （ String ）|printf （<表达式>@param）
******************************************************/
namespace ast
{
	class WriteStatement:public AbstractCode
	{
	public:
		WriteStatement():AbstractCode(tuple::Tuple::PARAM),m_hasstring(false){}
		void addNode(pExpression);
		void addToken(Token);
		void nodeComplete();
	private:
		bool m_hasstring;
		std::string m_string;
		std::string m_suffix;
	};
}//namespace
#endif
