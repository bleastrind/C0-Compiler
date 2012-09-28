#ifndef __BLEASTRIND_FUNCTIONDECLARATION
#define __BLEASTRIND_FUNCTIONDECLARATION
#include "astnode.h"
#include "declarationhead.h"
#include "paramlist.h"
#include "compoundstatement.h"
/******************************************************
<�������岿��>  ::=  <����ͷ��>��<������>�� ��<�������>��

<�������岿��>  ::=  <����ͷ��>��m_function@addFunction@genFuncLabel��<������>��m_function �� ��<�������>��
�����µ�Env
�����������壬������ű�
******************************************************/
namespace ast
{
	class FunctionDeclaration:public ASTNode
	{
	public:
		FunctionDeclaration():m_pEnv(new lookup::Env()){ }

		//-----------------------------------
		//����û�����Parmlist��Function Symbol
		//��ӵ����ڵ��Env��
		//-----------------------------------
		void addNode(pDeclarationHead);
		//----------------------------------
		//�̳������´�
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

		//������ű�
		lookup::Env* m_pEnv;

		//����Symbol
		lookup::Function * m_function;

	};
}//namespace
#endif
