#ifndef __BLEASTRIND_VARIABLEDECLARATIONLIST
#define __BLEASTRIND_VARIABLEDECLARATIONLIST
#include "astnode.h"
/******************************************************
<����˵������> ::= {<��������> ;}
             
	Ԥ�⣺{int float char},
	FOLLOW{��Ϊ int float char ||Identifier ||  ��   ��תΪ�з���ֵ����}

******************************************************/
namespace ast
{
	class VariableDeclarationList:public ASTNode
	{
	};
}//namespace
#endif
