#ifndef __BLEASTRIND_VARIABLEDECLARATION
#define __BLEASTRIND_VARIABLEDECLARATION
#include "astnode.h"
#include "typespecifier.h"
#include "../lookup/symbol.h"
/******************************************************
<��������>  ::= <���ͱ�ʶ��> Identifier {, Identifier }

<��������>  ::= <���ͱ�ʶ��> ��m_type Identifier@createVariable(m_type,Identifier) {, Identifier @createVariable(m_type,Identifier)}

******************************************************/
namespace ast
{
	class VariableDeclaration:public ASTNode
	{
	public:
		VariableDeclaration():m_type(lookup::Null){}

		//lookup::Symbol::SymbolType getType() { return m_type; } 
		//std::vector<lookup::Symbol *> getSymbols();
		
		void addToken(Token);
		void addNode(pTypeSpecifier);
	private:
		void setType(Token t);
		lookup::ValueType m_type ;

	};
}//namespace
#endif
