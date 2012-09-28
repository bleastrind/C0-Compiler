#include "readstatement.h"
#include "../lookup/symbol.h"
#include "../error.h"
#include <assert.h>
void ast::ReadStatement::addToken(Token identifier)
{
	lookup::Variable* pvar = getEnv()->findVariableOrParam(identifier.getString());
	if (pvar == NULL)
	{
		throw err::Error(err::READ_TO_NONVAR,identifier);
	}

	std::string readstr;
	switch( pvar->getType() )
	{
	case lookup::Char:
		readstr = "%c";
		break;
	case lookup::Int:
		readstr = "%d";
		break;
	case lookup::Float:
		readstr = "%f";
		break;
	default:
		assert(0);
	}
	

	m_src1 = lookup::Env::createString("",readstr);
	m_target = pvar;
	genCode();
}
