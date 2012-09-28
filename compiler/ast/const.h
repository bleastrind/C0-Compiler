#ifndef __BLEASTRIND_CONST
#define __BLEASTRIND_CONST
#include "abstractcode.h"
/******************************************************
<����>   ::=  <ʵ��>|<ConstChar>

<����>��caseresult��nextlabel ::=  <ʵ��>|<ConstChar>��value@jneNextLabel
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
