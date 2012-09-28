#include "parser.h"
#include "../astnodes.h"
#include "../error.h"
#include <cassert>
namespace
{


	//const
	inline bool IsConstDeclaration(Lexer &lexer)
	{
		return lexer.peekToken().getKind() == Token::CONST;
	}

	//int float char
	inline bool IsPrime(Lexer & lexer)
	{
		return (   lexer.peekToken().getKind() == Token::INT
				||	lexer.peekToken().getKind() == Token::FLOAT
				||	lexer.peekToken().getKind() == Token::CHAR
				);
	}
	//int float char || identifier || ; ,
	inline bool IsVariableDeclaration(Lexer &lexer)
	{
		return  IsPrime(lexer)
				&& 
				lexer.peekToken(2).getKind() == Token::IDENTIFIER
				&& 
				(	lexer.peekToken(3).getKind() == Token::COMMA
				||  lexer.peekToken(3).getKind() == Token::SEMICOLON
				);
	}

	//void int float char || Identifier || （
	inline bool IsFunctionDeclaration(Lexer & lexer)
	{
		return (   lexer.peekToken().getKind() == Token::INT
				|| lexer.peekToken().getKind() == Token::FLOAT
				|| lexer.peekToken().getKind() == Token::CHAR
				|| lexer.peekToken().getKind() == Token::VOID
				)
				&& 
				lexer.peekToken(2).getKind() == Token::IDENTIFIER
				&& 
				lexer.peekToken(3).getKind() == Token::OPENPAREN
				;
	}
	//void || main
	inline bool IsMain(Lexer & lexer)
	{
		return lexer.peekToken(1).getKind() == Token::VOID 
			&& lexer.peekToken(2).getKind() == Token::MAIN
			;

	}

	inline bool IsStatement(Lexer & lexer)
	{
		return lexer.peekToken().getKind() == Token::IF
			|| lexer.peekToken().getKind() == Token::WHILE
			|| lexer.peekToken().getKind() == Token::OPENBRACE
			|| lexer.peekToken().getKind() == Token::IDENTIFIER
			|| lexer.peekToken().getKind() == Token::SCANF
			|| lexer.peekToken().getKind() == Token::PRINTF
			|| lexer.peekToken().getKind() == Token::SWITCH
			|| lexer.peekToken().getKind() == Token::RETURN
			|| lexer.peekToken().getKind() == Token::SEMICOLON
			;
	}

	//identifier || (
	inline bool IsFunctionCall(Lexer & lexer)
	{
		return lexer.peekToken(1).getKind() == Token::IDENTIFIER
			&& lexer.peekToken(2).getKind() == Token::OPENPAREN
			;
	}

	//identifier || =
	inline bool IsAssignMent(Lexer & lexer)
	{
		return lexer.peekToken(1).getKind() == Token::IDENTIFIER
			&& lexer.peekToken(2).getKind() == Token::ASSIGN
			;
	}


	//+ - Num
	inline bool IsReal(Lexer & lexer)
	{
		return lexer.peekToken().getKind() == Token::ADD 
			|| lexer.peekToken().getKind() == Token::SUB
			|| lexer.peekToken().getKind() == Token::NUM;
	}

	//* /
	inline bool IsMulOp(Lexer & lexer)
	{
		return lexer.peekToken().getKind() == Token::MUL
			|| lexer.peekToken().getKind() == Token::DIV;
	}

	//+ -
	inline bool IsAddOp(Lexer & lexer)
	{
		return lexer.peekToken().getKind() == Token::ADD 
			|| lexer.peekToken().getKind() == Token::SUB;
	}

	//< <= > >= == !=
	inline bool IsRelationOp(Lexer & lexer)
	{
		return lexer.peekToken().getKind() == Token::L
			|| lexer.peekToken().getKind() == Token::LE
			|| lexer.peekToken().getKind() == Token::G
			|| lexer.peekToken().getKind() == Token::GE
			|| lexer.peekToken().getKind() == Token::E
			|| lexer.peekToken().getKind() == Token::NE;
	}
	

	namespace ForGoFor
	{

