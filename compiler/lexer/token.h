#ifndef __TOKEN
#define __TOKEN
#include <string>
#include <iostream>
/****************************************************
�޷�������  [+|-][1-9]\d*
�ַ��� "\w*"
��ʶ�� [a-zA-Z_]\w*

****************************************************/
class Lexer;


class Token
{

public:
	enum TokenType
	{
		NULLTYPE,	//���������������
		ERRORTYPE,	//�����ʷ�����

		NUM,	//���ִ�
		STRING,	//�ַ���
		CONSTCHAR,	//�ַ�����

		IDENTIFIER,	//��ʶ��
		INT,	//int �ؼ���
		FLOAT,	//float �ؼ���
		CHAR,	//char �ؼ���
		VOID,	//void �ؼ���
		CONST,	//const �ؼ���

		IF,	//if�ؼ���
		ELSE,	//else�ؼ���
		WHILE,	//while�ؼ���
		SWITCH,	//switch�ؼ���
		CASE,	//case�ؼ���
		DEFAULT,	//default�ؼ���

		LE,	//<= �ؼ���
		L,	//< �ؼ���
		GE,	//>=�ؼ���
		G,	//>�ؼ���
		NE,	//!=�ؼ���
		E,	//==�ؼ���

		ASSIGN,	//=�ؼ���
	
		ADD,	// + �ؼ���
		SUB,	// - �ؼ���
		MUL,	// * �ؼ���
		DIV,	// / �ؼ���

		OPENBRACE,	// { �ؼ���
		CLOSEBRACE,	// } �ؼ���
		OPENPAREN,	// ( �ؼ���
		CLOSEPAREN,	// ) �ؼ���

		COMMA,	// , �ؼ���
		DOT,	// . �ؼ���
		SEMICOLON,	// ; �ؼ���
		COLON,	// : �ؼ���

		MAIN,	//main �ؼ���
		SCANF,	//scanf �ؼ���
		PRINTF,	//printf �ؼ���
		RETURN	//return �ؼ���

		
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
