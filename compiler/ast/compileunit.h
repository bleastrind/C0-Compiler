#ifndef __BLEASTRIND_COMPILEUNIT
#define __BLEASTRIND_COMPILEUNIT
#include "astnode.h"
/*******************************************************************
<����>    ::= <����˵������><����˵������>{<�������岿��>}<������>



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
