#include "integer.h"
#include "../error.h"
#include <assert.h>
#include <cctype>
using namespace ast;



void Integer::addToken(Token t)
{
	if ( t.getKind() == Token::SUB)
		m_positive = false;
	else if (t.getKind() == Token::NUM)
	{
		if (t.getString().size() > 1 && t.getString()[0] == '0')
		{
			err::Error( err::ZEROPREFFIX , t).report();
		}
		m_value = atoi( t.getString().c_str() );

		if ( !m_positive )
			m_value = - m_value;
	}else if ( t.getKind() == Token::ADD )
		;
	else
		assert(0);

}

void Integer::nodeComplete()
{
	if( m_fromdeclaration )
	{
		lookup::Const* p = lookup::Env::createConst( m_identifier.getString(), m_value );
		if( !getEnv()->addConst(p) )
			err::Error(err::IDENTIFIER_ALLREADY_DECLARED,m_identifier).report();
	}
}
