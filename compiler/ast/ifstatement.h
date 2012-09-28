#ifndef __BLEASTRIND_IFSTATEMENT
#define __BLEASTRIND_IFSTATEMENT
#include "abstractcode.h"
/******************************************************
<条件语句>  ::=  if （<条件>）<语句>[else<语句>]

<条件语句>  ::=  if （<条件>↑falseLabel）<语句>@gotoend@putfalseLabel[else<语句>]@putendlabel
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
