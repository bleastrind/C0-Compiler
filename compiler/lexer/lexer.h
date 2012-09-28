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
	
	//读掉当前Token，当前Token变成下一个
	Token getNextToken();

	//预读Token，lookahead=1即当前Token
	Token peekToken(unsigned int lookahead = 1);

	//跳过token，直到当前Token为参数类型或满足函数指针的判断,失败则返回false，并且恢复指针位置
	bool goForToken(Token::TokenType);
	bool goForToken(pJudgeFunction);

	Token createToken(Token::TokenType t) { return m_tokenFactory.createToken(t); }
private:

	std::map<std::string,Token::TokenType> KeyWords;

	int m_preline; //每个token开始分析前的位置
	int m_line; //当前代码位置
	int m_peek; //当前字符
	std::istream &m_istream; //输入流
	std::deque<Token> m_tokenQueue; //缓冲区
	TokenFactory m_tokenFactory; //Token工厂

	void readch();
	
	Token::TokenType findKeyword(std::string str);
    Token readNewToken();


};



#endif
