#ifndef __BLEASTRIND_WRITESTATEMENT
#define __BLEASTRIND_WRITESTATEMENT
#include "abstractcode.h"
/******************************************************
<д���>    ::= printf �� String��<���ʽ> ��|printf �� String ��|printf ��<���ʽ>��
<д���>    ::= printf �� String��<���ʽ>@param ��|printf �� String ��|printf ��<���ʽ>@param��
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
