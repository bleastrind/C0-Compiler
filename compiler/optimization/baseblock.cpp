#include "baseblock.h"
#include "../tuple/tuple.h"
#include "../lookup/symbol.h"
#include "../lookup/env.h"
#include "alivevarinfo.h"
#include <assert.h>
#include <stack>
#include <algorithm>
using namespace optimization;

int DAGnode::s_id=0;
BaseBlock::~BaseBlock()
{
	std::list<DAGnode*>::iterator iter = m_dagnodes.begin();
	while ( iter != m_dagnodes.end() )
	{
		delete *iter;
		iter++;
	}

	std::list<pTuple>::iterator iter2 = m_newtuples.begin();
	while (iter2 != m_newtuples.end() )
	{
		delete *iter2;
		iter2++;
	}
}
namespace{
	struct WasteAssignFunctionObject 
	{
		AliveVarInfo* m_info;
		lookup::Env* m_env;
		WasteAssignFunctionObject(AliveVarInfo * info,lookup::Env* env):m_info(info),m_env(env){}
		bool operator()(pTuple p)
		{
			if (p->getOp() != tuple::Tuple::ASSIGN)
				return false;
			assert(p->getTarget());
			if ( !m_info->WillAlive(p,p->getTarget()) && m_env->findVariableOrParamOnLevel(p->getTarget()->getName()) )
				return true;
			return false;
		}
	};
}
void BaseBlock::disposeWasteAssign()
{
	assert(m_env);
	AliveVarInfo * info = m_env->getAliveVarInfo();
	if ( ! info)
		return;

	struct WasteAssignFunctionObject pred = WasteAssignFunctionObject(info,m_env);
	std::list<pTuple>::iterator iter = m_tuples.begin();
	while(iter != m_tuples.end() )
	{
		if (pred(*iter))
			m_tuples.erase(iter);
		iter++;
	}
}



void BaseBlock::optimize()
{
	std::list<pTuple>::iterator iter = m_tuples.begin();
	while ( iter != m_tuples.end() )
	{
		switch( (*iter)->getOp() )
		{
			case tuple::Tuple::JMP:
			case tuple::Tuple::JE:
			case tuple::Tuple::JNE:
			case tuple::Tuple::JG:
			case tuple::Tuple::JGE:
			case tuple::Tuple::JL:
			case tuple::Tuple::JLE:
			case tuple::Tuple::PRINTF:
			case tuple::Tuple::SCANF:
			case tuple::Tuple::RET:
			case tuple::Tuple::CALL:
				restore();
				m_restuples.push_back( *iter );
				break;
			
			case tuple::Tuple::ASSIGN:
				dealwithAssign(*iter);
				break;
			case tuple::Tuple::PARAM:
				dealwithParam(*iter);
				break;
			case tuple::Tuple::ADD:
			case tuple::Tuple::SUB:
			case tuple::Tuple::MUL:
			case tuple::Tuple::DIV:
				dealwithThreeTuple(*iter);
				break;
			case tuple::Tuple::LABEL:
			case tuple::Tuple::FUNC:
				m_restuples.push_back( *iter );
				break;
			case tuple::Tuple::ENDP:
			case tuple::Tuple::MAINEND:
				#define SHOUND_NOT_IN_BASEBLOCK 0
				assert(SHOUND_NOT_IN_BASEBLOCK);
				break;

			default:
				#define NEW_TUPLE 0
				assert(NEW_TUPLE);				
		}
		iter++;
	}
	restore();

	m_tuples = m_restuples;
}
namespace
{

