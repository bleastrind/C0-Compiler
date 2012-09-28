#ifndef __BLEASTRIND_DECLARATIONHEAD
#define __BLEASTRIND_DECLARATIONHEAD
#include "astnode.h"
#include "../lookup/symbol.h"
/******************************************************
<ÉùÃ÷Í·²¿>  ::= void Identifier| int Identifier|float Identifier|char Identifier
******************************************************/
namespace ast
{
	class DeclarationHead:public ASTNode
	{
	public:
		DeclarationHead():m_type (lookup::Null){}

		void addToken(Token t);

		lookup::Function * getSymbol() { return m_symbol; }
	private:
		lookup::ValueType m_type;
		lookup::Function * m_symbol;
	};
}//namespace
#endif
