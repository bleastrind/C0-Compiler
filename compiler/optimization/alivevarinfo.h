#ifndef __BLEASTRIND_ALIVEVARINFO
#define __BLEASTRIND_ALIVEVARINFO
#include <map>
#include <vector>
#include "baseblock.h"
#include "dbitset.h"
#include "../lookup/envfwd.h"
namespace optimization
{

	class AliveVarInfo
	{
	public:
		void analysisBlocks(std::vector<BaseBlock*> );
		bool WillAlive(pTuple,pSymbol);
		bool IsAlive(pTuple,pSymbol);
		int getRefCount(pSymbol);
		std::list<pSymbol> getLocalVars(lookup::Env*);
		bool IsConflict(pSymbol,pSymbol);
	private:

		std::map<pSymbol,int> m_symbolIndexMap;
		std::vector<pSymbol> m_symbols;
		//引用计数
		std::vector<int> m_refContinueMaxCount;
		std::vector<int> m_refTempCount;

		std::map<pTuple,DBitSet> m_useInfo;
		std::map<pTuple,DBitSet> m_useInfoInclude;

		//下标对应BaseBlock
		std::vector<DBitSet> m_use;
		std::vector<DBitSet> m_def;
		std::vector<DBitSet> m_in;
		std::vector<DBitSet> m_out;


		void initBlocks(std::vector<BaseBlock*> &);
		void findInOutOfEachBlock(std::vector<BaseBlock*> &);
		void fillUseInfo(std::vector<BaseBlock*> &);


		//return int represent the symbol in dbitset
		unsigned int getIndex(pSymbol);
		//return int represent the block in blocks
		unsigned int getIndex(BaseBlock*);
		std::map<BaseBlock*,int> m_baseBlockIndexMap;

		void use(int blockid,pSymbol);
		void def(int blockid,pSymbol);
		void findUseAndDefOfBlock(int,BaseBlock*,DBitSet out);

		bool isOverBaseBlock(unsigned int);
		
	};

	

}//namespace
#endif
