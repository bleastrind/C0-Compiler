#ifndef __BLEASTRIND_CASESTATEMENTLIST
#define __BLEASTRIND_CASESTATEMENTLIST
#include "astnode.h"
/******************************************************
<�����>   ::=  <��������>{<��������>}

<�����>��result��endlabel   ::= @genEndlabel <��������>��result��endlabel{<��������>��result��endlabel}

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
