#include "lexer.h"
#include <iostream>
#include <string>
#include <algorithm>
#include <cctype>
#include <sstream>
#include <assert.h>
#include "../error.h"
using namespace std;

namespace
{

	namespace ForReadNewToken
	{

		const int CHAR_DIGIT = 257;
		const int CHAR_ALPHA = 258;

		int charHash(int c)
		{

			if(isdigit(c))
				return CHAR_DIGIT;
			else if('_' == c || isalpha(c))
				return CHAR_ALPHA;
			else
				return c;
		}

		//如果关键字和标识符都不区分大小写，直接在下面LOWER
		struct LowerFunctionObject 
		{
			int operator()(int c)
			{
				return tolower((unsigned char)c);
			}
		};

		std::string ToLower(std::string & str)
		{
			std::string ans(str);
			std::transform(str.begin(),str.end(),ans.begin(),LowerFunctionObject());
			return ans;
		}

	}

}


Lexer::Lexer(std::istream &_istream):m_istream(_istream),m_line(0),m_peek(' ')
{ 
	KeyWords.insert(pair<string,Token::TokenType>("int",Token::INT));
	KeyWords.insert(pair<string,Token::TokenType>("float",Token::FLOAT));
	KeyWords.insert(pair<string,Token::TokenType>("char",Token::CHAR));
	KeyWords.insert(pair<string,Token::TokenType>("void",Token::VOID));
	KeyWords.insert(pair<string,Token::TokenType>("while",Token::WHILE));
	KeyWords.insert(pair<string,Token::TokenType>("if",Token::IF));
	KeyWords.insert(pair<string,Token::TokenType>("else",Token::ELSE));
	KeyWords.insert(pair<string,Token::TokenType>("main",Token::MAIN));
	KeyWords.insert(pair<string,Token::TokenType>("scanf",Token::SCANF));
	KeyWords.insert(pair<string,Token::TokenType>("const",Token::CONST));
	KeyWords.insert(pair<string,Token::TokenType>("printf",Token::PRINTF));
	KeyWords.insert(pair<string,Token::TokenType>("return",Token::RETURN));
	KeyWords.insert(pair<string,Token::TokenType>("switch",Token::SWITCH));
	KeyWords.insert(pair<string,Token::TokenType>("case",Token::CASE));
	KeyWords.insert(pair<string,Token::TokenType>("default",Token::DEFAULT));

	m_tokenFactory.setLexer(this);

}
Lexer::Lexer(const Lexer & lexer)
:m_istream(lexer.m_istream),
m_line(lexer.m_line),
m_peek(lexer.m_peek),
m_tokenFactory(lexer.m_tokenFactory),
KeyWords(lexer.KeyWords),
m_tokenQueue(lexer.m_tokenQueue)
{
	m_tokenFactory.setLexer(this);
}



Token Lexer::getNextToken() 
{
	if ( this->m_tokenQueue.size() != 0)
	{
		Token ans = m_tokenQueue.front();
		m_tokenQueue.pop_front();
		return ans;
	}
	return readNewToken();
}

Token Lexer::peekToken(unsigned int lookahead)
{ 

	while (m_tokenQueue.size() < lookahead)
	{
		m_tokenQueue.push_back(readNewToken());
		m_tokenQueue.push_back(readNewToken());
		m_tokenQueue.push_back(readNewToken());
		m_tokenQueue.push_back(readNewToken());
	}
	return m_tokenQueue[lookahead-1];
}

int Lexer::getLine()
{
	return peekToken().getLine();
}

bool Lexer::goForToken(Token::TokenType type)
{
	int lookahead = 1;
	while(peekToken(lookahead).getKind() != type )
	{
		lookahead ++;
		if ( peekToken(lookahead).getKind() == Token::NULLTYPE )
			return false;
	}
	
	//跳过
	while ( lookahead != 1 )
	{
		getNextToken();
		lookahead--;
	}
	return true;
}

bool Lexer::goForToken( pJudgeFunction func)
{
	int lookahead = 1;
	while (! func(*this,lookahead) )
	{
		lookahead ++;
		if ( peekToken(lookahead).getKind() == Token::NULLTYPE )
			return false;
	}

	//跳过
	while ( lookahead != 1 )
	{
		getNextToken();
		lookahead--;
	}
	return true;
}

//this function will read a char to m_peek
void Lexer::readch()
{
	if(m_istream.eof())
		m_peek = '\0';
	else
		m_peek = m_istream.get();

	if(m_peek == '\n')
		m_line++;
}
Token::TokenType Lexer::findKeyword(std::string str)
{
	if (KeyWords.end() == KeyWords.find(str) )
		return Token::IDENTIFIER;
	else
		return  KeyWords[str];
}


