#include "functioncallstatement.h"
#include "valueparamlist.h"
#include "../error.h"
#include "../lookup/symbol.h"


void ast::FunctionCallStatement::prepareNode(pValueParamList p)
{
	if(!m_function)
		throw err::Error(err::UNKNOWN);
	p->setFunction(m_function);
}

void ast::FunctionCallStatement::addNode(pValueParamList p)
{
		//ȷ���Զ�ɾ��
	std::auto_ptr<ast::ASTNode> point_manager(p);
	
	//CALL
	genCode();
}

void ast::FunctionCallStatement::addToken(Token identifier)
{
	if(identifier.getKind() != Token::IDENTIFIER)
		throw err::Error(err::UNKNOWN);

	//����ֵ����
	m_function = getEnv()->findFunction(identifier.getString());
	if (m_function == NULL)
	{
		err::Error(err::IDENTIFIER_NOT_DECLARED,identifier).report();
		return;
	}
	m_valuetype = m_function->getType();


	//����tuple���ã�������genCode����ѹ���������genCode
	m_src1 = m_function;
	m_op = tuple::Tuple::CALL;
	if ( m_valuetype != lookup::Null )
	{
		//create NEW temp and add to env	
		lookup::Variable *ptemp = lookup::Env::createTemp( m_function->getType() );
		getEnv()->addVariable(ptemp);
		m_target = ptemp;
	}
	


}