#ifndef _BLEASTRIND_ASTNODESFWD
#define _BLEASTRIND_ASTNODESFWD
namespace ast{
class AbstractConst;
class ASTNode;
class AssignmentStatement;
class CaseStatement;
class CaseStatementList;

class CompileUnit;
class CompoundStatement;
class Const;
class ConstChar;
class ConstDeclaration;

class ConstDeclarationList;
class DeclarationHead;
class DefaultStatement;
class Expression;
class Factor;

class FunctionCallStatement;
class FunctionDeclaration;
class IfExpression;
class IfStatement;
class Integer;

class Item;
class Main;
class ParamList;
class ReadStatement;
class Real;

class RealSuffix;
class ReturnStatement;
class Statement;
class StatementList;
class SwitchStatement;

class TypeSpecifier;
class ValueParamList;
class VariableDeclaration;
class VariableDeclarationList;
class WhileStatement;

class WriteStatement;
}


typedef ast::AbstractConst * pAbstractConst;
typedef ast::ASTNode * pASTNode;
typedef ast::AssignmentStatement * pAssignmentStatement;
typedef ast::CaseStatement * pCaseStatement;
typedef ast::CaseStatementList * pCaseStatementList;

typedef ast::CompileUnit * pCompileUnit;
typedef ast::CompoundStatement * pCompoundStatement;
typedef ast::Const * pConst;
typedef ast::ConstChar * pConstChar;
typedef ast::ConstDeclaration * pConstDeclaration;

typedef ast::ConstDeclarationList * pConstDeclarationList;
typedef ast::DeclarationHead * pDeclarationHead;
typedef ast::DefaultStatement * pDefaultStatement;
typedef ast::Expression * pExpression;
typedef ast::Factor * pFactor;

typedef ast::FunctionCallStatement * pFunctionCallStatement;
typedef ast::FunctionDeclaration * pFunctionDeclaration;
typedef ast::IfExpression * pIfExpression;
typedef ast::IfStatement * pIfStatement;
typedef ast::Integer * pInteger;

typedef ast::Item * pItem;
typedef ast::Main * pMain;
typedef ast::ParamList * pParamList;
typedef ast::ReadStatement * pReadStatement;
typedef ast::Real * pReal;

typedef ast::RealSuffix * pRealSuffix;
typedef ast::ReturnStatement * pReturnStatement;
typedef ast::Statement * pStatement;
typedef ast::StatementList * pStatementList;
typedef ast::SwitchStatement * pSwitchStatement;

typedef ast::TypeSpecifier * pTypeSpecifier;
typedef ast::ValueParamList * pValueParamList;
typedef ast::VariableDeclaration * pVariableDeclaration;
typedef ast::VariableDeclarationList * pVariableDeclarationList;
typedef ast::WhileStatement * pWhileStatement;

typedef ast::WriteStatement * pWriteStatement;

#endif
