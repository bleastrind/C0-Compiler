#ifndef __BLEASTRIND_ITEM
#define __BLEASTRIND_ITEM
#include "abstractcode.h"
/******************************************************
<项>     ::= <因子>{(*|/)<因子>}

<项> ↑result    ::= <因子>↑pSymbol{(*|/)<因子>↑pSymbol@genCode}@result
设置值类型
生成四元式
******************************************************/
namespace ast
{
	class Item:public AbstractCode
	{
	public:
		Item()
			:AbstractCode(tuple::Tuple::MUL),
			m_maxtype(lookup::Null),
			m_state(ast::Item::FIRST)
		{}


		lookup::ValueType getType() { return m_maxtype; }

		pSymbol getResult() { return m_target; }
		void addToken(Token);
		void addNode(pFactor);
	private:

		//-----------For语义检查，得出表达式类型----------------
		//最广类型
		lookup::ValueType m_maxtype;
		void maintainType(pFactor);


		//------------genCode------------------
		enum State
		{
			FIRST,
			OP,
			SECOUND
		};

		Item::State m_state;
		void genTuple(pFactor);
		void genTuple(Token);
	};
}//namespace
#endif
