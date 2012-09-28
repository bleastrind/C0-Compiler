#ifndef __BLEASTRIND_RETURNSTATEMENT
#define __BLEASTRIND_RETURNSTATEMENT
#include "abstractcode.h"
/******************************************************
<返回语句>   ::=  return[（<表达式>）]     
<返回语句>   ::=  return[（<表达式>）]@genRet     
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
