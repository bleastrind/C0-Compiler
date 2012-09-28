#ifndef __BLEASTRIND_PARSER
#define __BLEASTRIND_PARSER
#include "../util.h"
#include "../astnodesfwd.h"
#include "../lexer/lexer.h"
#include <string>





class Parser
{
public:
	#ifdef DEBUG
	friend class Test;
	#endif
	Parser(Lexer);
	Parser(const Parser&);
	pCompileUnit parse();
private:
	Lexer  m_lexer;
	inline void error(std::string s="");
	inline void consume(Token::TokenType);
	inline void check(Token,Token::TokenType);

	template<typename TA,typename TB>
	void createNewNode(TA parent, TB newnode)
	{
		newnode->setParent(parent);
		parent->prepareNode(newnode);
		parse(newnode);
		parent->addNode( newnode);
	}

	void parse(pCompileUnit);
	void parse(pConstDeclarationList);
	void parse(pVariableDeclarationList);
	void parse(pFunctionDeclaration);
	void parse(pMain);

	void parse(pConstDeclaration);
	void parse(pVariableDeclaration);
	void parse(pDeclarationHead);
	void parse(pParamList);
	void parse(pCompoundStatement);

	void parse(pConstChar);
	void parse(pInteger);
	void parse(pReal);
	void parse(pTypeSpecifier);
	void parse(pStatementList);
	void parse(pRealSuffix);

	void parse(pStatement);
	void parse(pIfStatement);
	void parse(pWhileStatement);
	void parse(pAssignmentStatement);
	void parse(pFunctionCallStatement);

	void parse(pReadStatement);
	void parse(pWriteStatement);
	void parse(pReturnStatement);
	void parse(pSwitchStatement);
	void parse(pIfExpression);

	void parse(pExpression);
	void parse(pItem);
	void parse(pFactor);
	void parse(pCaseStatementList);
	void parse(pCaseStatement);

	void parse(pConst);
	void parse(pDefaultStatement);
	void parse(pValueParamList);

};
#endif
