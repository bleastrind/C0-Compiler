#ifndef __BLEASTRIND_SWITCHSTATEMENT
#define __BLEASTRIND_SWITCHSTATEMENT
#include "astnode.h"
/******************************************************
<������>  ::=  switch ��<���ʽ>�� ��<�����>[<ȱʡ>]��

<������>  ::=  switch ��<���ʽ>��result�� ��<�����>��result��endlabel[<ȱʡ>]@putendlabel��

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
