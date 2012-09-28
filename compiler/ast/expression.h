#ifndef __BLEASTRIND_EXPRESSION
#define __BLEASTRIND_EXPRESSION
#include "abstractcode.h"
/******************************************************
<表达式>    ::= [+|-]<项>{(+|-)<项>}
<表达式> ↑result   ::= [+|-]<项>↑pSymbol{(+|-)<项>↑pSymbol@genCode} @result

设置值类型
生成四元式*
******************************************************/

namespace ast
{


	class Expression:public AbstractCode
	{
	public:

		Expression()
			:AbstractCode(tuple::Tuple::ADD),
			m_maxtype(lookup::Null),
			m_state(ast::Expression::FIRST)
		{}

		lookup::ValueType getType() { return m_maxtype; }

		pSymbol getResult();

		void addToken(Token);
		void addNode(pItem);
	private:
		//-----------For语义检查，得出表达式类型----------------
		//最广类型
		lookup::ValueType m_maxtype;
		void maintainType(pItem);


		//------------genCode------------------
		enum State
		{
			FIRST,
			OP,
			SECOUND
		};
		Expression::State m_state;
		void genTuple(pItem);
		void genTuple(Token);
	};
}//namespace
#endif
