#ifndef __BLEASTRIND_ASSIGNMENTSTATEMENT
#define __BLEASTRIND_ASSIGNMENTSTATEMENT
#include "abstractcode.h"
/******************************************************
<��ֵ���>   ::=  Identifier��<���ʽ>
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
