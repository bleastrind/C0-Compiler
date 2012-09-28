#include "item.h"
#include <assert.h>
#include "factor.h"
#include "../lookup/symbol.h"
namespace
{
	inline lookup::ValueType getWider(pFactor p , lookup::ValueType m_maxtype)
	{
		return m_maxtype > p->getType() ? m_maxtype  : p->getType() ;
	}
	inline lookup::ValueType getWider(pSymbol p1 , pSymbol p2)
	{
		return p1->getType() > p2->getType() ? p1->getType() : p2->getType();
	}
}

void ast::Item::addNode(pFactor p)
{
	//确保自动删除
	std::auto_ptr<ast::ASTNode> point_manager(p);

	//较大的为准
	maintainType(p);

	genTuple(p);

}
void ast::Item::addToken(Token op)
{
	genTuple(op);
}

void ast::Item::maintainType(pFactor p)
{
	m_maxtype = getWider(p , m_maxtype);
}

inline void ast::Item::genTuple(pFactor p)
{
	switch(m_state)
	{
	case ast::Item::FIRST:
		m_src1 = p->getResult();
		m_target = m_src1;

		m_state = ast::Item::OP;
		break;
	case ast::Item::OP:
		assert(0);
		break;
	case ast::Item::SECOUND:
		{
			m_src2 = p->getResult();

			//create NEW temp and add to env	
			lookup::Variable *ptemp = lookup::Env::createTemp( getWider(m_src1,m_src2) );
			getEnv()->addVariable(ptemp);
			m_target = ptemp;

			//genCode
			genCode();

			//continue
			m_src1 = m_target;
			m_state = ast::Item::OP;
		}
		break;
	default:
		assert(0);
	}
}

inline void ast::Item::genTuple(Token t)
{	
	switch( m_state )
	{
	case ast::Item::FIRST:
		assert(0);
		break;
	case ast::Item::OP:
		m_op = t.getKind() == Token::DIV ? tuple::Tuple::DIV : tuple::Tuple::MUL;
		
		m_state = ast::Item::SECOUND;
		break;
	case ast::Item::SECOUND:
		assert(0);
		break;
	default:
		assert(0);
	}
}