#include "switchstatement.h"
#include "expression.h"
#include "casestatementlist.h"
#include "defaultstatement.h"
#include "../tuple/tuplemanager.h"
#include "../error.h"
#include <assert.h>

void ast::SwitchStatement::addNode(pExpression p)
{	
	//ȷ���Զ�ɾ��
	std::auto_ptr<ast::ASTNode> point_manager(p);

	m_result = p->getResult();
}

void ast::SwitchStatement::addNode(pCaseStatementList p)
{	
	//ȷ���Զ�ɾ��
	std::auto_ptr<ast::ASTNode> point_manager(p);

	m_endlabel = p->getEndLabel();
}

void ast::SwitchStatement::prepareNode(pCaseStatementList p)
{
	p->setCaseResult(m_result);
}

void ast::SwitchStatement::nodeComplete()
{
	tuple::TupleManager::putLabel( m_endlabel );
}
