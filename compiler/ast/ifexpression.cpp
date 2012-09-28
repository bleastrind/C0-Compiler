#include "ifexpression.h"
#include "expression.h"
#include "../lookup/symbol.h"
#include <assert.h>
ast::IfExpression::IfExpression():AbstractCode(tuple::Tuple::JE),m_onlyexp(true)
{
	m_src2 = lookup::Env::createZero();

	m_target = lookup::Env::createLabel();
}

void ast::IfExpression::addNode(pExpression p)
{
	//确保自动删除
	std::auto_ptr<ast::ASTNode> point_manager(p);

	if ( m_onlyexp )
		m_src1 = p->getResult();
	else
		m_src2 = p->getResult();
}

void ast::IfExpression::addToken(Token t)
{
	m_onlyexp = false;
	switch(t.getKind())
	{
	case Token::E:
		m_op = tuple::Tuple::JNE;
		break;
	case Token::NE:
		m_op = tuple::Tuple::JE;
		break;
	case Token::L:
		m_op = tuple::Tuple::JGE;
		break;
	case Token::LE:
		m_op = tuple::Tuple::JG;
		break;
	case Token::G:
		m_op = tuple::Tuple::JLE;
		break;
	case Token::GE:
		m_op = tuple::Tuple::JL;
		break;
	default:
		assert(0);
	}
}

void ast::IfExpression::nodeComplete()
{
	genCode();
}