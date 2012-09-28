#ifndef __BLEASTRIND_TYPESPECIFIER
#define __BLEASTRIND_TYPESPECIFIER
#include "astnode.h"
#include "../lookup/symbol.h"
/******************************************************
<���ͱ�ʶ��> ::=  int | float | char 

******************************************************/
namespace ast
{
	class TypeSpecifier:public ASTNode
	{
	public:
		lookup::ValueType getType() { return m_type; }
		void addToken(Token t);
	private:
		lookup::ValueType m_type ;
	};
}//namespace
#endif
