#include "casestatementlist.h"
#include "casestatement.h"
#include "../tuple/tuple.h"
#include "../lookup/symbol.h"
ast::CaseStatementList::CaseStatementList():m_endlabel(NULL),m_caseresult(NULL)
{
	m_endlabel = lookup::Env::createLabel();
}

void ast::CaseStatementList::prepareNode(pCaseStatement p)
{
	p->setCaseResult(m_caseresult);
	p->setEndlabel(m_endlabel);
}