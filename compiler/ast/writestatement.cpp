#include "writestatement.h"
#include "expression.h"
#include "../lookup/symbol.h"
#include <assert.h>
void ast::WriteStatement::addNode(pExpression p)
{

	switch(p->getType())
	{
	case lookup::Char:
		m_suffix += "%c";
		break;
	case lookup::Int:
		m_suffix += "%d";
		break;
	case lookup::Float:
		m_suffix += "%f";
		break;
	default:
		assert(0);
	}
	m_src2 = p->getResult();
}

void ast::WriteStatement::addToken(Token t)
{
	m_hasstring = true;
	m_string = t.getString();
}

void ast::WriteStatement::nodeComplete()
{


	m_src1 = lookup::Env::createString(m_string,m_suffix);
	m_op = tuple::Tuple::PRINTF;
	genCode();
}

