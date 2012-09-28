#ifndef __BLEASTRIND_CONSTDECLARATION
#define __BLEASTRIND_CONSTDECLARATION
#include "astnode.h"
#include "abstractconst.h"
#include "../lookup/symbol.h"
/**********************************************************
<常量定义>   ::=   int Identifier ＝<整数>{, Identifier＝<整数> }
                    | float Identifier ＝<实数>{, Identifier＝<实数>}
                    | char  Identifier ＝<ConstChar>{, Identifier＝<ConstChar>}

<常量定义>   ::=   int Identifier↑m_identifier ＝<整数>↓m_identifier{, Identifier↑m_identifier ＝<整数>↓(m_identifier}
                    | float Identifier↑m_identifier ＝<实数>↓m_identifier{, Identifier↑m_identifier＝<实数>↓m_identifier}
                    | char  Identifier↑m_identifier ＝<ConstChar>↓m_identifier{, Identifier↑m_identifier＝<ConstChar>↓m_identifier}
				
将常量定义添加到符合表
分配存储空间语句*
***********************************************************/
namespace ast
{
	class ConstDeclaration:public ASTNode
	{
	public:

		
		//----------------------------
		//后设定当前identifer
		//----------------------------
		void addToken(Token t);

		//----------------------------
		//使用当前identifer和AbstractConst节点的值
		//创建一个Symbol插到符号表中
		//----------------------------
		void prepareNode(pAbstractConst);
		
	private:
		void setType(Token t);
		
		//当前标识符
		Token m_identifier;
	};
}//namespace
#endif
