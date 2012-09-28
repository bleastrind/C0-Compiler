#include "ifstatement.h"
#include "ifexpression.h"
#include "statement.h"
#include "../tuple/tuplemanager.h"
#include "../lookup/symbol.h"

void ast::IfStatement::addNode(pIfExpression p)
{
	//确保自动删除
	std::auto_ptr<ast::ASTNode> point_manager(p);

	m_falseLabel = p->getFalseLabel();

}

void ast::IfStatement::addNode(pStatement p)
{
	//确保自动删除
	std::auto_ptr<ast::ASTNode> point_manager(p);

	if (m_first)
	{
		m_first = false;
		
		//gen new label as end;
		m_endlabel = lookup::Env::createLabel();

		//( m_endlabel is alias of m_src1)gotoend
		genCode();

		tuple::TupleManager::putLabel( m_falseLabel );
	}
}

void ast::IfStatement::nodeComplete()
{

	tuple::TupleManager::putLabel( m_endlabel );
}
