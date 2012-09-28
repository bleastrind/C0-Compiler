#ifndef __BLEASTRIND_ABSTRACTCONST
#define __BLEASTRIND_ABSTRACTCONST
#include "astnode.h"
#include "../lookup/symbol.h"
/******************************************************
子类：ConstChar，Real，Integer
******************************************************/
namespace ast
{
	class AbstractConst:public ASTNode
	{
	public:
		//默认并非常量声明
		AbstractConst():m_fromdeclaration(false){}
		virtual void setIdentifier(Token)=0;
	protected:
		bool m_fromdeclaration;
	};
}//namespace

#endif
