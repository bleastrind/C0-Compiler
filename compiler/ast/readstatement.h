#ifndef __BLEASTRIND_READSTATEMENT
#define __BLEASTRIND_READSTATEMENT
#include "abstractcode.h"
#include <stack>
/******************************************************
<�����>    ::=  scanf ��Identifier{��Identifier}��
<�����>    ::=  scanf ��Identifier@scanf{��Identifier@scanf}��
******************************************************/
namespace ast
{
	class ReadStatement:public AbstractCode
	{
	public:
		ReadStatement():AbstractCode(tuple::Tuple::SCANF){}
		void addToken(Token);
	private:
	};
}//namespace
#endif
