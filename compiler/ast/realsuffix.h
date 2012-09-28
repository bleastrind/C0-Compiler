#ifndef __BLEASTRIND_REALSUFFIX
#define __BLEASTRIND_REALSUFFIX
#include "astnode.h"
/******************************************************
<ÊµÊý²¹³ä>    ::= .Num
******************************************************/
namespace ast
{
	class RealSuffix:public ASTNode
	{
	public:
		RealSuffix():m_value(0){}
		float getValue() { return m_value; }
		void addToken(Token t);
	private:
		float m_value;
	};
}//namespace
#endif
