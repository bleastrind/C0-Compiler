#ifndef __BLEASTRIND_CONSTCHAR
#define __BLEASTRIND_CONSTCHAR
#include "abstractconst.h"
/******************************************************
<ConstChar> ::= ConstChar

//if set m_fromdeclaration
<ConstChar> ¡ýidentifier::= ConstChar@addIdentifier
//else
<ConstChar>¡üValue
******************************************************/
namespace ast
{
	class ConstChar:public AbstractConst
	{
	public:
		lookup::Const* createConst(std::string name);

		void setIdentifier(Token t){ m_identifier = t; m_fromdeclaration = true; }

		void addToken(Token t);
		char getValue(){ return m_value; }
	private:
		char m_value;
		Token m_identifier;
	};

}//namespace
#endif