		bool IsFunctionDeclarationList(Lexer & lexer, int lookahead)
		{
			return (   lexer.peekToken(lookahead).getKind() == Token::INT
					|| lexer.peekToken(lookahead).getKind() == Token::FLOAT
					|| lexer.peekToken(lookahead).getKind() == Token::CHAR
					|| lexer.peekToken(lookahead).getKind() == Token::VOID
					)
					&& 
					(
						lexer.peekToken(lookahead+1).getKind() == Token::IDENTIFIER
						||	lexer.peekToken(lookahead+1).getKind() == Token::MAIN
					)
					&& 
					lexer.peekToken(lookahead+2).getKind() == Token::OPENPAREN
					;
		}

		bool IsVariableDeclarationList(Lexer & lexer, int lookahead)
		{
			return (   lexer.peekToken(lookahead).getKind() == Token::INT
					|| lexer.peekToken(lookahead).getKind() == Token::FLOAT
					|| lexer.peekToken(lookahead).getKind() == Token::CHAR
					)
					&& lexer.peekToken(lookahead+1).getKind() == Token::IDENTIFIER
					&& 
					(
					lexer.peekToken(lookahead + 2).getKind() == Token::SEMICOLON
					|| lexer.peekToken(lookahead + 2).getKind() == Token::COMMA
					);
		}

		bool IsDeclarationHead(Lexer & lexer,int lookahead)
		{
			return (   lexer.peekToken(lookahead).getKind() == Token::INT
					|| lexer.peekToken(lookahead).getKind() == Token::FLOAT
					|| lexer.peekToken(lookahead).getKind() == Token::CHAR
					)
					&& lexer.peekToken(lookahead+1).getKind() == Token::IDENTIFIER;
		}

		bool IsMainFunction(Lexer & lexer,int lookahead)
		{
			return lexer.peekToken(lookahead).getKind() == Token::VOID 
				&& lexer.peekToken(lookahead+1).getKind() == Token::MAIN;
		}

		bool IsStatementEnd(Lexer & lexer,int lookahead)
		{
			return lexer.peekToken(lookahead).getKind() == Token::CLOSEBRACE
				|| lexer.peekToken(lookahead).getKind() == Token::SEMICOLON;
		}
	}//namespace GoFor
}//namespace


Parser::Parser(Lexer lexer):m_lexer(lexer)
{

	err::Error::init(m_lexer);
	warning::Warning::init(m_lexer);

}

Parser::Parser(const Parser& parser):m_lexer(parser.m_lexer)
{
	err::Error::init(m_lexer);
	warning::Warning::init(m_lexer);

}
inline void Parser::error(std::string s)
{
	if (s.size() == 0)
		throw err::Error(err::SYNTAX_ERROR,m_lexer.peekToken());
	else
		throw err::Error(s);
}
pCompileUnit Parser::parse()
{

	pCompileUnit p = new ast::CompileUnit();
	std::auto_ptr<ast::ASTNode> pointmanager(p);
	parse(p);
	return p;
}


void Parser::consume(Token::TokenType except)
{
	if(m_lexer.getNextToken().getKind() != except)
	{
		error();
	}
}
void Parser::check(Token t,Token::TokenType except)
{
	if(t.getKind() != except)
	{
		std::string sym;
		switch(except)
		{
		case Token::ADD:
			sym = "+";
			break;
		case Token::ASSIGN:
			sym = "=";
			break;
		case Token::CASE:
			sym = "case";
			break;
		case Token::CHAR:
			sym = "char";
			break;
		case Token::CLOSEBRACE:
			sym = "}";
			break;
		case Token::CLOSEPAREN:
			sym = ")";
			break;
		case Token::COLON:
			sym = ":";
			break;
		case Token::COMMA:
			sym = ",";
			break;
		case Token::CONST:
			sym = "const";
			break;
		case Token::CONSTCHAR:
			sym = "'";
			break;
		case Token::DEFAULT:
			sym = "default";
			break;
		case Token::DIV:
			sym = "/";
			break;
		case Token::DOT:
			sym = ".";
			break;
		case Token::E:
			sym = "=";
			break;
		case Token::ELSE:
			sym = "else";
			break;
		case Token::ERRORTYPE:
			assert(0);
			break;
		case Token::FLOAT:
			sym = "float";
			break;
		case Token::G:
			sym = ">";
			break;
		case Token::GE:
			sym = ">=";
			break;
		case Token::IDENTIFIER:
			sym = "indentifier";
			break;
		case Token::IF:
			sym = "if";
			break;
		case Token::INT:
			sym = "int";
			break;
		case Token::L:
			sym = "<";
			break;
		case Token::LE:
			sym = "<=";
			break;
		case Token::MAIN:
			sym = "main";
			break;
		case Token::MUL:
			sym = "*";
			break;
		case Token::NE:
			sym = "!=";
			break;
		case Token::NULLTYPE:
			assert(0);
			break;
		case Token::NUM:
			sym = "[0-9]";
			break;
		case Token::OPENBRACE:
			sym = "{";
			break;
		case Token::OPENPAREN:
			sym = "(";
			break;
		case Token::PRINTF:
			sym = "printf";
			break;
		case Token::SCANF:
			sym = "scanf";
			break;
		case Token::SEMICOLON:
			sym = ";";
			break;
		case Token::STRING:
			sym = "\"";
			break;
		case Token::SUB:
			sym = "-";
			break;
		case Token::SWITCH:
			sym = "switch";
			break;
		case Token::VOID:
			sym = "void";
			break;
		case Token::WHILE:
			sym = "while";
			break;
		default:
			assert(0);
		}
		error("Missing "+sym );
	}
}

