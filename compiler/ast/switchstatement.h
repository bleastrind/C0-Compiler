#ifndef __BLEASTRIND_SWITCHSTATEMENT
#define __BLEASTRIND_SWITCHSTATEMENT
#include "astnode.h"
/******************************************************
<情况语句>  ::=  switch （<表达式>） ｛<情况表>[<缺省>]｝

<情况语句>  ::=  switch （<表达式>↑result） ｛<情况表>↓result↑endlabel[<缺省>]@putendlabel｝

******************************************************/
namespace ast
{
	class SwitchStatement:public ASTNode
	{
	public:
		SwitchStatement():m_result(NULL),m_endlabel(NULL){}
		void addNode(pExpression);
		void addNode(pCaseStatementList);


		void prepareNode(pCaseStatementList);
		void nodeComplete();

		//stub  not using ASTNode::prepareNode in case of old compilier
		void addNode(ast::ASTNode* p){ ASTNode::addNode(p); }
		void prepareNode(ast::ASTNode* p){ ASTNode::prepareNode(p); }
	private:
		pSymbol m_result;
		pSymbol m_endlabel;
	};
}//namespace
#endif
