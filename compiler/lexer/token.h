#ifndef __TOKEN
#define __TOKEN
#include <string>
#include <iostream>
/****************************************************
无符号整数  [+|-][1-9]\d*
字符串 "\w*"
标识符 [a-zA-Z_]\w*

****************************************************/
class Lexer;


class Token
{

public:
	enum TokenType
	{
		NULLTYPE,	//用于整个程序结束
		ERRORTYPE,	//发生词法错误

		NUM,	//数字串
		STRING,	//字符串
		CONSTCHAR,	//字符常量

		IDENTIFIER,	//标识符
		INT,	//int 关键字
		FLOAT,	//float 关键字
		CHAR,	//char 关键字
		VOID,	//void 关键字
		CONST,	//const 关键字

		IF,	//if关键字
		ELSE,	//else关键字
		WHILE,	//while关键字
		SWITCH,	//switch关键字
		CASE,	//case关键字
		DEFAULT,	//default关键字

		LE,	//<= 关键字
		L,	//< 关键字
		GE,	//>=关键字
		G,	//>关键字
		NE,	//!=关键字
		E,	//==关键字

		ASSIGN,	//=关键字
	
		ADD,	// + 关键字
		SUB,	// - 关键字
		MUL,	// * 关键字
		DIV,	// / 关键字

		OPENBRACE,	// { 关键字
		CLOSEBRACE,	// } 关键字
		OPENPAREN,	// ( 关键字
		CLOSEPAREN,	// ) 关键字

		COMMA,	// , 关键字
		DOT,	// . 关键字
		SEMICOLON,	// ; 关键字
		COLON,	// : 关键字

		MAIN,	//main 关键字
		SCANF,	//scanf 关键字
		PRINTF,	//printf 关键字
		RETURN	//return 关键字

		
	};

	Token(){}
	typedef Token::TokenType TokenType;
	Token(int,TokenType,std::string);
	
	
	Token::TokenType getKind() const{ return m_kind;}
	std::string getString() const { return m_string; }
	int getLine() const { return m_line; }

	friend std::ostream & operator<<(std::ostream&,const Token&);

private:
	int m_line;
	Token::TokenType m_kind;
	std::string m_string;


};



class TokenFactory 
{
public:
	Token createToken(Token::TokenType,std::string str = "");
	TokenFactory(Lexer * lexer):m_plexer(lexer){}
	TokenFactory(){}
	void setLexer(Lexer * lexer) { m_plexer = lexer; }
private:
	Lexer *m_plexer;
};

#endif