//-----------------------------------------------------------------------
//<程序>    ::= <常量说明部分><变量说明部分>{<函数定义部分>}<主函数>
//				  预测:{常量说明部分:const}
//				  {变量说明部分:int float char || identifier || ;}
//				  {函数定义部分:void int float char || Identifier || （}
//				  {主函数:void || main}
//-----------------------------------------------------------------------
void Parser::parse(pCompileUnit p)
{

	try
	{
		//<常量说明部分>
		createNewNode(p,new ast::ConstDeclarationList());
	}catch(err::Error e)
	{
		e.report();
		m_lexer.goForToken( ForGoFor::IsVariableDeclarationList );

	}


	
	try
	{
		//<变量说明部分>
		createNewNode(p,new ast::VariableDeclarationList());
	}catch(err::Error e)
	{
		e.report();
		m_lexer.goForToken( ForGoFor::IsFunctionDeclarationList );
		
	}

	//{<函数定义部分>}

	while ( IsFunctionDeclaration(m_lexer) )
	{
		try
		{
			createNewNode(p,new ast::FunctionDeclaration());

		}catch(err::Error e)
		{
			e.report();
			m_lexer.goForToken( ForGoFor::IsMainFunction );

		}
	}

	if( !IsMain(m_lexer) )
	{
		err::Error(err::UNKNOWN).report();
		m_lexer.goForToken( ForGoFor::IsMainFunction );
	}

	try
	{
		createNewNode(p,new ast::Main());
	}catch(err::Error e)
	{
		e.report();
		return;
	}
	if(m_lexer.peekToken().getKind() != Token::NULLTYPE)
		error();


	p->nodeComplete();
}


//-----------------------------------------------------------------------
//<常量说明部分> ::=  { const<常量定义>;}
//			FOLLOW无const
//-----------------------------------------------------------------------
void Parser::parse(pConstDeclarationList p)
{
	//{ const<常量定义>;}
	while( m_lexer.peekToken().getKind() == Token::CONST)
	{
		consume(Token::CONST);

		try
		{
			createNewNode(p,new ast::ConstDeclaration());
		}catch(err::Error e)
		{
			e.report();
			m_lexer.goForToken(Token::SEMICOLON);
		}
		consume(Token::SEMICOLON);
	}

	p->nodeComplete();
}



//-----------------------------------------------------------------------
//<变量说明部分> ::= {<变量定义> ;}
//		FOLLOW{若为 int float char ||Identifier ||  （   则转为有返回值函数}
//-----------------------------------------------------------------------
void Parser::parse(pVariableDeclarationList p)
{
	//{<变量定义> ;}
	while( IsVariableDeclaration(m_lexer) )
	{
		try
		{
			createNewNode(p,new ast::VariableDeclaration());
		}catch(err::Error e)
		{
			e.report();
			m_lexer.goForToken(Token::SEMICOLON);
		}
		consume(Token::SEMICOLON);
	}

	p->nodeComplete();
}