	bool IsFree(std::list<optimization::DAGnode*> undonenodes,DAGnode * node)
	{

		std::list<optimization::DAGnode*> parents = node->getParents();
		std::list<optimization::DAGnode*>::iterator iter =parents.begin();
		while ( iter != parents.end() )
		{
			if ( std::find(undonenodes.begin(),undonenodes.end(),*iter) != undonenodes.end() )
				return false;
			iter++;
		}
		return true;

	}
	optimization::DAGnode* findAFreeNode(std::list<optimization::DAGnode*> undonenodes)
	{
		std::list<optimization::DAGnode*>::reverse_iterator iter = undonenodes.rbegin();
		while ( iter != undonenodes.rend() )
		{
			if ( IsFree(undonenodes,*iter) )
				return *iter;
			iter++;
		}
		assert(0);
		return NULL;
	}
}
void BaseBlock::restore()
{
	std::stack<DAGnode*> nodes;
	while( !m_midnodes.empty() )
	{
		DAGnode* node = findAFreeNode(m_midnodes);
		nodes.push(node);
		m_midnodes.remove(node);
		

		while ( true )
		{
			node = node->getLeft();
			
			if (node 
				&& std::find( m_midnodes.begin(), m_midnodes.end(), node) != m_midnodes.end()
				&& IsFree(m_midnodes ,node )
				)
			{
				nodes.push(node);
				m_midnodes.remove(node);
			}
			else
				break;
			
		}
	}

	while ( !nodes.empty() )
	{
		reCreateTuple( nodes.top() );
		nodes.pop();
	}

	assert(m_midnodes.empty());

	reAssign();

	m_lastparam = NULL;
	m_nodetable.clear();
}
void BaseBlock::reAssign()
{
	std::map<pSymbol,DAGnode*>::iterator iter = m_nodetable.begin();
	while ( iter != m_nodetable.end() )
	{
		if ( iter->second->getSymbol() != iter->first)
		//	//|| iter->first->getName()[0] != '@' )
			m_restuples.push_back(
				tuple::Tuple::createTuple(
				tuple::Tuple::ASSIGN,
				iter->second->getSymbol(),
				NULL,
				iter->first
				));
		iter++;
	}
}

void BaseBlock::reCreateTuple(DAGnode* node)
{
	
	switch( node->getOp() )
	{
		case tuple::Tuple::ADD:
		case tuple::Tuple::SUB:
		case tuple::Tuple::MUL:
		case tuple::Tuple::DIV:
		
			{

				assert(node->getLeft());
				assert(node->getRight());
				assert(node->getLeft()->getSymbol());
				assert(node->getRight()->getSymbol());
				assert(node->getSymbol());

				tuple::Tuple* newtuple = tuple::Tuple::createTuple(
					node->getOp(),
					node->getLeft()->getSymbol(),
					node->getRight()->getSymbol(),
					node->getSymbol()
					);
				m_newtuples.push_back( newtuple );
				m_restuples.push_back( newtuple	);
			}
			break;
		case tuple::Tuple::PARAM:
			{

				assert(node->getLeft());
				assert(node->getLeft()->getSymbol());
				assert(node->getSymbol());
				pSymbol toparam = node->getLeft()->getSymbol();
				if ( node->getSymbol()->getType() != node->getLeft()->getSymbol()->getType() )
				{		
					
					toparam = node->getSymbol();

					tuple::Tuple* newtuple =tuple::Tuple::createTuple(
						tuple::Tuple::ASSIGN,
						node->getLeft()->getSymbol(),
						NULL,
						toparam
						);
					m_newtuples.push_back( newtuple );
					m_restuples.push_back( newtuple	);

				}
				tuple::Tuple* newtuple =tuple::Tuple::createTuple(
					node->getOp(),
					toparam,
					NULL,
					NULL
					);
				
				m_newtuples.push_back( newtuple );
				m_restuples.push_back( newtuple	);
			}
			break;
		default:
			assert(0);
	}
}


int BaseBlock::tempcount = 0;
DAGnode* BaseBlock::createLeafNode(pSymbol p)
{
	DAGnode* newnode = new DAGnode();
	
	pSymbol src0 = p;

	if ( m_env->findVariableOrParam(src0->getName()) )
	{
		lookup::Variable* newsymbol = m_env->createVariable("@"+src0->getName()+"0" + util::ToString(tempcount++) ,src0->getType());
		m_env->addVariable( newsymbol );
		tuple::Tuple* newtuple = tuple::Tuple::createTuple(
			tuple::Tuple::ASSIGN,
			src0,
			NULL,
			newsymbol
			);
		m_restuples.push_back(newtuple);
		src0 = newsymbol;
	}

	newnode->setSymbol(src0);
	m_dagnodes.push_back( newnode );
	m_nodetable[p] = newnode;

	return newnode;
}

void BaseBlock::dealwithAssign(pTuple p)
{
	DAGnode* src = m_nodetable[p->getSrc1()];
	
	if ( src == NULL)
	{
		src = createLeafNode(p->getSrc1());
	}else if ( src->getOp() != tuple::Tuple::Null )
		src->setSymbol(p->getTarget());

	m_nodetable[p->getTarget()] = src;

}