Token Lexer::readNewToken()
{

	m_preline = m_line;//开始前的行数

	//skip space
	while(isspace(m_peek))
		readch();

	switch(ForReadNewToken::charHash( m_peek) )
	{
		case '<':
			readch();
			if ( '=' == m_peek )
			{
				Token token = m_tokenFactory.createToken(Token::LE);
				readch();
				return token;
			}else
			{
				return m_tokenFactory.createToken(Token::L);
			}
			break;
		case '>':
			readch();
			if ( '=' == m_peek )
			{
				Token token = m_tokenFactory.createToken(Token::GE);
				readch();
				return token;
			}else
				return m_tokenFactory.createToken(Token::G);
			break;
		case '=':
			readch();
			if ( '=' == m_peek )
			{
				Token token = m_tokenFactory.createToken(Token::E);
				readch();
				return token;
			}else
				return m_tokenFactory.createToken(Token::ASSIGN);
			break;
		case '!':
			readch();
			if ( '=' == m_peek )
			{
				Token token = m_tokenFactory.createToken(Token::NE);
				readch();
				return token;
			}else
				return m_tokenFactory.createToken(Token::ERRORTYPE);
		case '+':
			readch();
			return m_tokenFactory.createToken(Token::ADD);
		case '-':
			readch();
			return m_tokenFactory.createToken(Token::SUB);
		case '*':
			readch();
			return m_tokenFactory.createToken(Token::MUL);
		case '/':
			readch();
			return m_tokenFactory.createToken(Token::DIV);
		case '(':
			readch();
			return m_tokenFactory.createToken(Token::OPENPAREN);
		case ')':
			readch();
			return m_tokenFactory.createToken(Token::CLOSEPAREN);
		case '{':
			readch();
			return m_tokenFactory.createToken(Token::OPENBRACE);
		case '}':
			readch();
			return m_tokenFactory.createToken(Token::CLOSEBRACE);
		case ',':
			readch();
			return m_tokenFactory.createToken(Token::COMMA);
		case '.':
			readch();
			return m_tokenFactory.createToken(Token::DOT);
		case ':':
			readch();
			return m_tokenFactory.createToken(Token::COLON);
		case ';':
			readch();
			return m_tokenFactory.createToken(Token::SEMICOLON);
		case '\'':
			readch();
			if( '+' == m_peek ||
				'-' == m_peek ||
				'*' == m_peek ||
				'/' == m_peek ||
				ForReadNewToken::charHash(m_peek) == ForReadNewToken::CHAR_ALPHA ||
				ForReadNewToken::charHash(m_peek) == ForReadNewToken::CHAR_DIGIT)
			{
					
				std::string c;
				c = m_peek;
				readch();
				if('\'' == m_peek)
				{
					readch();
					return m_tokenFactory.createToken(Token::CONSTCHAR,c);
				}
			}
			throw err::Error(err::BADCHAR,m_tokenFactory.createToken(Token::ERRORTYPE));
		case '"':
			{
				std::string buf;
				readch();
				while (m_peek != '"')
				{
					buf += m_peek;
					readch();

					if ( m_peek == '\0' )
						throw err::Error(err::STRING_NO_END, m_tokenFactory.createToken(Token::ERRORTYPE));
				}
				readch();
				return m_tokenFactory.createToken(Token::STRING,buf);
			}
		case ForReadNewToken::CHAR_DIGIT:
			{
				std::string buf;
				while( ForReadNewToken::CHAR_DIGIT == ForReadNewToken::charHash(m_peek) )
				{
					buf += m_peek;
					readch();
				}
				return m_tokenFactory.createToken(Token::NUM,buf);
			}
		case ForReadNewToken::CHAR_ALPHA:
			{
				std::string buf;
				
				while( ForReadNewToken::CHAR_ALPHA == ForReadNewToken::charHash(m_peek) ||
					ForReadNewToken::CHAR_DIGIT == ForReadNewToken::charHash(m_peek))
				{
					buf += m_peek;
					readch();
				}
				Token::TokenType kind = findKeyword(buf);
				if (kind != Token::IDENTIFIER)
					return m_tokenFactory.createToken(kind);
				return m_tokenFactory.createToken(Token::IDENTIFIER,ForReadNewToken::ToLower(buf));
			}
		default:
			return m_tokenFactory.createToken(Token::NULLTYPE);
	}
	

//	assert(0);
}








