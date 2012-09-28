#include "expression.h"
#include <assert.h>
#include "item.h"
#include "../lookup/symbol.h"
#include "../error.h"
namespace
{
	inline lookup::ValueType getWider(pItem p , lookup::ValueType m_maxtype)
	{
		return m_maxtype > p->getType() ? m_maxtype  : p->getType() ;
	}
	inline lookup::ValueType getWider(pSymbol p1 , pSymbol p2)
	{
		return p1->getType() > p2->getType() ? p1->getType() : p2->getType();
	}
}

pSymbol ast::Expression::getResult (){
	if (!m_target)
		throw err::Error(err::UNKNOWN);
	return m_target; 
}

void ast::Expression::addNode(pItem p)
{	
	//确保自动删除
	std::auto_ptr<ast::ASTNode> point_manager(p);

	maintainType(p);
	
	genTuple( p );
	
}
void ast::Expression::addToken(Token t)
{
	genTuple(t);
	//ast::ASTNode::addToken(t);
}
void ast::Expression::maintainType(pItem p)
{
	//维护表达式的类型，较宽的为准
	m_maxtype = getWider(p , m_maxtype);
}

void ast::Expression::genTuple(pItem p)
{
	switch( m_state )
	{
	case ast::Expression::FIRST:
		//若只有1个，result即它本身
		m_src1 = p->getResult();
		m_target = m_src1;

		m_state = Expression::OP;
		break;
	case ast::Expression::OP:
		assert(0);
		break;
	case ast::Expression::SECOUND:
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
			m_state = Expression::OP;
		}
		break;
	default:
		assert(0);
	}
}

void ast::Expression::genTuple(Token t)
{
	switch( m_state )
	{
	case ast::Expression::FIRST:
		m_src1 = lookup::Env::createZero();
		m_op = t.getKind() == Token::SUB ? tuple::Tuple::SUB : tuple::Tuple::ADD;
		m_state = Expression::SECOUND;
		break;
	case ast::Expression::OP:
		m_op = t.getKind() == Token::SUB ? tuple::Tuple::SUB : tuple::Tuple::ADD;
		m_state = Expression::SECOUND;
		break;
	case ast::Expression::SECOUND:
		assert(0);
		break;
	}
}
