#include "typespecifier.h"
#include <assert.h>
void ast::TypeSpecifier::addToken(Token token)
{
	switch( token.getKind())
	{
	case Token::INT:
		m_type = lookup::Int;
		break;
	case Token::FLOAT:
		m_type = lookup::Float;
		break;
	case Token::CHAR:
		m_type = lookup::Char;
		break;
	default:
		assert(0);
	}
}