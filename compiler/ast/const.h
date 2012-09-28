#ifndef __BLEASTRIND_CONST
#define __BLEASTRIND_CONST
#include "abstractcode.h"
/******************************************************
<常量>   ::=  <实数>|<ConstChar>

<常量>↓caseresult↑nextlabel ::=  <实数>|<ConstChar>↑value@jneNextLabel
******************************************************/
namespace ast
{
	class Const:public AbstractCode
	{
	public:
		Const():AbstractCode(tuple::Tuple::JNE){}

		void addNode(pReal);
		void addNode(pConstChar);

		void setCaseResult(pSymbol p) { m_src1 = p;}
		pSymbol getNextLabel() { return m_target; }
	};
}//namespace
#endif