//-----------------------------------------------------------------------
//<函数定义部分>  ::=  <声明头部>（<参数表>） ｛<复合语句>｝
//-----------------------------------------------------------------------
void Parser::parse(pFunctionDeclaration p)
{
	//<声明头部>
	createNewNode(p,new ast::DeclarationHead());

	//（<参数表>）
	try
	{
		consume(Token::OPENPAREN );
		
		createNewNode(p,new ast::ParamList());
	}catch(err::Error e)
	{
		e.report();
		m_lexer.goForToken( Token::CLOSEPAREN );

	}
	consume( Token::CLOSEPAREN );
	
	//｛<复合语句>｝
	try
	{
		consume( Token::OPENBRACE);

		createNewNode(p,new ast::CompoundStatement());
	}catch(err::Error e)
	{
		e.report();
		m_lexer.goForToken( Token::CLOSEBRACE );
	}

	consume(Token::CLOSEBRACE );

	p->nodeComplete();
}


//-----------------------------------------------------------------------
//<主函数>  ::= void main（）｛<复合语句>｝ 
//-----------------------------------------------------------------------
void Parser::parse(pMain p)
{
	
	consume(Token::VOID);
	consume( Token::MAIN);
	consume(Token::OPENPAREN);
	consume(Token::CLOSEPAREN);
	consume( Token::OPENBRACE);
	
	createNewNode(p,new ast::CompoundStatement());

	consume(Token::CLOSEBRACE);

	p->nodeComplete();
}


//-----------------------------------------------------------------
//<常量定义>   ::=   int Identifier ＝<整数>{, Identifier＝<整数> }
//                   | float Identifier ＝<实数>{, Identifier＝<实数>}
//                    | char  Identifier ＝<ConstChar>{, Identifier＝<ConstChar>}
//				
//				预测:{int,float,char},FOLLOW = {;}
//-----------------------------------------------------------------
void Parser::parse(pConstDeclaration p)
{

	//int float char
	Token type = m_lexer.getNextToken();
	//p->addToken(type);

	switch( type.getKind() )
	{
	case Token::INT:
		{
			//Identifier = <整数>
			Token identifier = m_lexer.getNextToken();
			if( identifier.getKind() != Token::IDENTIFIER )
				error();
			p->addToken(identifier);
			
			consume( Token::ASSIGN);
			
			createNewNode(p,new ast::Integer());

			//{, Identifier＝<整数> }
			while( m_lexer.peekToken(1).getKind() == Token::COMMA)
			{
				consume(Token::COMMA);

				Token identifier = m_lexer.getNextToken();
				if( identifier.getKind() != Token::IDENTIFIER )
					error();
				p->addToken(identifier);
				
				consume( Token::ASSIGN);
				
				createNewNode(p,new ast::Integer());
			}
		}
		break;
	case Token::FLOAT:
		{
			//Identifier = <实数>
			Token identifier = m_lexer.getNextToken();
			check( identifier,Token::IDENTIFIER );
			p->addToken(identifier);
			
			consume( Token::ASSIGN);
			
			createNewNode(p,new ast::Real());

			//{, Identifier＝<实数> }
			while( m_lexer.peekToken().getKind()  == Token::COMMA)
			{
				consume(Token::COMMA);

				Token identifier = m_lexer.getNextToken();
				check( identifier,Token::IDENTIFIER );
				p->addToken(identifier);
				
				consume( Token::ASSIGN);
				
				createNewNode(p,new ast::Real());
			}
		}
		break;
	case Token::CHAR:
		{
			//Identifier = <ConstChar>
			Token identifier = m_lexer.getNextToken();
			check( identifier,Token::IDENTIFIER );
			p->addToken(identifier);
			
			consume( Token::ASSIGN);

			createNewNode(p,new ast::ConstChar());

			//{, Identifier＝<ConstChar>}
			while( m_lexer.peekToken().getKind()  == Token::COMMA )
			{
				consume(Token::COMMA);

				Token identifier = m_lexer.getNextToken();
				check( identifier,Token::IDENTIFIER );
				p->addToken(identifier);
				
				consume( Token::ASSIGN);
				
				createNewNode(p,new ast::ConstChar());
			}
		}
		break;
	default:
		error();
	}
	
	p->nodeComplete();	
	
}




//-----------------------------------------------------------------
//<<变量定义>  ::= <类型标识符> Identifier {, Identifier }
//				FOLLOW = {;}
//-----------------------------------------------------------------
void Parser::parse(pVariableDeclaration p)
{
	createNewNode(p,new ast::TypeSpecifier());

	Token identifier = m_lexer.getNextToken();
	check( identifier,Token::IDENTIFIER );
	p->addToken(identifier);
	
	
	while( m_lexer.peekToken().getKind() == Token::COMMA )
	{

		consume(Token::COMMA);

		Token identifier = m_lexer.getNextToken();
		check( identifier,Token::IDENTIFIER );
		p->addToken(identifier);
	}

	p->nodeComplete();
}

