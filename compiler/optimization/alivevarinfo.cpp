#include "alivevarinfo.h"
#include "../tuple/tuple.h"
#include <cassert>
#include <iostream>
#include "../lookup/env.h"
#include "../lookup/symbol.h"
using namespace optimization;
//#define DEBUG

bool AliveVarInfo::WillAlive(pTuple ptuple,pSymbol psymbol)
{
	if ( !psymbol || ptuple->getOp() == tuple::Tuple::ENDP || ptuple->getOp() == tuple::Tuple::MAINEND)
		return false;
	assert( m_useInfo.find(ptuple) != m_useInfo.end() );
	unsigned int symbolindex = getIndex(psymbol);
	return m_useInfo[ptuple].find(symbolindex);
}

bool AliveVarInfo::IsAlive(pTuple ptuple,pSymbol psymbol)
{
	if ( ptuple->getOp() == tuple::Tuple::ENDP || ptuple->getOp() == tuple::Tuple::MAINEND)
		return false;
	assert( m_useInfoInclude.find(ptuple) != m_useInfoInclude.end() );
	unsigned int symbolindex = getIndex(psymbol);
	return m_useInfoInclude[ptuple].find(symbolindex);
}



void AliveVarInfo::analysisBlocks(std::vector<BaseBlock*> blocks)
{
	//找出每个block的use和def

	initBlocks(blocks);

	
	findInOutOfEachBlock(blocks);


	//补充每条tuple的信息
	fillUseInfo(blocks);

#ifdef DEBUG
	std::cout<<"Symbols:"<<std::endl;
	for (unsigned int j=0;j < m_symbols.size();j++)
	{
		std::cout<<m_symbols[j]->getName()<<"="<<m_symbols[j]->toString()<<":\t"<<m_symbolIndexMap[m_symbols[j]]<<std::endl;
	}

	for (unsigned int i=0;i < blocks.size(); i++)
	{
		std::cout<<"Use:"<<i<<": "<< m_use[i] <<std::endl;
		std::cout<<"Def:"<<i<<": "<< m_def[i] <<std::endl;
		std::cout<<"In:"<<i<<": "<< m_in[i] <<std::endl;
		std::cout<<"Out:"<<i<<": "<< m_out[i] <<std::endl;
		std::cout<<std::endl;

		std::list<pTuple> tuples = blocks[i]->getTuples();
		std::list<pTuple>::iterator iter = tuples.begin();
		while (iter != tuples.end())
		{
			std::cout<<(*iter)->toString()<<": "<<m_useInfo[*iter]<<std::endl;
			iter++;
		}
	}
#endif
}

void AliveVarInfo::initBlocks(std::vector<BaseBlock*> &blocks)
{
	for (unsigned int i =0 ; i < blocks.size() ;i ++)
	{
		m_use.push_back( DBitSet());
		m_def.push_back( DBitSet());
		m_in.push_back(DBitSet());
		m_out.push_back(DBitSet());
		m_baseBlockIndexMap[ blocks[i] ] = i;
		findUseAndDefOfBlock(i, blocks[i] ,DBitSet());
	}

}

void AliveVarInfo::findInOutOfEachBlock(std::vector<BaseBlock*> &blocks)
{
	//循环直至in不变
	bool changed = true;
	while ( changed )
	{
		changed = false;
		for (unsigned int thisblockid =0 ; thisblockid < blocks.size() ;thisblockid ++)
		{
			std::list<BaseBlock*> tos = blocks[thisblockid]->getToBlocks();
			std::list<BaseBlock*>::iterator iter = tos.begin();
			while( iter != tos.end() )
			{
				unsigned int toblockid = getIndex(*iter);
				m_out[thisblockid] += m_in[toblockid];
				iter++;
			}
			DBitSet oldin = m_in[thisblockid];
			m_in[thisblockid] = m_use[thisblockid] + ( m_out[thisblockid] - m_def[thisblockid] );
			
			if (oldin != m_in[thisblockid])
				changed = true;
		}
	}
}

void AliveVarInfo::fillUseInfo(std::vector<BaseBlock*> &blocks)
{
	for (unsigned int i =0 ; i < blocks.size() ;i ++)
	{
		findUseAndDefOfBlock(i, blocks[i] ,m_out[i]);
	}
}

int AliveVarInfo::getRefCount(pSymbol p)
{
	unsigned int index = getIndex (p);
	return m_refContinueMaxCount[index];
}

