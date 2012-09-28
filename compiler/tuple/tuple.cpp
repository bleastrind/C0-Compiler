#include <iostream>
#include "tuple.h"
#include "../lookup/env.h"
#include "../lookup/symbol.h"
#include "../util.h"


tuple::Tuple* tuple::Tuple::createTuple(Tuple::OpType op,lookup::Symbol *src1,lookup::Symbol *src2,lookup::Symbol *target)
{
	tuple::Tuple* ptuple = new tuple::Tuple(op,src1,src2,target);
	return ptuple;
}

std::string tuple::Tuple::toString()
{
	std::string ans;
	switch ( this->m_op )
	{
	case tuple::Tuple::ADD:
		ans = "add\t";
		break;
	case tuple::Tuple::ASSIGN:
		ans = "assign\t";
		break;
	case tuple::Tuple::CALL:
		ans = "call\t";
		break;
	case tuple::Tuple::DIV:
		ans = "div\t";
		break;
	case tuple::Tuple::ENDP:
		ans = "endp\t";
		break;
	case tuple::Tuple::FUNC:
		ans = "func\t";
		break;
	case tuple::Tuple::JE:
		ans = "je\t";
		break;
	case tuple::Tuple::JG:
		ans = "jg\t";
		break;
	case tuple::Tuple::JGE:
		ans = "jge\t";
		break;
	case tuple::Tuple::JL:
		ans = "jl\t";
		break;
	case tuple::Tuple::JLE:
		ans = "jle\t";
		break;
	case tuple::Tuple::JMP:
		ans = "jmp\t";
		break;
	case tuple::Tuple::JNE:
		ans = "jne\t";
		break;
	case tuple::Tuple::LABEL:
		ans = "label\t";
		break;
	case tuple::Tuple::MAINEND:
		ans = "mainend\t";
		break;
	case tuple::Tuple::MUL:
		ans = "mul\t";
		break;
	case tuple::Tuple::PARAM:
		ans = "param\t";
		break;
	case tuple::Tuple::PRINTF:
		ans = "printf\t";
		break;
	case tuple::Tuple::RET:
		ans = "ret\t";
		break;
	case tuple::Tuple::SCANF:
		ans = "scanf\t";
		break;
	case tuple::Tuple::SUB:
		ans = "sub\t";
		break;
	default:
		ans = "\t";
		break;

	}
	if (this->m_src1)
		ans += m_src1->toString();
	else
		ans += "null";
	ans += "\t";
	if (this->m_src2)
		ans += m_src2->toString();
	else
		ans += "null";
	ans += "\t";
	if (this->m_target)
		ans += m_target->toString();
	else
		ans += "null";
	ans += "\t";
	return ans;
}