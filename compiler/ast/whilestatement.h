#ifndef __BLEASTRIND_WHILESTATEMENT
#define __BLEASTRIND_WHILESTATEMENT
#include "abstractcode.h"
/******************************************************
<Ñ­»·Óï¾ä>   ::=  while £¨<Ìõ¼þ>£©<Óï¾ä>

<Ñ­»·Óï¾ä>   ::=  while £¨@putstartLabel<Ìõ¼þ>¡üfalseLabel£©<Óï¾ä>@gotostart@putfalseLabel
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

