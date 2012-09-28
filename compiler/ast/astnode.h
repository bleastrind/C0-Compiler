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
		//��������Parserֱ��ʹ�õľ���ָ�룬
		//��ֱ�ӵ�����д�ķ���������virtual
		//-----------------------------------

		//parse֮ǰ�����ã�����׼���̳�����
		virtual void prepareNode(ASTNode *){}

		//parse֮�󱻵��ã����ڻ�ȡ�ۺ�����
		virtual void addNode(ASTNode * ast){ delete ast; }

		virtual void addToken(Token token){ ; }

		//������ɽ���һЩ����
		virtual void nodeComplete() { }

		virtual void setParent(ASTNode * pnode) { m_parent = pnode; }

		virtual lookup::Env* getEnv() { return m_parent == NULL ? NULL : m_parent->getEnv(); }

	protected:


		ASTNode * m_parent;
	};
}//namespace
#endif
