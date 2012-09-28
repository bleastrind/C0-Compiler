#include "real.h"
#include "abstractconst.h"
#include "realsuffix.h"
#include "integer.h"
#include "../error.h"
#include <assert.h>

void ast::Real::addNode(pInteger pint)
{
	//确保自动删除
	std::auto_ptr<ast::ASTNode> point_manager(pint);

	m_int = pint->getValue();
	m_float = (float) m_int;


}

void ast::Real::addNode(pRealSuffix prs)
{
		//确保自动删除
	std::auto_ptr<ast::ASTNode> point_manager(prs);

	m_valuetype = lookup::Float;

	m_float += m_int >= 0 ? prs->getValue() : - prs->getValue() ;

}	

void ast::Real::nodeComplete()
{
	if( m_fromdeclaration )
	{
		lookup::Const* p = lookup::Env::createConst( m_identifier.getString(), m_float );
		if( !getEnv()->addConst(p) )
			err::Error(err::IDENTIFIER_ALLREADY_DECLARED,m_identifier).report();
	}
}