//------------------------------------------------------------------
//<声明头部>  ::= void Identifier| int Identifier|float Identifier|char Identifier
//------------------------------------------------------------------
void Parser::parse(pDeclarationHead p)
{
	Token t = m_lexer.getNextToken();
	p->addToken(t);

	Token identifier = m_lexer.getNextToken();
	check(identifier,Token::IDENTIFIER);
	p->addToken(identifier);

	p->nodeComplete();
}

//------------------------------------------------------------------
//<参数表>    ::=  <类型标识符>Identifier{，<类型标识符>Identifier}|<空>
//   
//------------------------------------------------------------------
void Parser::parse(pParamList p)
{

	//Not EPSILON
	if ( m_lexer.peekToken().getKind() != Token::CLOSEPAREN )
	{

		try{
			createNewNode(p,new ast::TypeSpecifier());

			Token identifier = m_lexer.getNextToken();
			check(identifier,Token::IDENTIFIER);
			p->addToken(identifier);

			while( m_lexer.peekToken().getKind() == Token::COMMA )
			{

				consume(Token::COMMA);

				createNewNode(p,new ast::TypeSpecifier());

				Token identifier = m_lexer.getNextToken();
				check(identifier,Token::IDENTIFIER);
				p->addToken(identifier);
			}
		}
		catch(err::Error e)
		{
			e.report();
			m_lexer.goForToken(Token::CLOSEPAREN);
		}

	}
	p->nodeComplete();
}



//------------------------------------------------------------------
//<复合语句> ::=  <常量说明部分><变量说明部分><语句列>
//	预测：{常量说明部分：const}{变量说明部分：int char float}
//------------------------------------------------------------------
void Parser::parse(pCompoundStatement p)
{
	// <常量说明部分>
	createNewNode(p,new ast::ConstDeclarationList());

	//<变量说明部分>
	createNewNode(p,new ast::VariableDeclarationList());

	//<语句列>
	createNewNode(p,new ast::StatementList());

	p->nodeComplete();
}

//---------------------------------------------------------------
//<ConstChar> ::= ConstChar
//---------------------------------------------------------------
void Parser::parse(pConstChar p)
{
	Token t = m_lexer.getNextToken();
	check(t,Token::CONSTCHAR);
	p->addToken(t);

	p->nodeComplete();
}

//---------------------------------------------------------------
//		<整数>  ::= [+|-]Num
//---------------------------------------------------------------
void Parser::parse(pInteger p)
{
	Token t = m_lexer.peekToken();
	if ( t.getKind() == Token::NUM)
	{
		p->addToken(m_lexer.getNextToken());
	}
	else if(t.getKind() == Token::ADD	||	t.getKind() == Token::SUB )
	{
		Token t1 = m_lexer.getNextToken();
		Token t2 = m_lexer.getNextToken();
		check(t2,Token::NUM);
		p->addToken(t1);
		p->addToken(t2);
	}else
		error();

	p->nodeComplete();
}


//---------------------------------------------------------------
//<实数>    ::=<整数> [<实数补充>] 
//---------------------------------------------------------------
void Parser::parse(pReal p)
{
	//<整数>
	createNewNode(p,new ast::Integer());

	if ( m_lexer.peekToken(1).getKind() == Token::DOT )
	{
		createNewNode(p,new ast::RealSuffix());
	}

	p->nodeComplete();
}


//---------------------------------------------------------------
//<类型标识符> ::=  int | float | char 
//---------------------------------------------------------------
void Parser::parse(pTypeSpecifier p)
{
	if( IsPrime(m_lexer) )
	{
		Token t = m_lexer.getNextToken();
		p->addToken(t);
	}else
		error();

	p->nodeComplete();
}

//---------------------------------------------------------------
//<语句列>  ::=  <语句>{<语句>}
//                 
//---------------------------------------------------------------
void Parser::parse(pStatementList p)
{
	try
	{
		createNewNode(p,new ast::Statement());
	}catch(err::Error e)
	{
		e.report();
		m_lexer.goForToken(ForGoFor::IsStatementEnd);
		m_lexer.getNextToken();
	}
	while ( IsStatement(m_lexer) )
	{
		try
		{
			createNewNode(p,new ast::Statement());
		}catch(err::Error e)
		{
			e.report();
			m_lexer.goForToken(ForGoFor::IsStatementEnd);
			m_lexer.getNextToken();
		}
	}

	p->nodeComplete();
}


