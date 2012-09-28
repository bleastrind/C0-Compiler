#ifndef __BLEASTRIND_FUNCTIONCALL
#define __BLEASTRIND_FUNCTIONCALL
#include "abstractcode.h"
/******************************************************
<�����������> ::= Identifier��<ֵ������>��
<�����������>��result ::= Identifier��<ֵ������>��function��@result
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
			//ֵ����
		lookup::ValueType m_valuetype;
		
		lookup::Function * m_function;


	};
}//namespace
#endif
