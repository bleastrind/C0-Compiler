#ifndef __BLEASTRIND_COMPILEUNIT
#define __BLEASTRIND_COMPILEUNIT
#include "astnode.h"
/*******************************************************************
<程序>    ::= <常量说明部分><变量说明部分>{<函数定义部分>}<主函数>



*******************************************************************/
namespace ast
{
	class CompileUnit:public ASTNode
	{
	public:
		CompileUnit():m_pEnv(new lookup::Env()){ }

		virtual lookup::Env* getEnv() { return m_pEnv; }
		~CompileUnit(){ delete m_pEnv; }
	private:

		lookup::Env* m_pEnv;
	};
}//namespace
#endif
