#include "returnstatement.h"
#include "expression.h"
#include "../lookup/symbol.h"
#include "../error.h"
void ast::ReturnStatement::addNode(pExpression p)
{
	lookup::Function* f =getEnv()->getFunction();
	if ( f->getType() == lookup::Null )
		err::Error(err::RETURN_AT_NONRETURNFUNCTION).report();
	m_src2 = p->getResult();

}

void ast::ReturnStatement::nodeComplete()
{
	m_src1 = getEnv()->getFunction();
	genCode();
}
