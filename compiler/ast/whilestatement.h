#ifndef __BLEASTRIND_WHILESTATEMENT
#define __BLEASTRIND_WHILESTATEMENT
#include "abstractcode.h"
/******************************************************
<ѭ�����>   ::=  while ��<����>��<���>

<ѭ�����>   ::=  while ��@putstartLabel<����>��falseLabel��<���>@gotostart@putfalseLabel
******************************************************/
namespace ast
{
	class WhileStatement:public AbstractCode
	{
	public:
		WhileStatement();
		void addNode(pIfExpression);
		void addNode(pStatement);
	private:
		pSymbol m_falseLabel;
		pSymbol &m_startLabel;
	};
}//namespace
#endif

