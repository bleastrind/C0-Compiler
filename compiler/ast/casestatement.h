#ifndef __BLEASTRIND_CASESTATEMENT
#define __BLEASTRIND_CASESTATEMENT
#include "abstractcode.h"
/******************************************************
<情况子语句>  ::=  case<常量>:<语句>

<情况子语句> ↓caseresult↓endlabel ::=  case<常量>↓caseresult↑nextlabel:<语句>@gotoEndlabel@putnextlabel
******************************************************/
namespace ast
{
	class CaseStatement:public AbstractCode
	{
	public:
		CaseStatement():
		  AbstractCode(tuple::Tuple::JMP),m_caseresult(NULL),m_endlabel(m_src1),m_nextlabel(NULL){}
		void setEndlabel(pSymbol p) { m_endlabel=p;}
		void setCaseResult(pSymbol p) { m_caseresult=p; }

		void prepareNode(pConst);
		void addNode(pConst);
		void addNode(pStatement);

		//stub
		void prepareNode(ast::ASTNode* p){ ASTNode::prepareNode(p); }
	private:
		pSymbol m_caseresult;
		pSymbol &m_endlabel;
		pSymbol m_nextlabel;
	};
}//namespace
#endif
