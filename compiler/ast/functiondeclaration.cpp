#include "functiondeclaration.h"
#include "declarationhead.h"
#include "paramlist.h"
#include "compoundstatement.h"
#include "../tuple/tuplemanager.h"
#include "../tuple/tuple.h"
#include "../error.h"
void ast::FunctionDeclaration::addNode(pDeclarationHead p)
{
	//确保自动删除
	std::auto_ptr<ast::ASTNode> point_manager(p);
	

	//将函数声明加入环境
	m_function = p->getSymbol();

	if ( !m_parent->getEnv()->addFunction( m_function ) )
		err::Error(err::IDENTIFIER_ALLREADY_DECLARED).report();
	//设置此层次环境所在函数
	getEnv()->setFunction( m_function);

	//函数初始化
	tuple::TupleManager::addFunction(getEnv() );

	
}

void ast::FunctionDeclaration::prepareNode(pParamList p)
{

	
	p->setFunctionSymbol( m_function );
		
}

void ast::FunctionDeclaration::addNode(pCompoundStatement p)
{
	//确保自动删除
	std::auto_ptr<ast::ASTNode> point_manager(p);


	//函数结束
	pTuple endp = tuple::Tuple::createTuple(tuple::Tuple::ENDP,NULL,NULL,NULL);
	tuple::TupleManager::addCode(endp);
}