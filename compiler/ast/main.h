#ifndef __BLEASTRIND_MAIN
#define __BLEASTRIND_MAIN
#include "astnode.h"
/******************************************************
<主函数>  ::= void main（）｛<复合语句>｝ 
******************************************************/
namespace ast
{
	class Main:public ASTNode
	{
	public:
		Main();
		virtual lookup::Env* getEnv() { return m_pEnv; }
		
		void addNode(pCompoundStatement);

		void setParent(ASTNode * pnode) 
		{
			m_pEnv->setParent( pnode->getEnv() );
			ASTNode::setParent(pnode);
		}

		~Main(){ delete m_pEnv; }
	private:
		//本层符号表
		lookup::Env* m_pEnv;
	};
}//namespace
#endif
