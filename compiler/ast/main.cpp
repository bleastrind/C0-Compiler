#include "main.h"
#include "compoundstatement.h"
#include "../lookup/symbol.h"
#include "../tuple/tuple.h"
#include "../tuple/tuplemanager.h"
ast::Main::Main():m_pEnv(lookup::Env::newMain())
{
	tuple::TupleManager::addFunction(getEnv());
}

void ast::Main::addNode(pCompoundStatement p)
{
	//确保自动删除
	std::auto_ptr<ast::ASTNode> point_manager(p);
	
	
	//函数返回
	pTuple ret = tuple::Tuple::createTuple(tuple::Tuple::RET,getEnv()->getFunction(),NULL,NULL);
	tuple::TupleManager::addCode(ret);

	//主函数结束
	pTuple endp = tuple::Tuple::createTuple(tuple::Tuple::MAINEND,NULL,NULL,NULL);
	tuple::TupleManager::addCode(endp);
}
