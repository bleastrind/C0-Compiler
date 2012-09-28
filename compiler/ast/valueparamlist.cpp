#include "valueparamlist.h"
#include "expression.h"
#include "../lookup/symbol.h"
#include "../error.h"
void ast::ValueParamList::addNode(pExpression p)
{
	//确保自动删除
	std::auto_ptr<ast::ASTNode> point_manager(p);
	if (!m_pfunc)
		return;

	if (m_paramindex >= m_pfunc->getParamCount())
		throw err::Error(err::FUNCTION_WORING_PARAMS);
	//check, set paramindex + 1
	lookup::ValueType paramtype = m_pfunc->getParamKind(m_paramindex ++);
	if ( p->getType() > paramtype )
		warning::Warning(warning::BADCONVERT).report();


	lookup::Variable* temp = lookup::Env::createTemp(paramtype);
	getEnv()->addVariable(temp);
	
	//calc param
	m_target = temp;
	m_op = tuple::Tuple::ASSIGN;
	m_src1 = p->getResult();
	m_src2 = NULL;
	genCode();

	m_paramstack.push(m_target);



}

void ast::ValueParamList::nodeComplete()
{

	if (!m_pfunc)
		return;

	if (  m_paramstack.size() != m_pfunc->getParamCount() )
		throw err::Error(err::FUNCTION_WORING_PARAMS);

	while ( !m_paramstack.empty() )
	{
		//deliver param
		m_src1 = m_paramstack.top();
		m_op = tuple::Tuple::PARAM;
		m_src2 = NULL;
		m_target = NULL;
		genCode();


		m_paramstack.pop();
	}

	
}