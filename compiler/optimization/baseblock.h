#ifndef __BLEASTRIND_BASEBLOCK
#define __BLEASTRIND_BASEBLOCK
#include <list>
#include <map>
#include "../lookup/symbolfwd.h"
#include "../lookup/envfwd.h"
#include "../tuple/tuple.h"
namespace optimization
{

	class DAGnode
	{
	public:
		DAGnode():id(s_id++),m_left(NULL),m_right(NULL),m_symbol(NULL),m_op(tuple::Tuple::Null){}
		bool equals();

		std::list<DAGnode*> getParents() { return m_parents; }
		DAGnode* getRight() { return m_right; }
		DAGnode* getLeft() { return m_left; }
		tuple::Tuple::OpType getOp() { return m_op; }
		pSymbol getSymbol() { return m_symbol; }

		void addParent(DAGnode* d) { m_parents.push_back(d); }
		void setRight(DAGnode* d) { m_right = d; }
		void setLeft(DAGnode* d) { m_left = d; }
		void setOp(tuple::Tuple::OpType op) { m_op = op; }
		void setSymbol(pSymbol p) { m_symbol = p; }
	private:
		//Just for debug
		static int s_id;
		int id;

		std::list<DAGnode*> m_parents;
		DAGnode* m_left;
		DAGnode* m_right;
		pSymbol m_symbol;
		tuple::Tuple::OpType m_op;
	};

	class BaseBlock
	{
	public:

		BaseBlock(lookup::Env *env):m_env(env),m_inlabel(NULL),m_gotolabel(NULL),m_lastparam(NULL){}
		virtual ~BaseBlock();

		std::list<pTuple> getTuples() { return m_tuples; }
		std::list<BaseBlock*> getFromBlocks() { return m_fromBlocks; }
		std::list<BaseBlock*> getToBlocks() { return m_toBlocks; }

		pSymbol getInLabel() { return m_inlabel; }
		pSymbol getGotoLabel() { return m_gotolabel; }

		void addToBlock(BaseBlock* tb) { m_toBlocks.push_back(tb); }
		void addFromBlock(BaseBlock* fb) { m_fromBlocks.push_back(fb); }
		void addTuple(pTuple p){ m_tuples.push_back(p); }

		void setInLabel(pSymbol p) { m_inlabel = p; }
		void setGotoLabel(pSymbol p) { m_gotolabel = p; }

		//优化！
		void copyOptimize();
		void optimize();
		void disposeWasteAssign();
	private:
		std::list<pTuple> m_tuples;
		std::list<pTuple> m_restuples;
		std::list<BaseBlock*> m_fromBlocks;
		std::list<BaseBlock*> m_toBlocks;

		lookup::Env* m_env;

		

		//----------------------------------------------
		//for copyOptimize
		//----------------------------------------------
		std::map<pSymbol,pSymbol> m_assigns;

		void copy_clear(pSymbol);
		pSymbol copy_getValue(pSymbol);

		//----------------------------------------------
		//for localDAG
		//----------------------------------------------
		std::map<pSymbol,DAGnode*> m_nodetable;
		static int tempcount;
		
		//中间节点
		std::list<DAGnode*> m_midnodes;


		DAGnode* createLeafNode(pSymbol);
		std::list<DAGnode*> m_dagnodes;
		std::list<pTuple> m_newtuples;

		pSymbol m_inlabel;
		pSymbol m_gotolabel;

		void restore();
		void reCreateTuple(DAGnode*);
		void reAssign();

		void dealwithAssign(pTuple);
		DAGnode * m_lastparam;
		void dealwithParam(pTuple);
		void dealwithThreeTuple(pTuple);


	};



}//namespace
#endif
