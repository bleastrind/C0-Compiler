#ifndef __BLEASTRIND_GRAPHHANDLER
#define __BLEASTRIND_GRAPHHANDLER
#include "../tuple/tuplemanager.h"
#include "baseblock.h"
#include "conflictgraph.h"
#include "alivevarinfo.h"
#include <vector>
namespace optimization
{
	class GraphHandler:public tuple::TupleHandler
	{
	public:
		GraphHandler(tuple::TupleHandler * next,int options);
		virtual ~GraphHandler();
		void recieveTuple(lookup::Env*,pTuple);
		void flushFunction(pTuple endpormainend);

	private:
		tuple::TupleHandler* m_nextHandler;

		lookup::Env* m_env;


		BaseBlock* m_currentBlock;
		std::vector<BaseBlock*> m_baseBlocks;
		
		BaseBlock* createNewBaseBlock();
		void ConstructControlFlowGraph();

		int m_options;
		void DAGOptimize();
		void CopyOptimize();

		void AliveVarAnalysis();
		void DAGDisposeWasteAssign();

		void ConstructConflictGraph();
	};
}//namespace
#endif
