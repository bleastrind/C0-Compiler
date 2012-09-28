#ifndef __BLEASTRIND_READSTATEMENT
#define __BLEASTRIND_READSTATEMENT
#include "abstractcode.h"
#include <stack>
/******************************************************
<¶ÁÓï¾ä>    ::=  scanf £¨Identifier{£¬Identifier}£©
<¶ÁÓï¾ä>    ::=  scanf £¨Identifier@scanf{£¬Identifier@scanf}£©
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