void BaseBlock::dealwithParam(pTuple p)
{
	DAGnode * x = m_nodetable[p->getSrc1()];
	
	//找现有的x，如果不存在新建
	if ( x == NULL)
	{
		x = createLeafNode(p->getSrc1());
	}

	DAGnode * newnode = new DAGnode();
	newnode->setOp(p->getOp());
	newnode->setLeft(x);
	newnode->setSymbol(p->getSrc1());//record type for rebuild
	
	x->addParent(newnode);

	if (m_lastparam)
		m_lastparam->addParent(newnode);
	m_lastparam = newnode;

	m_midnodes.push_back(newnode);
	m_dagnodes.push_back(newnode);
}


void BaseBlock::dealwithThreeTuple(pTuple p)
{
	DAGnode* x = m_nodetable[p->getSrc1()];
	DAGnode* y = m_nodetable[p->getSrc2()];

	//找现有的x，如果不存在新建
	if ( x == NULL)
	{
		x = createLeafNode(p->getSrc1());
	}
	//找现有的y，如果不存在新建
	if ( y == NULL )
	{
		y = createLeafNode(p->getSrc2());
	}

	//找节点类型相同且左右相同的节点
	std::list<DAGnode*>::iterator iter = m_dagnodes.begin();

	DAGnode* opnode = NULL;
	while ( iter != m_dagnodes.end() )
	{
		if ( (*iter)->getOp() == p->getOp() 
			&& (*iter)->getLeft() == x
			&& (*iter)->getRight() == y
			)
		{
			opnode = *iter;
			break;
		}
		iter++;
	}

	//没找到则新建
	if (opnode == NULL)
	{
		opnode = new DAGnode();
		
		opnode->setLeft(x);
		x->addParent(opnode);
		
		opnode->setRight(y);
		y->addParent(opnode);

		opnode->setOp(p->getOp());
		opnode->setSymbol(p->getTarget());
		m_midnodes.push_back( opnode );
		m_dagnodes.push_back( opnode );
	}

	//此节点为target
	m_nodetable[p->getTarget()] = opnode; 
	



}


void BaseBlock::copyOptimize()
{
std::list<pTuple>::iterator iter = m_tuples.begin();
	while ( iter != m_tuples.end() )
	{
		switch( (*iter)->getOp() )
		{
			
			case tuple::Tuple::JE:
			case tuple::Tuple::JNE:
			case tuple::Tuple::JG:
			case tuple::Tuple::JGE:
			case tuple::Tuple::JL:
			case tuple::Tuple::JLE:
			case tuple::Tuple::PARAM:
			case tuple::Tuple::ADD:
			case tuple::Tuple::SUB:
			case tuple::Tuple::MUL:
			case tuple::Tuple::DIV:
			case tuple::Tuple::RET:
			case tuple::Tuple::PRINTF:
				(*iter)->setSrc1( copy_getValue((*iter)->getSrc1())  );
				(*iter)->setSrc2( copy_getValue((*iter)->getSrc2())  );
				copy_clear((*iter)->getTarget() );
				break;
			case tuple::Tuple::SCANF:
			case tuple::Tuple::CALL:
				copy_clear((*iter)->getTarget() );
				break;

			case tuple::Tuple::ASSIGN:
				copy_clear( (*iter)->getTarget() );
				
				m_assigns[(*iter)->getTarget()] = copy_getValue( (*iter) ->getSrc1());
				break;

			case tuple::Tuple::LABEL:
			case tuple::Tuple::FUNC:
			case tuple::Tuple::JMP:
			case tuple::Tuple::ENDP:
			case tuple::Tuple::MAINEND:
				
				break;

			default:
				#define NEW_TUPLE 0
				assert(NEW_TUPLE);				
		}
		iter++;
	}
}
void BaseBlock::copy_clear(pSymbol p)
{
	if (p == NULL)
		return;
	std::map<pSymbol,pSymbol>::iterator iter = m_assigns.begin();
	while( iter != m_assigns.end())
	{
		if (iter->second == p)
		{
			iter->second = iter->first;
		}else if (iter->first == p)
		{
			iter->second = p;
		}
		iter++;
	}

	

}
pSymbol BaseBlock::copy_getValue(pSymbol p)
{
	if (p == NULL)
		return NULL;
	if ( m_assigns[p] == NULL )
		m_assigns[p] = p;
	return m_assigns[p];
}
