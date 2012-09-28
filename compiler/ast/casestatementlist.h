#ifndef __BLEASTRIND_CASESTATEMENTLIST
#define __BLEASTRIND_CASESTATEMENTLIST
#include "astnode.h"
/******************************************************
<情况表>   ::=  <情况子语句>{<情况子语句>}

<情况表>↓result↑endlabel   ::= @genEndlabel <情况子语句>↓result↓endlabel{<情况子语句>↓result↓endlabel}

******************************************************/
namespace ast
{
	class CaseStatementList:public ASTNode
	{
	public:
		CaseStatementList();

		void prepareNode(pCaseStatement);

		pSymbol getEndLabel() { return m_endlabel; }

		void setCaseResult(pSymbol p) { m_caseresult = p; }
	private:
		pSymbol m_endlabel;
		pSymbol m_caseresult;
	};
}//namespace
#endif
