#include "constchar.h"
#include "../error.h"

void ast::ConstChar::addToken(Token t)
{
	m_value = t.getString()[0];

	if( m_fromdeclaration )
	{
		lookup::Const* p = lookup::Env::createConst( m_identifier.getString(), m_value );
		if( !getEnv()->addConst(p) )
			err::Error(err::IDENTIFIER_ALLREADY_DECLARED,m_identifier).report();
	}
}
