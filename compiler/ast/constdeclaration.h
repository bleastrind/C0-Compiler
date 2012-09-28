#ifndef __BLEASTRIND_CONSTDECLARATION
#define __BLEASTRIND_CONSTDECLARATION
#include "astnode.h"
#include "abstractconst.h"
#include "../lookup/symbol.h"
/**********************************************************
<��������>   ::=   int Identifier ��<����>{, Identifier��<����> }
                    | float Identifier ��<ʵ��>{, Identifier��<ʵ��>}
                    | char  Identifier ��<ConstChar>{, Identifier��<ConstChar>}

<��������>   ::=   int Identifier��m_identifier ��<����>��m_identifier{, Identifier��m_identifier ��<����>��(m_identifier}
                    | float Identifier��m_identifier ��<ʵ��>��m_identifier{, Identifier��m_identifier��<ʵ��>��m_identifier}
                    | char  Identifier��m_identifier ��<ConstChar>��m_identifier{, Identifier��m_identifier��<ConstChar>��m_identifier}
				
������������ӵ����ϱ�
����洢�ռ����*
***********************************************************/
namespace ast
{
	class ConstDeclaration:public ASTNode
	{
	public:

		
		//----------------------------
		//���趨��ǰidentifer
		//----------------------------
		void addToken(Token t);

		//----------------------------
		//ʹ�õ�ǰidentifer��AbstractConst�ڵ��ֵ
		//����һ��Symbol�嵽���ű���
		//----------------------------
		void prepareNode(pAbstractConst);
		
	private:
		void setType(Token t);
		
		//��ǰ��ʶ��
		Token m_identifier;
	};
}//namespace
#endif
