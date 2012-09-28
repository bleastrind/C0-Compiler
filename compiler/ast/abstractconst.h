#ifndef __BLEASTRIND_ABSTRACTCONST
#define __BLEASTRIND_ABSTRACTCONST
#include "astnode.h"
#include "../lookup/symbol.h"
/******************************************************
���ࣺConstChar��Real��Integer
******************************************************/
namespace ast
{
	class AbstractConst:public ASTNode
	{
	public:
		//Ĭ�ϲ��ǳ�������
		AbstractConst():m_fromdeclaration(false){}
		virtual void setIdentifier(Token)=0;
	protected:
		bool m_fromdeclaration;
	};
}//namespace

#endif
