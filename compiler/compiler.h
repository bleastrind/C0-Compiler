#ifndef __BLEASTRIND_COMPILER
#define __BLEASTRIND_COMPILER
#include <iostream>
#include "lexer/lexer.h"
#include "parser/parser.h"
#include "tuple/tuplemanager.h"
#include "x86/basehandler.h"
#include "optimization/graphhandler.h"
#include "error.h"
class Compiler
{
public:

	void compile(char*,int,bool);


};
#endif
