#ifndef __BLEASTRIND_FUNCTIONDECLARATION
#define __BLEASTRIND_FUNCTIONDECLARATION
#include "astnode.h"
#include "declarationhead.h"
#include "paramlist.h"
#include "compoundstatement.h"
/******************************************************
<函数定义部分>  ::=  <声明头部>（<参数表>） ｛<复合语句>｝

<函数定义部分>  ::=  <声明头部>↑m_function@addFunction@genFuncLabel（<参数表>↓m_function ） ｛<复合语句>｝
创建新的Env
创建函数定义，加入符号表
******************************************************/
namespace ast
{
	class FunctionDeclaration:public ASTNode
	{
	public:
		FunctionDeclaration():m_pEnv(new lookup::Env()){ }

		//-----------------------------------
		//创建没有完成Parmlist的Function Symbol
		//添加到父节点的Env中
		//-----------------------------------
		void addNode(pDeclarationHead);
		//----------------------------------
		//继承属性下传
		//----------------------------------
		void prepareNode(pParamList);

		
		//---------------------------------
		//TODO ret
		//---------------------------------
		void addNode(pCompoundStatement);


		virtual lookup::Env* getEnv() { return m_pEnv; }

		void setParent(ASTNode * pnode) 
		{
			m_pEnv->setParent( pnode->getEnv() );
			ASTNode::setParent(pnode);
		}

		~FunctionDeclaration(){ delete m_pEnv; }


		//----------------------------------------
		//stubs
		//----------------------------------------
		void addNode(pParamList p){ASTNode::addNode(p);}
		void prepareNode(pDeclarationHead p){ASTNode::prepareNode(p);}
		void prepareNode(pCompoundStatement p){ASTNode::prepareNode(p);}
	private:

		//本层符号表
		lookup::Env* m_pEnv;

		//函数Symbol
		lookup::Function * m_function;

	};
}//namespace
#endif
