#include "variabledeclaration.h"
#include "typespecifier.h"
#include "../error.h"

void ast::VariableDeclaration::addNode(pTypeSpecifier p)
{
			//确保自动删除
	std::auto_ptr<ast::ASTNode> point_manager(p);

	m_type = p->getType();

}

void ast::VariableDeclaration::addToken(Token identifier)
{
	lookup::Env * penv = getEnv();
	if ( ! penv->addVariable
			( 
			lookup::Env::createVariable( identifier.getString(), m_type ) 
			)
		)
		err::Error( err::IDENTIFIER_ALLREADY_DECLARED,identifier).report();


}