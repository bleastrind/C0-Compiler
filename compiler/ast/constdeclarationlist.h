#ifndef __BLEASTRIND_CONSTDECLARATIONLIST
#define __BLEASTRIND_CONSTDECLARATIONLIST
#include "astnode.h"
/*******************************************************************
<常量说明部分> ::=  { const<常量定义>;}
			FOLLOW无const
*******************************************************************/
namespace ast
{
	class ConstDeclarationList:public ASTNode
	{
	};
}//namespace
#endif
