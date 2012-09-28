#include "assignmentstatement.h"
#include "expression.h"
#include "../lookup/symbol.h"
#include "../error.h"
void ast::AssignmentStatement::addNode(pExpression p)
{
	//确保自动删除
	std::auto_ptr<ast::ASTNode> point_manager(p);
	
	m_src1 = p->getResult();

	genCode();

	if ( m_src1->getType() > m_target->getType() )
		warning::Warning(warning::BADCONVERT).report();
}

void ast::AssignmentStatement::addToken(Token identifier)
{
	m_target = getEnv()->findVariableOrParam( identifier.getString() );
	if ( m_target == NULL )
	{

		throw err::Error(err::IDENTIFIER_NO_LEFTVALUE);
	}

}