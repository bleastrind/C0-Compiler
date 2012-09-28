#include "token.h"
#include "lexer.h"
#include <sstream>
#include <iostream>
#include <string>
#include "../error.h"
Token TokenFactory::createToken(Token::TokenType type, std::string str)
{
	if ( str.size() > 247 )
		throw err::Error(err::VARNAME_TOO_LONG);
	return Token(m_plexer->getPreLine(),type,str);
}


Token::Token(int line,Token::TokenType k,std::string str):m_line(line),m_kind(k),m_string(str){}

std::ostream & operator<<(std::ostream& out,const Token& token)
{
	out<<"kind:"<<token.m_kind<<" string:"<<token.m_string;
    return out;
}
