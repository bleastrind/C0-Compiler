#include "factor.h"
#include "assert.h"
#include "constchar.h"
#include "expression.h"
#include "functioncallstatement.h"
#include "real.h"
#include "../error.h"
#include "../lookup/symbol.h"
void ast::Factor::addToken(Token identifier)
{
	assert(identifier.getKind() == Token::IDENTIFIER);

	//设置值类型
	lookup::Const * pconst = getEnv()->findConst(identifier.getString());
	lookup::Variable* pvar = getEnv()->findVariableOrParam(identifier.getString());
	if (pconst == NULL && pvar == NULL)
	{
		throw err::Error(err::IDENTIFIER_NOT_DECLARED,identifier);
	}
	
	if ( pconst != NULL )
	{
		m_valuetype = pconst->getType();
		m_result = pconst;
	}else
	{	
		m_valuetype = pvar->getType();
		m_result = pvar;
	}

}

void ast::Factor::addNode(pConstChar p)
{
	//确保自动删除
	std::auto_ptr<ast::ASTNode> point_manager(p);

	//设置值类型
	m_valuetype = lookup::Char;

	//设置TupleElement变量
	m_result = lookup::Env::createConst( p->getValue() );

}

void ast::Factor::addNode(pExpression p)
{
	//确保自动删除
	std::auto_ptr<ast::ASTNode> point_manager(p);

	//设置值类型
	m_valuetype = p->getType();

	//设置TupleElement变量
	m_result = p->getResult();

}

void ast::Factor::addNode(pFunctionCallStatement p)
{
	//确保自动删除
	std::auto_ptr<ast::ASTNode> point_manager(p);

	//设置值类型
	m_valuetype = p->getType();
	if ( m_valuetype == lookup::Null )
	{
		
		throw err::Error(err::NONRETURNFACTION);
	}
	m_result = p->getResult();

}

void ast::Factor::addNode(pReal p)
{
	//确保自动删除
	std::auto_ptr<ast::ASTNode> point_manager(p);

	//设置值类型
	m_valuetype = p->getType();
	
	if (p->getType() == lookup::Int)
		m_result = lookup::Env::createConst(p->getInt());
	else if (p->getType() == lookup::Float)
		m_result = lookup::Env::createConst(p->getValue() );
	else 
		assert(0);
}