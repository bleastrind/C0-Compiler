#ifndef __BLEASTRIND_VALUEPARAMLIST
#define __BLEASTRIND_VALUEPARAMLIST
#include "abstractcode.h"
#include <stack>
/******************************************************
<值参数表>   ::= <表达式>{，<表达式>}|EPSILON

<值参数表> ↓function  ::= <表达式>↑param@pushparam{，<表达式>↑param@pushparam}|EPSILON  @reverseparamall
  
******************************************************/
namespace ast
{
	class ValueParamList:public AbstractCode
	{
	public:
		ValueParamList():m_pfunc(NULL),m_paramindex(0){}

		void setFunction(lookup::Function* p){ m_pfunc = p; }
		void addNode(pExpression);

		void nodeComplete();
	private:
		std::stack<pSymbol> m_paramstack;
		lookup::Function* m_pfunc;

		unsigned int m_paramindex ;

	};
}//namespace
#endif