//---------------------------------------------------------------
//<实数补充>    ::= .Num
//---------------------------------------------------------------
void Parser::parse(pRealSuffix p)
{
	consume(Token::DOT);

	Token num = m_lexer.getNextToken();
	check(num,Token::NUM);
	p->addToken(num);

	p->nodeComplete();
}


//----------------------------------------------------------
//<语句>    ::= <条件语句>|<循环语句>| ｛<语句列>｝
//		| <函数调用语句>；|<赋值语句>；|<读语句>；
//		|<写语句>；|<情况语句>|<返回语句>；|；
//	 
//	   预测：{条件语句：if}
//		 {循环语句：while}
//		 {｛<语句列>｝:｛}
//		 {函数调用语句: Identifier || （}
//		 {赋值语句：Identifier || ＝}
//		 {读语句：scanf}
//		 {写语句：printf}
//		 {情况语句：switch}
//		 {返回语句return}
//-----------------------------------------------------------

void Parser::parse(pStatement p)
{
	//；
	if ( m_lexer.peekToken().getKind() == Token::SEMICOLON )
	{
		consume( Token::SEMICOLON );
	}
	//<条件语句>
	else if ( m_lexer.peekToken().getKind() == Token::IF )
	{
		createNewNode(p,new ast::IfStatement());
		
	}
	//<循环语句>
	else if ( m_lexer.peekToken().getKind() == Token::WHILE )
	{
		createNewNode(p,new ast::WhileStatement());

	}
	//<函数调用语句>；
	else if( IsFunctionCall(m_lexer) )
	{
		createNewNode(p,new ast::FunctionCallStatement());

		consume(Token::SEMICOLON);

	}
	//<赋值语句>；
	else if( IsAssignMent(m_lexer) )
	{

		createNewNode(p,new ast::AssignmentStatement());

		consume(Token::SEMICOLON);

	}
	//<读语句>；
	else if( m_lexer.peekToken().getKind() == Token::SCANF )
	{
		createNewNode(p,new ast::ReadStatement());

		consume(Token::SEMICOLON);

	}
	//<写语句>；
	else if( m_lexer.peekToken().getKind() == Token::PRINTF )
	{
		createNewNode(p,new ast::WriteStatement());
		
		consume(Token::SEMICOLON);

	}
	//<返回语句>；
	else if( m_lexer.peekToken().getKind() == Token::RETURN )
	{
		createNewNode(p,new ast::ReturnStatement());

		consume(Token::SEMICOLON);

	}
	//<情况语句>
	else if( m_lexer.peekToken().getKind() == Token::SWITCH )
	{
		createNewNode(p,new ast::SwitchStatement());

	}
	//｛<语句列>｝
	else if( m_lexer.peekToken().getKind() == Token::OPENBRACE )
	{
		consume(Token::OPENBRACE);

		createNewNode(p,new ast::StatementList());

		consume(Token::CLOSEBRACE);

	}else
		error();
	

	p->nodeComplete();
}


//-----------------------------------------------------
//<条件语句>  ::=  if （<条件>）<语句>[else<语句>]
//-----------------------------------------------------
void Parser::parse(pIfStatement p)
{
	consume(Token::IF);
	consume(Token::OPENPAREN);

	//条件
	createNewNode(p,new ast::IfExpression());

	consume(Token::CLOSEPAREN);

	//语句
	createNewNode(p,new ast::Statement());

	if ( m_lexer.peekToken().getKind() == Token::ELSE )
	{
		consume(Token::ELSE);
		
		//语句
		createNewNode(p,new ast::Statement());
	}

	p->nodeComplete();
}


//-------------------------------------------------------
//<循环语句>   ::=  while （<条件>）<语句>
//-------------------------------------------------------
void Parser::parse(pWhileStatement p)
{
	consume(Token::WHILE);
	consume(Token::OPENPAREN);

	createNewNode(p,new ast::IfExpression());

	consume(Token::CLOSEPAREN);

	createNewNode(p,new ast::Statement());

	p->nodeComplete();
}



//-------------------------------------------
//<赋值语句>   ::=  Identifier＝<表达式>
//-------------------------------------------
void Parser::parse(pAssignmentStatement p)
{
	p->addToken( m_lexer.getNextToken() );

	consume(Token::ASSIGN);

	createNewNode(p,new ast::Expression());

	p->nodeComplete();
}

