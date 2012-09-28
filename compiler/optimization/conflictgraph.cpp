#include "conflictgraph.h"
#include "alivevarinfo.h"
#include <cassert>
using namespace optimization;

ConflictGraph::ConflictGraph(std::list<pSymbol> symbols)
{
	unsigned int size = symbols.size();
	m_conflict.resize(size);
	for (unsigned int i = 0; i < size ; i++ )
		m_conflict[i].resize(size);

	std::list<pSymbol>::iterator iter = symbols.begin();
	while ( iter != symbols.end() )
	{
		m_symbolIndexMap[*iter] = m_symbols.size();
		m_symbols.push_back(*iter);
		iter++;
	}
}

pSymbol ConflictGraph::findFreeNode(int k)
{
	for (unsigned int i = 0; i < m_conflict.size(); i++)
	{
		if (!m_symbols[i])
			continue;
		int conflicttime = 0;
		for (unsigned int j = 0; j < m_conflict[i].size(); j++)
		{
			if ( m_conflict[i][j] )
				conflicttime ++;
		}
		if ( conflicttime <= k)
			return m_symbols[i];
	}
	return NULL;
}

pSymbol ConflictGraph::removeSymbol(pSymbol p)
{
	typedef std::vector<bool> boolvector;
	std::vector<boolvector>::iterator iter = m_conflict.begin();
	
	unsigned int index = m_symbolIndexMap[p];
	

	for (unsigned int i = 0; i < m_conflict.size(); i++)
		m_conflict[i][index] = false;

	for (unsigned int j = 0; j < m_conflict.size(); j++)
		m_conflict[index][j] = false;

	m_symbols[index] = NULL;
	
	std::map<pSymbol,unsigned int>::iterator symbolindexiter = m_symbolIndexMap.find(p);
	m_symbolIndexMap.erase(symbolindexiter);

	return p;
}

void ConflictGraph::setConflict(pSymbol p1, pSymbol p2, bool conflict)
{
	assert( m_symbolIndexMap.find(p1) != m_symbolIndexMap.end() );
	assert( m_symbolIndexMap.find(p2) != m_symbolIndexMap.end() );
	unsigned int i1 = m_symbolIndexMap[p1];
	unsigned int i2 = m_symbolIndexMap[p2];
	m_conflict[i1][i2] = conflict;

}

unsigned int ConflictGraph::size()
{
	return m_symbolIndexMap.size();
}

std::list<pSymbol> ConflictGraph::getConflictSymbols(pSymbol p)
{
	std::list<pSymbol> res;
	unsigned int index = m_symbolIndexMap[p];
	for (unsigned int j = 0;j < m_conflict[index].size(); j++)
	{
		if ( m_conflict[index][j] )
			res.push_back( m_symbols[j] );
	}
	return res;
}

//依据引用计数
pSymbol ConflictGraph::getfirst(AliveVarInfo * info)
{
	int maxref = 0;
	pSymbol ans = NULL;
	for (unsigned int i = 0; i < m_symbols.size() ; i++)
		if ( m_symbols[i] )
		{
			int c = info->getRefCount( m_symbols[i]);
			if ( c >= maxref )
			{
				c = maxref;
				ans = m_symbols[i];
			}
		}

	assert(ans);
	return ans;
}

