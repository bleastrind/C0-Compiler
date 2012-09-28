#ifndef __BLEASTRIND_ITEM
#define __BLEASTRIND_ITEM
#include "abstractcode.h"
/******************************************************
<��>     ::= <����>{(*|/)<����>}

<��> ��result    ::= <����>��pSymbol{(*|/)<����>��pSymbol@genCode}@result
����ֵ����
������Ԫʽ
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

		//-----------For�����飬�ó����ʽ����----------------
		//�������
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
