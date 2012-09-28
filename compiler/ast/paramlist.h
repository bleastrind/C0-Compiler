#ifndef __BLEASTRIND_PARAMLIST
#define __BLEASTRIND_PARAMLIST
#include "astnode.h"
#include "typespecifier.h"
#include "../lookup/symbol.h"
/******************************************************
<������>    ::=  <���ͱ�ʶ��>Identifier{��<���ͱ�ʶ��>Identifier}|<��>
	
����Function��Symbol
�������б��еı����ӵ�env��

******************************************************/
namespace ast
{
	class ParamList:public ASTNode
	{
	public:
		//-------------------------------------
		//���һ����������ǰenv��
		//-------------------------------------
		void addToken(Token);

		//-------------------------------------
		//��ȫ����
		//-------------------------------------
		void addNode(pTypeSpecifier);


		//-------------------------------------
		//����δ��ɵ�Function Symbol�Ĳ�����
		//-------------------------------------
		void setFunctionSymbol(lookup::Function* pFunc);
	private:

		lookup::Function* m_pFunc;
		lookup::ValueType m_type;

	};
}//namespace
#endif
