#ifndef __BLEASTRIND_IFEXPRESSION
#define __BLEASTRIND_IFEXPRESSION
#include "abstractcode.h"
/******************************************************
<����>  ::=  <���ʽ>[<��ϵ�����><���ʽ>] 

<����> ��falseLabel ::= @genLabel <���ʽ>��result[<��ϵ�����><���ʽ>��result]@genCode //ֻ�б��ʽ����ʽΪ0����Ϊ�٣�����Ϊ��

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
