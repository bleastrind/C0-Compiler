#ifndef __BLEASTRIND_FACTOR
#define __BLEASTRIND_FACTOR
#include "astnode.h"
/******************************************************
<因子>    ::= Identifier|（<表达式>）|<实数>|<函数调用语句>|<ConstChar >        

设置值类型
生成四元式*
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
		
		//获得值类型
		lookup::ValueType getType() { return m_valuetype; }
	private:
		FactorKind m_kind;

		
		//值类型
		lookup::ValueType m_valuetype;

		pSymbol m_result;
	};
}//namespace
#endif
