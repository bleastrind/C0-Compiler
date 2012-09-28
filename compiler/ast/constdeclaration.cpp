#include "constdeclaration.h"
#include "real.h"
#include "integer.h"
#include "../error.h"
#include <assert.h>


void ast::ConstDeclaration::addToken(Token t)
{

	if ( t.getKind() == Token::IDENTIFIER )
		m_identifier = t;
}

void ast::ConstDeclaration::prepareNode(pAbstractConst p)
{

	p->setIdentifier( m_identifier) ;

}
