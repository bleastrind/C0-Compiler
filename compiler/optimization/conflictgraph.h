#ifndef __BLEASTRIND_CONFLICTGRAPH
#define __BLEASTRIND_CONFLICTGRAPH
#include "../lookup/symbolfwd.h"
#include "alivevarinfofwd.h"
#include <list>
#include <map>
#include <vector>
namespace optimization
{

	//class Node
	//{
	//public:
	//	Node(pSymbol p):m_symbol(p){}
	//	pSymbol getSymbol() { return m_symbol; }

	//	void addConflict(pSymbol p) { m_conflicts.push_back(p); }
	//	std::list<pSymbol> getConflictSymbols() { return m_conflicts; }
	//private:
	//	pSymbol m_symbol;
	//	std::list<pSymbol> m_conflicts;
	//};

	class ConflictGraph
	{
	public:
		ConflictGraph(std::list<pSymbol>);


		void setConflict(pSymbol,pSymbol,bool);

		std::list<pSymbol> getConflictSymbols(pSymbol);
		pSymbol getfirst(AliveVarInfo * info);
		pSymbol findFreeNode(int registernumb);
		pSymbol removeSymbol(pSymbol);
		unsigned int size();
		typedef std::vector<bool> boolvector;
		std::vector<boolvector>& getConflictMatrix() { return m_conflict; }
	private:
		std::vector<pSymbol> m_symbols;
		std::map<pSymbol,unsigned int> m_symbolIndexMap;
		std::vector<boolvector> m_conflict;
	};



}//namespace
#endif
