#ifndef __BLEASTRIND_VARIABLEDECLARATIONLIST
#define __BLEASTRIND_VARIABLEDECLARATIONLIST
#include "astnode.h"
/******************************************************
<变量说明部分> ::= {<变量定义> ;}
             
	预测：{int float char},
	FOLLOW{若为 int float char ||Identifier ||  （   则转为有返回值函数}

******************************************************/
namespace ast
{
	class VariableDeclarationList:public ASTNode
	{
	};
}//namespace
#endif
