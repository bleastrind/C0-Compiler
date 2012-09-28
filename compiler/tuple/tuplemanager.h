#ifndef __BLEATRIND_TUPLEMANAGER
#define __BLEATRIND_TUPLEMANAGER 
#include <vector>
#include "tuplefwd.h"
#include "../lookup/envfwd.h"
#include "../lookup/symbolfwd.h"
namespace tuple
{


	//-------------------------------------------
	//处理四元式的类
	//-------------------------------------------
	class TupleHandler
	{
	public:
		virtual void recieveTuple(lookup::Env*,pTuple)=0;
	};

	class TupleManager
	{
	public:

		static const int DAG;
		static const int REGISTER;
		static const int COPY;
		static const int DAGANDREGISTER;
		static const int ALL;
		static void initHandlers(std::ostream& ,int option,bool changeline);

		static void addCode(pTuple);
		static void addCode(std::vector<pTuple>);		
		static void addCode(lookup::Env*,pTuple);
		static void addCode(lookup::Env*, std::vector<pTuple>);
		static void putLabel(pSymbol);
		static void addFunction(lookup::Env*);
	private:

		static TupleHandler* s_handler_head;
		//保存当前环境
		static lookup::Env* s_pEnv;

	};
}//namespace
#endif