//-------------------------------------------
//<读语句>    ::=  scanf （Identifier{，Identifier}）
//-------------------------------------------
void Parser::parse(pReadStatement p)
{
	consume(Token::SCANF);
	consume(Token::OPENPAREN);

	Token identifier = m_lexer.getNextToken();
	check(identifier,Token::IDENTIFIER);
	p->addToken(identifier);

	while ( m_lexer.peekToken().getKind() == Token::COMMA )
	{
		consume(Token::COMMA);

		Token identifier = m_lexer.getNextToken();
		check(identifier,Token::IDENTIFIER);
		p->addToken(identifier);
	}

	consume(Token::CLOSEPAREN);

	p->nodeComplete();
}


//-------------------------------------------
//<写语句>    ::= printf （ String，<表达式> ）
//				| printf （ String ）
//				| printf （<表达式>）
//-------------------------------------------
void Parser::parse(pWriteStatement p)
{
	consume(Token::PRINTF);
	consume(Token::OPENPAREN);

	if ( m_lexer.peekToken().getKind() == Token::STRING )
	{
		p->addToken(m_lexer.getNextToken());

		if (m_lexer.peekToken().getKind() == Token::COMMA)
		{
			consume(Token::COMMA);

			createNewNode(p,new ast::Expression());
		}
	}else
	{
		createNewNode(p,new ast::Expression());
	}

	consume(Token::CLOSEPAREN);

	p->nodeComplete();
}


//---------------------------------------------------------
//<返回语句>   ::=  return[（<表达式>）]     
//---------------------------------------------------------
void Parser::parse(pReturnStatement p)
{
	consume(Token::RETURN);
	
	if ( m_lexer.peekToken().getKind() == Token::OPENPAREN )
	{
		consume(Token::OPENPAREN);

		createNewNode(p,new ast::Expression());

		consume(Token::CLOSEPAREN);
	}

	p->nodeComplete();
}

//---------------------------------------------------------------
//<函数调用语句> ::= Identifier（<值参数表>）
//---------------------------------------------------------------
void Parser::parse(pFunctionCallStatement p)
{
	check( m_lexer.peekToken(), Token::IDENTIFIER);
	p->addToken( m_lexer.getNextToken() );

	consume(Token::OPENPAREN);

	createNewNode(p,new ast::ValueParamList());

	consume(Token::CLOSEPAREN);

	p->nodeComplete();
}


//---------------------------------------------------------------
//<情况语句>  ::=  switch （<表达式>） ｛<情况表>[<缺省>]｝
//                    FOLLOW{非default则跳出}
//---------------------------------------------------------------
void Parser::parse(pSwitchStatement p)
{
	consume(Token::SWITCH);
	consume(Token::OPENPAREN);

	createNewNode(p,new ast::Expression());

	consume(Token::CLOSEPAREN);
	consume(Token::OPENBRACE);

	createNewNode(p,new ast::CaseStatementList());

	if ( m_lexer.peekToken().getKind() == Token::DEFAULT )
	{
		createNewNode(p,new ast::DefaultStatement());
	}

	consume(Token::CLOSEBRACE);

	p->nodeComplete();
}

//---------------------------------------------------
//<条件>  ::=  <表达式>[<关系运算符><表达式>] //只有表达式则表达式为0条件为假，否则为真
//                        FOLLOW{条件: ）}
//---------------------------------------------------
void Parser::parse(pIfExpression p)
{
	createNewNode(p,new ast::Expression());

	if ( IsRelationOp(m_lexer) )
	{
		p->addToken( m_lexer.getNextToken() );

		createNewNode(p,new ast::Expression());
	}

	p->nodeComplete();
}

//---------------------------------------------------------------
//<表达式>    ::= [+|-]<项>{(+|-)<项>}
//                        FOLLOW
//							{[+|-] : + - || Num}
//							{非加法运算符则跳出}
//---------------------------------------------------------------
void Parser::parse(pExpression p)
{

	//[+|-] 
	if ( IsAddOp(m_lexer) )
		p->addToken( m_lexer.getNextToken() );
	//else//默认添加+号,确定此节点以符号开始
	//	p->addToken ( m_lexer.createToken(Token::ADD) );

	//<项>
	createNewNode(p,new ast::Item());

	//{<加法运算符><项>}
	while( IsAddOp(m_lexer) )
	{
		p->addToken( m_lexer.getNextToken() );

		createNewNode(p,new ast::Item());
	}
	p->nodeComplete();
}


