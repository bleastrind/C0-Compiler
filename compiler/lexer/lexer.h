#ifndef __BLEASTRIND_LEXER
#define __BLEASTRIND_LEXER
#include "token.h"
#include "../util.h"
#include <iostream>
#include <vector>
#include <deque>
#include <map>
#include <memory>
class Lexer:private UnChangeable{
public:

	typedef bool (*pJudgeFunction)(Lexer & lexer,int lookahead );

	Lexer(std::istream &_istream);
	Lexer(const Lexer&); 
	int getPreLine(){ return m_preline + 1; }
	int getLine();
	
	//������ǰToken����ǰToken�����һ��
	Token getNextToken();

	//Ԥ��Token��lookahead=1����ǰToken
	Token peekToken(unsigned int lookahead = 1);

	//����token��ֱ����ǰTokenΪ�������ͻ����㺯��ָ����ж�,ʧ���򷵻�false�����һָ�ָ��λ��
	bool goForToken(Token::TokenType);
	bool goForToken(pJudgeFunction);

	Token createToken(Token::TokenType t) { return m_tokenFactory.createToken(t); }
private:

	std::map<std::string,Token::TokenType> KeyWords;

	int m_preline; //ÿ��token��ʼ����ǰ��λ��
	int m_line; //��ǰ����λ��
	int m_peek; //��ǰ�ַ�
	std::istream &m_istream; //������
	std::deque<Token> m_tokenQueue; //������
	TokenFactory m_tokenFactory; //Token����

	void readch();
	
	Token::TokenType findKeyword(std::string str);
    Token readNewToken();


};



#endif