void AliveVarInfo::use(int blockid,pSymbol p)
{
	unsigned int index = getIndex (p);
	m_use[blockid] += index;
	m_def[blockid] -= index;

	m_refTempCount[index] ++;
}
void AliveVarInfo::def(int blockid,pSymbol p)
{
	unsigned int index = getIndex (p);
	m_def[blockid] += index;
	m_use[blockid] -= index;

	if ( m_refTempCount[index] > m_refContinueMaxCount[index])
		 m_refContinueMaxCount[index] = m_refTempCount[index];
	m_refTempCount[index] = 0;
}
void AliveVarInfo::findUseAndDefOfBlock(int i,BaseBlock* block,DBitSet out)
{
	std::list<pTuple> tuples = block->getTuples();

	m_use[i] = out;

	std::list<pTuple>::reverse_iterator riter = tuples.rbegin();
	while ( riter != tuples.rend() )
	{

		//不包括本条在内的使用信息
		m_useInfo[*riter] = m_use[i];

		switch( (*riter)->getOp())
		{
			

		case tuple::Tuple::JE:
		case tuple::Tuple::JNE:
		case tuple::Tuple::JG:
		case tuple::Tuple::JGE:
		case tuple::Tuple::JL:
		case tuple::Tuple::JLE:
			use(i, (*riter)->getSrc1() );
			use(i, (*riter)->getSrc2() );
			
			break;
		case tuple::Tuple::PARAM:
			use(i, (*riter)->getSrc1() );
			break;

		case tuple::Tuple::ASSIGN:
			def(i, (*riter)->getTarget() );
			use(i, (*riter)->getSrc1() );
			
			break;
		case tuple::Tuple::ADD:
		case tuple::Tuple::SUB:
		case tuple::Tuple::MUL:
		case tuple::Tuple::DIV:
			def(i, (*riter)->getTarget() );
			use(i, (*riter)->getSrc1() );
			use(i, (*riter)->getSrc2() );
			break;
		case tuple::Tuple::RET:
		case tuple::Tuple::PRINTF:
			if ( (*riter)->getSrc2() )
				use(i, (*riter)->getSrc2() );
			break;
		case tuple::Tuple::SCANF:
			def(i , (*riter)->getTarget());
			break;
		case tuple::Tuple::CALL:
			if ( (*riter)->getTarget() )
				def(i, (*riter)->getTarget() );
			break;
		default:
			;
		
		}

		//包括本条在内的使用信息
		m_useInfoInclude[*riter] = m_use[i];

		riter++;
	}

}

bool AliveVarInfo::IsConflict(pSymbol p1,pSymbol p2)
{

	unsigned int index1 = getIndex(p1);
	unsigned int index2 = getIndex(p2);

	std::map<pTuple,DBitSet>::iterator iter = m_useInfoInclude.begin();
	while( iter != m_useInfoInclude.end() )
	{
		if (iter->second.find(index1) && iter->second.find(index2) )
			return true;
		iter++;
	}

	return false;
}


unsigned int AliveVarInfo::getIndex(pSymbol p)
{
	if (!p)
	{
		int a=0,b;
		b = 6/a;
	}	
	assert(p);
	if ( m_symbolIndexMap.find(p) == m_symbolIndexMap.end() )
	{
		m_symbolIndexMap[p] = m_symbols.size();
		m_symbols.push_back( p );
		m_refContinueMaxCount.resize(m_symbols.size());
		m_refTempCount.resize(m_symbols.size());
	}
	return m_symbolIndexMap[p];
}

unsigned int AliveVarInfo::getIndex(BaseBlock* p)
{
	assert(p);
	assert ( m_baseBlockIndexMap.find(p) != m_baseBlockIndexMap.end() );
	
	return m_baseBlockIndexMap[p];
}

bool AliveVarInfo::isOverBaseBlock(unsigned int symbolindex)
{
	for (unsigned int i = 0; i < m_in.size(); i++)
		if (m_in[i].find(symbolindex))
			return true;
	return false;
}


std::list<pSymbol> AliveVarInfo::getLocalVars(lookup::Env * env)
{
	std::list<pSymbol> res;
	for (unsigned int i = 0 ; i < m_symbols.size() ;i++)
	{
		//仅局部变量
		if ( env->findVariableOrParamOnLevel(m_symbols[i]->getName()) )
		{
			//必须跨越基本块且非浮点数
			if (isOverBaseBlock(i) && m_symbols[i]->getType() != lookup::Float )			
				res.push_back( m_symbols[i] );
			assert( m_symbols[i] == env->findVariableOrParamOnLevel(m_symbols[i]->getName()) );
	
		}
	}
	return res;
}
