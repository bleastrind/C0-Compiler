#ifndef __BLEASTRIND_ASSIGNMENTSTATEMENT
#define __BLEASTRIND_ASSIGNMENTSTATEMENT
#include "abstractcode.h"
/******************************************************
<赋值语句>   ::=  Identifier＝<表达式>
******************************************************/
namespace ast
{
	class AssignmentStatement:public AbstractCode
	{
	public:
		AssignmentStatement():AbstractCode(tuple::Tuple::ASSIGN){}
		void addToken(Token);
		void addNode(pExpression);
	};
}//namespace
#endif
