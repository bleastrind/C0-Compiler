#ifndef __BLEASTRIND_PARAMLIST
#define __BLEASTRIND_PARAMLIST
#include "astnode.h"
#include "typespecifier.h"
#include "../lookup/symbol.h"
/******************************************************
<参数表>    ::=  <类型标识符>Identifier{，<类型标识符>Identifier}|<空>
	
完善Function的Symbol
将参数列表中的变量加到env中

******************************************************/
namespace ast
{
	class ParamList:public ASTNode
	{
	public:
		//-------------------------------------
		//添加一个变量到当前env中
		//-------------------------------------
		void addToken(Token);

		//-------------------------------------
		//补全参数
		//-------------------------------------
		void addNode(pTypeSpecifier);


		//-------------------------------------
		//设置未完成的Function Symbol的参数表
		//-------------------------------------
		void setFunctionSymbol(lookup::Function* pFunc);
	private:

		lookup::Function* m_pFunc;
		lookup::ValueType m_type;

	};
}//namespace
#endif
