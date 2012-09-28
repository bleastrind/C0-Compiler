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

	//void int float char || Identifier || ��
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
//<����>    ::= <����˵������><����˵������>{<�������岿��>}<������>
//				  Ԥ��:{����˵������:const}
//				  {����˵������:int float char || identifier || ;}
//				  {�������岿��:void int float char || Identifier || ��}
//				  {������:void || main}
//-----------------------------------------------------------------------
void Parser::parse(pCompileUnit p)
{

	try
	{
		//<����˵������>
		createNewNode(p,new ast::ConstDeclarationList());
	}catch(err::Error e)
	{
		e.report();
		m_lexer.goForToken( ForGoFor::IsVariableDeclarationList );

	}


	
	try
	{
		//<����˵������>
		createNewNode(p,new ast::VariableDeclarationList());
	}catch(err::Error e)
	{
		e.report();
		m_lexer.goForToken( ForGoFor::IsFunctionDeclarationList );
		
	}

	//{<�������岿��>}

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
//<����˵������> ::=  { const<��������>;}
//			FOLLOW��const
//-----------------------------------------------------------------------
void Parser::parse(pConstDeclarationList p)
{
	//{ const<��������>;}
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
//<����˵������> ::= {<��������> ;}
//		FOLLOW{��Ϊ int float char ||Identifier ||  ��   ��תΪ�з���ֵ����}
//-----------------------------------------------------------------------
void Parser::parse(pVariableDeclarationList p)
{
	//{<��������> ;}
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
//<�������岿��>  ::=  <����ͷ��>��<������>�� ��<�������>��
//-----------------------------------------------------------------------
void Parser::parse(pFunctionDeclaration p)
{
	//<����ͷ��>
	createNewNode(p,new ast::DeclarationHead());

	//��<������>��
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
	
	//��<�������>��
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
//<������>  ::= void main������<�������>�� 
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
//<��������>   ::=   int Identifier ��<����>{, Identifier��<����> }
//                   | float Identifier ��<ʵ��>{, Identifier��<ʵ��>}
//                    | char  Identifier ��<ConstChar>{, Identifier��<ConstChar>}
//				
//				Ԥ��:{int,float,char},FOLLOW = {;}
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
			//Identifier = <����>
			Token identifier = m_lexer.getNextToken();
			if( identifier.getKind() != Token::IDENTIFIER )
				error();
			p->addToken(identifier);
			
			consume( Token::ASSIGN);
			
			createNewNode(p,new ast::Integer());

			//{, Identifier��<����> }
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
			//Identifier = <ʵ��>
			Token identifier = m_lexer.getNextToken();
			check( identifier,Token::IDENTIFIER );
			p->addToken(identifier);
			
			consume( Token::ASSIGN);
			
			createNewNode(p,new ast::Real());

			//{, Identifier��<ʵ��> }
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

			//{, Identifier��<ConstChar>}
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
//<<��������>  ::= <���ͱ�ʶ��> Identifier {, Identifier }
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
//<����ͷ��>  ::= void Identifier| int Identifier|float Identifier|char Identifier
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
//<������>    ::=  <���ͱ�ʶ��>Identifier{��<���ͱ�ʶ��>Identifier}|<��>
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
//<�������> ::=  <����˵������><����˵������><�����>
//	Ԥ�⣺{����˵�����֣�const}{����˵�����֣�int char float}
//------------------------------------------------------------------
void Parser::parse(pCompoundStatement p)
{
	// <����˵������>
	createNewNode(p,new ast::ConstDeclarationList());

	//<����˵������>
	createNewNode(p,new ast::VariableDeclarationList());

	//<�����>
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
//		<����>  ::= [+|-]Num
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
//<ʵ��>    ::=<����> [<ʵ������>] 
//---------------------------------------------------------------
void Parser::parse(pReal p)
{
	//<����>
	createNewNode(p,new ast::Integer());

	if ( m_lexer.peekToken(1).getKind() == Token::DOT )
	{
		createNewNode(p,new ast::RealSuffix());
	}

	p->nodeComplete();
}


//---------------------------------------------------------------
//<���ͱ�ʶ��> ::=  int | float | char 
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
//<�����>  ::=  <���>{<���>}
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
//<ʵ������>    ::= .Num
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
//<���>    ::= <�������>|<ѭ�����>| ��<�����>��
//		| <�����������>��|<��ֵ���>��|<�����>��
//		|<д���>��|<������>|<�������>��|��
//	 
//	   Ԥ�⣺{������䣺if}
//		 {ѭ����䣺while}
//		 {��<�����>��:��}
//		 {�����������: Identifier || ��}
//		 {��ֵ��䣺Identifier || ��}
//		 {����䣺scanf}
//		 {д��䣺printf}
//		 {�����䣺switch}
//		 {�������return}
//-----------------------------------------------------------

void Parser::parse(pStatement p)
{
	//��
	if ( m_lexer.peekToken().getKind() == Token::SEMICOLON )
	{
		consume( Token::SEMICOLON );
	}
	//<�������>
	else if ( m_lexer.peekToken().getKind() == Token::IF )
	{
		createNewNode(p,new ast::IfStatement());
		
	}
	//<ѭ�����>
	else if ( m_lexer.peekToken().getKind() == Token::WHILE )
	{
		createNewNode(p,new ast::WhileStatement());

	}
	//<�����������>��
	else if( IsFunctionCall(m_lexer) )
	{
		createNewNode(p,new ast::FunctionCallStatement());

		consume(Token::SEMICOLON);

	}
	//<��ֵ���>��
	else if( IsAssignMent(m_lexer) )
	{

		createNewNode(p,new ast::AssignmentStatement());

		consume(Token::SEMICOLON);

	}
	//<�����>��
	else if( m_lexer.peekToken().getKind() == Token::SCANF )
	{
		createNewNode(p,new ast::ReadStatement());

		consume(Token::SEMICOLON);

	}
	//<д���>��
	else if( m_lexer.peekToken().getKind() == Token::PRINTF )
	{
		createNewNode(p,new ast::WriteStatement());
		
		consume(Token::SEMICOLON);

	}
	//<�������>��
	else if( m_lexer.peekToken().getKind() == Token::RETURN )
	{
		createNewNode(p,new ast::ReturnStatement());

		consume(Token::SEMICOLON);

	}
	//<������>
	else if( m_lexer.peekToken().getKind() == Token::SWITCH )
	{
		createNewNode(p,new ast::SwitchStatement());

	}
	//��<�����>��
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
//<�������>  ::=  if ��<����>��<���>[else<���>]
//-----------------------------------------------------
void Parser::parse(pIfStatement p)
{
	consume(Token::IF);
	consume(Token::OPENPAREN);

	//����
	createNewNode(p,new ast::IfExpression());

	consume(Token::CLOSEPAREN);

	//���
	createNewNode(p,new ast::Statement());

	if ( m_lexer.peekToken().getKind() == Token::ELSE )
	{
		consume(Token::ELSE);
		
		//���
		createNewNode(p,new ast::Statement());
	}

	p->nodeComplete();
}


//-------------------------------------------------------
//<ѭ�����>   ::=  while ��<����>��<���>
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
//<��ֵ���>   ::=  Identifier��<���ʽ>
//-------------------------------------------
void Parser::parse(pAssignmentStatement p)
{
	p->addToken( m_lexer.getNextToken() );

	consume(Token::ASSIGN);

	createNewNode(p,new ast::Expression());

	p->nodeComplete();
}

//-------------------------------------------
//<�����>    ::=  scanf ��Identifier{��Identifier}��
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
//<д���>    ::= printf �� String��<���ʽ> ��
//				| printf �� String ��
//				| printf ��<���ʽ>��
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
//<�������>   ::=  return[��<���ʽ>��]     
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
//<�����������> ::= Identifier��<ֵ������>��
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
//<������>  ::=  switch ��<���ʽ>�� ��<�����>[<ȱʡ>]��
//                    FOLLOW{��default������}
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
//<����>  ::=  <���ʽ>[<��ϵ�����><���ʽ>] //ֻ�б��ʽ����ʽΪ0����Ϊ�٣�����Ϊ��
//                        FOLLOW{����: ��}
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
//<���ʽ>    ::= [+|-]<��>{(+|-)<��>}
//                        FOLLOW
//							{[+|-] : + - || Num}
//							{�Ǽӷ������������}
//---------------------------------------------------------------
void Parser::parse(pExpression p)
{

	//[+|-] 
	if ( IsAddOp(m_lexer) )
		p->addToken( m_lexer.getNextToken() );
	//else//Ĭ�����+��,ȷ���˽ڵ��Է��ſ�ʼ
	//	p->addToken ( m_lexer.createToken(Token::ADD) );

	//<��>
	createNewNode(p,new ast::Item());

	//{<�ӷ������><��>}
	while( IsAddOp(m_lexer) )
	{
		p->addToken( m_lexer.getNextToken() );

		createNewNode(p,new ast::Item());
	}
	p->nodeComplete();
}


//---------------------------------------------------------------
//<��>     ::= <����>{<�˷������><����>}
//                       FOLLOW{�ǳ˷������������}
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
//<����>    ::= Identifier|��<���ʽ>��|<ʵ��>|<�����������>|<ConstChar>       
//                    Ԥ�⣺{��<���ʽ>��:��}
//							{����������� : Identifier ||�� }
//							{ʵ��:+ - Num}
//---------------------------------------------------------------
void Parser::parse(pFactor p)
{
	//<�����������>
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
	//��<���ʽ>��
	else if( m_lexer.peekToken().getKind() == Token::OPENPAREN )
	{
		consume( Token::OPENPAREN );

		createNewNode(p,new ast::Expression());

		consume( Token::CLOSEPAREN);
		
		p->setKind(ast::Factor::Expression);
	}
	//<ʵ��>
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
//<ֵ������>   ::= <���ʽ>{��<���ʽ>}|EPSILON
//                    FOLLOW:)
//					  FOLLOW{�ǣ�������} 
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
//<�����>   ::=  <��������>{<��������>}
//                    FOLLOW{��case������}
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
//<��������>  ::=  case<����>:<���>
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
//<����>   ::=  <ʵ��>|<ConstChar>
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
//<ȱʡ>   ::=  default:<���>
//----------------------------------------------------------
void Parser::parse(pDefaultStatement p)
{
	consume(Token::DEFAULT);
	consume(Token::COLON);

	createNewNode(p,new ast::Statement());

	p->nodeComplete();
}