//---------------------------------------------------------------
//<项>     ::= <因子>{<乘法运算符><因子>}
//                       FOLLOW{非乘法运算符则跳出}
//---------------------------------------------------------------
void Parser::parse(pItem p)
{
	createNewNode(p,new ast::Factor());

	while( IsMulOp(m_lexer) )
	{
		p->addToken( m_lexer.getNextToken() );

		createNewNode(p,new ast::Factor());
	}

	p->nodeComplete();
}


//---------------------------------------------------------------
//<因子>    ::= Identifier|（<表达式>）|<实数>|<函数调用语句>|<ConstChar>       
//                    预测：{（<表达式>）:（}
//							{函数调用语句 : Identifier ||（ }
//							{实数:+ - Num}
//---------------------------------------------------------------
void Parser::parse(pFactor p)
{
	//<函数调用语句>
	if( IsFunctionCall(m_lexer) )
	{
		createNewNode(p,new ast::FunctionCallStatement());

		p->setKind(ast::Factor::FunctionCall);
	}
	//Identifier
	else if( m_lexer.peekToken().getKind() == Token::IDENTIFIER )
	{
		p->addToken(m_lexer.getNextToken());

		p->setKind(ast::Factor::Identifier);
	}
	//（<表达式>）
	else if( m_lexer.peekToken().getKind() == Token::OPENPAREN )
	{
		consume( Token::OPENPAREN );

		createNewNode(p,new ast::Expression());

		consume( Token::CLOSEPAREN);
		
		p->setKind(ast::Factor::Expression);
	}
	//<实数>
	else if( IsReal(m_lexer) )
	{
		createNewNode(p,new ast::Real());

		p->setKind(ast::Factor::Real);
	}
	//<ConstChar>
	else if( m_lexer.peekToken().getKind() == Token::CONSTCHAR)
	{
		createNewNode(p,new ast::ConstChar());

		p->setKind(ast::Factor::ConstChar);
	}else
		error();

	p->nodeComplete();
}




//---------------------------------------------------------------
//<值参数表>   ::= <表达式>{，<表达式>}|EPSILON
//                    FOLLOW:)
//					  FOLLOW{非，则跳出} 
//---------------------------------------------------------------
void Parser::parse(pValueParamList p)
{
	//Not EPSILON
	if ( m_lexer.peekToken().getKind() != Token::CLOSEPAREN )
	{

		createNewNode(p,new ast::Expression());

		while ( m_lexer.peekToken().getKind() == Token::COMMA )
		{
			consume(Token::COMMA);

			createNewNode(p,new ast::Expression());
		}
	}
	p->nodeComplete();
}


//----------------------------------------------------------
//<情况表>   ::=  <情况子语句>{<情况子语句>}
//                    FOLLOW{非case则跳出}
//----------------------------------------------------------
void Parser::parse(pCaseStatementList p)
{
	createNewNode(p,new ast::CaseStatement());

	while( m_lexer.peekToken().getKind() == Token::CASE )
	{
		createNewNode(p,new ast::CaseStatement());
	}

	p->nodeComplete();
}



//----------------------------------------------------------
//<情况子语句>  ::=  case<常量>:<语句>
//----------------------------------------------------------
void Parser::parse(pCaseStatement p)
{
	consume(Token::CASE);

	createNewNode(p,new ast::Const());

	consume(Token::COLON);

	createNewNode(p,new ast::Statement());


	p->nodeComplete();
}

//----------------------------------------------------------
//<常量>   ::=  <实数>|<ConstChar>
//----------------------------------------------------------
void Parser::parse(pConst p)
{
	if ( m_lexer.peekToken().getKind() == Token::CONSTCHAR )
	{
		createNewNode(p,new ast::ConstChar());
	}else 
	{
		createNewNode(p,new ast::Real());
	}

	p->nodeComplete();
}


//----------------------------------------------------------
//<缺省>   ::=  default:<语句>
//----------------------------------------------------------
void Parser::parse(pDefaultStatement p)
{
	consume(Token::DEFAULT);
	consume(Token::COLON);

	createNewNode(p,new ast::Statement());

	p->nodeComplete();
}




