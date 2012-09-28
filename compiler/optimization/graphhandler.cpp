#include "graphhandler.h"
#include "../tuple/tuple.h"
#include "../lookup/env.h"
#include "../error.h"
#include <cassert>
using namespace optimization;
//#define DEBUG

optimization::BaseBlock* GraphHandler::createNewBaseBlock()
{
	return new BaseBlock(m_env);
}


GraphHandler::GraphHandler(tuple::TupleHandler * next,int options)
	:m_nextHandler(next),m_currentBlock(NULL),m_options(options)
{


}

GraphHandler::~GraphHandler()
{
	std::vector<BaseBlock*>::iterator iter = m_baseBlocks.begin();
	while ( iter != m_baseBlocks.end() )
	{
		delete *iter;
		iter++;
	}
}

void optimization::GraphHandler::recieveTuple(lookup::Env* env,pTuple p)
{

	if (m_currentBlock == NULL)
		m_env = env;
	else
		assert ( m_env == env );

	BaseBlock* newblock;
	switch( p->getOp() )
	{
	case tuple::Tuple::JMP:
		m_currentBlock->addTuple(p);
		m_currentBlock->setGotoLabel( p->getSrc1() );
		m_baseBlocks.push_back(m_currentBlock);
		m_currentBlock = createNewBaseBlock();
		
		break;
	case tuple::Tuple::JE:
	case tuple::Tuple::JNE:
	case tuple::Tuple::JG:
	case tuple::Tuple::JGE:
	case tuple::Tuple::JL:
	case tuple::Tuple::JLE:
		m_currentBlock->addTuple(p);
		m_currentBlock->setGotoLabel( p->getTarget() );

		newblock = createNewBaseBlock();
		//相邻可达
		m_currentBlock->addToBlock (newblock);
		newblock->addFromBlock(m_currentBlock);

		m_baseBlocks.push_back(m_currentBlock);
		
		m_currentBlock = newblock;
		
		break;
	case tuple::Tuple::RET:
		m_currentBlock->addTuple(p);
		m_baseBlocks.push_back(m_currentBlock);
		m_currentBlock = createNewBaseBlock();
		break;
	case tuple::Tuple::LABEL:
		newblock = createNewBaseBlock();
		//相邻可达
		m_currentBlock->addToBlock( newblock );
		newblock->addFromBlock(m_currentBlock);

		m_baseBlocks.push_back(m_currentBlock);
	
		m_currentBlock = newblock;
		m_currentBlock->setInLabel(p->getSrc1());

		m_currentBlock->addTuple(p);
		break;
	case tuple::Tuple::FUNC:
		m_currentBlock = createNewBaseBlock();
		m_currentBlock->addTuple(p);
		break;
	case tuple::Tuple::ENDP:
	case tuple::Tuple::MAINEND:

		m_baseBlocks.push_back(m_currentBlock);
		flushFunction(p);
		break;

	default:
		m_currentBlock->addTuple(p);
	}


}



void GraphHandler::ConstructControlFlowGraph()
{
	std::vector<BaseBlock*>::iterator iter = m_baseBlocks.begin();
	while ( iter != m_baseBlocks.end() )
	{
		pSymbol gotolabel = (*iter)->getGotoLabel();
		if (gotolabel == NULL)
		{
			iter++;
			continue;
		}
		std::vector<BaseBlock*>::iterator iter2 = m_baseBlocks.begin();
		while( iter2 != m_baseBlocks.end() )
		{
			if ( (*iter2)->getInLabel() == gotolabel )
			{
				(*iter)->addToBlock(*iter2);
				(*iter2)->addFromBlock(*iter);
				break;
			}
			iter2++;
		}
		iter++;
	}
}

void GraphHandler::DAGOptimize()
{
	std::vector<BaseBlock*>::iterator iter = m_baseBlocks.begin();
	while ( iter != m_baseBlocks.end() )
	{
		(*iter)->optimize();
		iter++;
	}
}

void GraphHandler::CopyOptimize()
{
	std::vector<BaseBlock*>::iterator iter = m_baseBlocks.begin();
	while ( iter != m_baseBlocks.end() )
	{

		(*iter)->copyOptimize();
		iter++;
	}
}
void GraphHandler::DAGDisposeWasteAssign()
{
	std::vector<BaseBlock*>::iterator iter = m_baseBlocks.begin();
	while ( iter != m_baseBlocks.end() )
	{
		//消除无用的Assign
		(*iter)->disposeWasteAssign();
		iter++;
	}
}
void GraphHandler::AliveVarAnalysis()
{
	
	AliveVarInfo * info =new AliveVarInfo();
	assert(m_env->getAliveVarInfo() ==NULL);
	m_env->setAliveVarInfo( info );

	info->analysisBlocks( m_baseBlocks );
}


void GraphHandler::ConstructConflictGraph()
{
	AliveVarInfo * info = m_env->getAliveVarInfo();
	std::list<pSymbol> localvars = info->getLocalVars( m_env );
	
	ConflictGraph* conflictgraph = new ConflictGraph(localvars);

	std::list<pSymbol>::iterator iter = localvars.begin();
	while ( iter != localvars.end() )
	{
		std::list<pSymbol>::iterator iter2 = localvars.begin();
		while ( iter2 != localvars.end() )
		{
			conflictgraph->setConflict(*iter,*iter2, info->IsConflict(*iter,*iter2));
			iter2++;
		}
		iter++;
	}

	m_env->setConflictGraph(conflictgraph);
	
}


void GraphHandler::flushFunction(pTuple endpormainend)
{
	ConstructControlFlowGraph();

	if ( !warning::Warning::IsWarning() && (m_options & tuple::TupleManager::DAG))
	{
		DAGOptimize();

		if (m_options & tuple::TupleManager::COPY)
			CopyOptimize();
	}

	if (m_options & tuple::TupleManager::REGISTER)
	{
		AliveVarAnalysis();
		DAGDisposeWasteAssign();

		ConstructConflictGraph();
	}

	

	assert(endpormainend->getOp() == tuple::Tuple::ENDP ||
		endpormainend->getOp() == tuple::Tuple::MAINEND );

	std::vector<BaseBlock*>::iterator iter = m_baseBlocks.begin();

	while ( iter != m_baseBlocks.end() )
	{
		std::list<pTuple> tuples = (*iter)->getTuples();
		std::list<pTuple>::iterator iter2 = tuples.begin();
		while( iter2 != tuples.end() )
		{
			m_nextHandler->recieveTuple(m_env,*iter2);

#ifdef DEBUG
			std::cout<<"    "<<(*iter2)->toString()<<std::endl;
#endif
			iter2++;
		}
		iter++;
	}

	m_nextHandler->recieveTuple(m_env,endpormainend);

	m_baseBlocks.clear();

	m_currentBlock = NULL;

}
