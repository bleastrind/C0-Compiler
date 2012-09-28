#include "declarationhead.h"
#include "../error.h"

void ast::DeclarationHead::addToken(Token t)
{
	switch( t.getKind() )
	{
	case Token::VOID:
		m_type = lookup::Null;
		break;
	case Token::INT:
		m_type = lookup::Int;
		break;
	case Token::FLOAT:
		m_type = lookup::Float;
		break;
	case Token::CHAR:
		m_type = lookup::Char;
		break;
	case Token::IDENTIFIER:
		m_symbol = lookup::Env::createFunction(t.getString() , m_type);
		break;
	default:
		throw err::Error(err::UNKNOWN);
	}
}