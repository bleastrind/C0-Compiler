#ifndef __BLEASTRIND_IFSTATEMENT
#define __BLEASTRIND_IFSTATEMENT
#include "abstractcode.h"
/******************************************************
<�������>  ::=  if ��<����>��<���>[else<���>]

<�������>  ::=  if ��<����>��falseLabel��<���>@gotoend@putfalseLabel[else<���>]@putendlabel
******************************************************/
namespace ast
{
	class IfStatement:public AbstractCode
	{
	public:
		IfStatement():AbstractCode(tuple::Tuple::JMP),m_first(true),m_endlabel(m_src1){}
		void addNode(pIfExpression);
		void addNode(pStatement);
		void nodeComplete();
	private:
		bool m_first;
		pSymbol & m_endlabel;
		pSymbol m_falseLabel;
	};
}//namespace
#endif
