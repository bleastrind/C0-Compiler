#ifndef __BLEASTRIND_RETURNSTATEMENT
#define __BLEASTRIND_RETURNSTATEMENT
#include "abstractcode.h"
/******************************************************
<�������>   ::=  return[��<���ʽ>��]     
<�������>   ::=  return[��<���ʽ>��]@genRet     
******************************************************/
namespace ast
{
	class ReturnStatement:public AbstractCode
	{
	public:
		ReturnStatement():AbstractCode(tuple::Tuple::RET){}
		void addNode(pExpression);
		void nodeComplete();
	private:
	};
}//namespace
#endif
