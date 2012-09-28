#ifndef __BLEASTRIND_FUNCTIONCALL
#define __BLEASTRIND_FUNCTIONCALL
#include "abstractcode.h"
/******************************************************
<函数调用语句> ::= Identifier（<值参数表>）
<函数调用语句>↑result ::= Identifier（<值参数表>↓function）@result
******************************************************/
namespace ast
{
	class FunctionCallStatement:public AbstractCode
	{
	public:
		FunctionCallStatement():AbstractCode(tuple::Tuple::CALL),m_function(NULL){}

		void prepareNode(pValueParamList);

		void addToken(Token);
		void addNode(pValueParamList);

		lookup::ValueType getType() { return m_valuetype; }
		pSymbol getResult() { return m_target; }
	private:
			//值类型
		lookup::ValueType m_valuetype;
		
		lookup::Function * m_function;


	};
}//namespace
#endif
