#ifndef __BLEASTRIND_FACTOR
#define __BLEASTRIND_FACTOR
#include "astnode.h"
/******************************************************
<����>    ::= Identifier|��<���ʽ>��|<ʵ��>|<�����������>|<ConstChar >        

����ֵ����
������Ԫʽ*
******************************************************/
namespace ast
{
	class Factor:public ASTNode
	{
	public:
		enum FactorKind
		{
			Identifier,
			Expression,
			Real,
			FunctionCall,
			ConstChar
		};
		Factor():m_result(NULL){}

		void addToken(Token);
		void addNode(pExpression);
		void addNode(pReal);
		void addNode(pFunctionCallStatement);
		void addNode(pConstChar);

		pSymbol getResult() { return m_result; }
		void setKind(Factor::FactorKind k){ m_kind = k; }
		
		//���ֵ����
		lookup::ValueType getType() { return m_valuetype; }
	private:
		FactorKind m_kind;

		
		//ֵ����
		lookup::ValueType m_valuetype;

		pSymbol m_result;
	};
}//namespace
#endif
