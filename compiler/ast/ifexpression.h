#ifndef __BLEASTRIND_IFEXPRESSION
#define __BLEASTRIND_IFEXPRESSION
#include "abstractcode.h"
/******************************************************
<条件>  ::=  <表达式>[<关系运算符><表达式>] 

<条件> ↑falseLabel ::= @genLabel <表达式>↑result[<关系运算符><表达式>↑result]@genCode //只有表达式则表达式为0条件为假，否则为真

******************************************************/
namespace ast
{
	class IfExpression:public AbstractCode
	{
	public:
		IfExpression();
		pSymbol getFalseLabel() { return m_target; }

		void addToken(Token);
		void addNode(pExpression);

		void nodeComplete();
	private:
		bool m_onlyexp;
	};
}//namespace
#endif
