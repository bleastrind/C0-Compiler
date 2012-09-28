#include "error.h"

Lexer * err::Error::s_pLexer;
Lexer * warning::Warning::s_pLexer;
bool err::Error::s_haveError = false;
bool warning::Warning::s_haveWarning = false;
char* err::Error::ErrorString[] = 
{
	"Bad Char!",
	"Unknown Error!",
	"String did't End!",
	"Read to non-variable!",
	"Can't return a value at a nonreturn function!",
	"Variable name to long!can't use more than 247 chars!",
	"Function params count didn't match!",
	"",
	"Syntax Error!",
	"Identifier can't be left value!",
	"Integer can't start with 0!",
	"Identifier duplicate declared!",
	"Identifier not declared!",
	"Nonreturn Function in Expression!"
};

char * warning::Warning::WarningString[] = 
{
	"Convert may loss data!"
};