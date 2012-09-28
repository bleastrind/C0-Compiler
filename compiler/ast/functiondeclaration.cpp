#include "functiondeclaration.h"
#include "declarationhead.h"
#include "paramlist.h"
#include "compoundstatement.h"
#include "../tuple/tuplemanager.h"
#include "../tuple/tuple.h"
#include "../error.h"
void ast::FunctionDeclaration::addNode(pDeclarationHead p)
{
	//ȷ���Զ�ɾ��
	std::auto_ptr<ast::ASTNode> point_manager(p);
	

	//�������������뻷��
	m_function = p->getSymbol();

	if ( !m_parent->getEnv()->addFunction( m_function ) )
		err::Error(err::IDENTIFIER_ALLREADY_DECLARED).report();
	//���ô˲�λ������ں���
	getEnv()->setFunction( m_function);

	//������ʼ��
	tuple::TupleManager::addFunction(getEnv() );

	
}

void ast::FunctionDeclaration::prepareNode(pParamList p)
{

	
	p->setFunctionSymbol( m_function );
		
}

void ast::FunctionDeclaration::addNode(pCompoundStatement p)
{
	//ȷ���Զ�ɾ��
	std::auto_ptr<ast::ASTNode> point_manager(p);


	//��������
	pTuple endp = tuple::Tuple::createTuple(tuple::Tuple::ENDP,NULL,NULL,NULL);
	tuple::TupleManager::addCode(endp);
}