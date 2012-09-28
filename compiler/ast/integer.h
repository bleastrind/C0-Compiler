#ifndef __BLEASTRIND_INTEGER
#define __BLEASTRIND_INTEGER
#include "abstractconst.h"
/******************************************************
<整数>  ::= [+|-]Num

//if set m_fromdeclaration
<整数>↓identifier ::= [+|-]↑m_positive  Num ↑m_value @ addIdentifier
//else
<整数> ↑Value
******************************************************/

namespace ast
{
	class Integer:public AbstractConst
	{
	public:

		Integer():m_positive(true),m_value(0){}

		void setIdentifier(Token t){ m_identifier = t; m_fromdeclaration = true; }

		void addToken(Token t);
		
		void nodeComplete();

		int getValue() { return m_value; }

	private:
		Token m_identifier;
		bool m_positive;
		int m_value;
	};
}//namespace
#endif
