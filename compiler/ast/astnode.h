#ifndef __BLEASTRIND_ASTNODE
#define __BLEASTRIND_ASTNODE
#include <vector>
#include <memory>
#include "../astnodesfwd.h"
#include "../lexer/token.h"
#include "../lookup/env.h"
#include "../util.h"
namespace ast
{
	class ASTNode:public UnChangeable
	{
	public:
		friend class Test;
		ASTNode():m_parent(NULL){}
		virtual ~ASTNode();

		//---------------------------------
		//以下由于Parser直接使用的具体指针，
		//会直接调用重写的方法，无需virtual
		//-----------------------------------

		//parse之前被调用，用于准备继承属性
		virtual void prepareNode(ASTNode *){}

		//parse之后被调用，用于获取综合属性
		virtual void addNode(ASTNode * ast){ delete ast; }

		virtual void addToken(Token token){ ; }

		//结束后可进行一些操作
		virtual void nodeComplete() { }

		virtual void setParent(ASTNode * pnode) { m_parent = pnode; }

		virtual lookup::Env* getEnv() { return m_parent == NULL ? NULL : m_parent->getEnv(); }

	protected:


		ASTNode * m_parent;
	};
}//namespace
#endif
