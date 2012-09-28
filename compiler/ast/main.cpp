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
	//ȷ���Զ�ɾ��
	std::auto_ptr<ast::ASTNode> point_manager(p);
	
	
	//��������
	pTuple ret = tuple::Tuple::createTuple(tuple::Tuple::RET,getEnv()->getFunction(),NULL,NULL);
	tuple::TupleManager::addCode(ret);

	//����������
	pTuple endp = tuple::Tuple::createTuple(tuple::Tuple::MAINEND,NULL,NULL,NULL);
	tuple::TupleManager::addCode(endp);
}
