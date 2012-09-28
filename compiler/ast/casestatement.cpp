#include "casestatement.h"
#include "const.h"
#include "statement.h"
#include "../tuple/tuplemanager.h"
#include "../error.h"
void ast::CaseStatement::addNode(pConst p)
{
	m_nextlabel = p->getNextLabel();
}

void ast::CaseStatement::addNode(pStatement p)
{
	//确保自动删除
	std::auto_ptr<ast::ASTNode> point_manager(p);

	//gotoEndlabel
	if(!m_src1)
		throw err::Error(err::UNKNOWN);;
	genCode();

	//putnextlabel
	tuple::TupleManager::putLabel(m_nextlabel);
}

void ast::CaseStatement::prepareNode(pConst p)
{
	p->setCaseResult( m_caseresult );
}