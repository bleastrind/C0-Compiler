#ifndef __BLEASTRIND_EXPRESSION
#define __BLEASTRIND_EXPRESSION
#include "abstractcode.h"
/******************************************************
<���ʽ>    ::= [+|-]<��>{(+|-)<��>}
<���ʽ> ��result   ::= [+|-]<��>��pSymbol{(+|-)<��>��pSymbol@genCode} @result

����ֵ����
������Ԫʽ*
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
		//-----------For�����飬�ó����ʽ����----------------
		//�������
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
