#include "whilestatement.h"
#include "ifexpression.h"
#include "statement.h"
#include "../tuple/tuplemanager.h"
#include "../lookup/symbol.h"

ast::WhileStatement::WhileStatement()
	:AbstractCode(tuple::Tuple::JMP),m_startLabel(m_src1)
{
	m_startLabel = lookup::Env::createLabel();

	tuple::TupleManager::putLabel( m_startLabel );
}

void ast::WhileStatement::addNode(pIfExpression p)
{
	//确保自动删除
	std::auto_ptr<ast::ASTNode> point_manager(p);

	m_falseLabel = p->getFalseLabel();


}

void ast::WhileStatement::addNode(pStatement p)
{
	//确保自动删除
	std::auto_ptr<ast::ASTNode> point_manager(p);
	
	//gotostart (m_startlabel is alias of m_src1)
	genCode();

	tuple::TupleManager::putLabel( m_falseLabel );
}
