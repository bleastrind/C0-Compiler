#ifndef __BLEASTRIND_ABSTRACTCODE
#define __BLEASTRIND_ABSTRACTCODE
#include "astnode.h"
#include "../tuple/tuple.h"
#include "../lookup/symbolfwd.h"
/******************************************************
所有可以产出四元式的类
******************************************************/
namespace ast
{
	class AbstractCode:public ASTNode
	{
	public:
		AbstractCode(tuple::Tuple::OpType op = tuple::Tuple::Null)
			:m_src1(NULL),
			m_src2(NULL),
			m_op(op),
			m_target(NULL)
		{}

		virtual void genCode();
	protected:
		//---------genCode-----------
		pSymbol m_src1;
		pSymbol m_src2;
		tuple::Tuple::OpType m_op;
		pSymbol m_target;

	};
}//namespace

#endif

