#include "const.h"
#include "real.h"
#include "constchar.h"
#include "../error.h"
void ast::Const::addNode(pConstChar p)
{
	//确保自动删除
	std::auto_ptr<ast::ASTNode> point_manager(p);
	
	if( !m_src1 || m_src1->getType() == lookup::Null)
		throw err::Error(err::UNKNOWN);
	m_src2 = lookup::Env::createConst(p->getValue());

	m_target = lookup::Env::createLabel();

	genCode();
}

void ast::Const::addNode(pReal p)
{
	//确保自动删除
	std::auto_ptr<ast::ASTNode> point_manager(p);

	if (p->getType() > m_src1->getType() )
		warning::Warning(warning::BADCONVERT).report();
	
	switch( p->getType() )
	{
	case lookup::Int:
		m_src2 = lookup::Env::createConst(p->getInt());
		break;
	case lookup::Float:
		m_src2 = lookup::Env::createConst(p->getValue());
		break;
	default:
		throw err::Error(err::UNKNOWN);
	}

	m_target = lookup::Env::createLabel();

	genCode();
}
