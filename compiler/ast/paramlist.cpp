#include "paramlist.h"
#include "typespecifier.h"
#include "../error.h"
void ast::ParamList::addNode(pTypeSpecifier p)
{		
	//确保自动删除
	std::auto_ptr<ast::ASTNode> point_manager(p);
	
	m_type = p->getType();

	m_pFunc->addParam( m_type );

}

void ast::ParamList::addToken(Token identifier)
{
	lookup::Env * penv = getEnv();
	if ( ! penv->addParam
			( 
			lookup::Env::createVariable( identifier.getString(), m_type ) 
			)
		)
		err::Error( err::IDENTIFIER_ALLREADY_DECLARED,identifier).report();

}

void ast::ParamList::setFunctionSymbol(lookup::Function* pFunc)
{
	
	m_pFunc = pFunc;
}