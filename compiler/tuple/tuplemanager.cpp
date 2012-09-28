#include "tuplemanager.h"
#include "../lookup/env.h"
#include "../lookup/symbol.h"
#include "tuple.h"
#include "../optimization/graphhandler.h"
#include "../x86/basehandler.h"
#include "../error.h"
#include <iostream>
//#define DEBUG
tuple::TupleHandler* tuple::TupleManager::s_handler_head ;
lookup::Env* tuple::TupleManager::s_pEnv;

const int tuple::TupleManager::DAG = 0x1;
const int tuple::TupleManager::REGISTER = 0x2;
const int tuple::TupleManager::COPY = 0x4;
const int tuple::TupleManager::DAGANDREGISTER = 0x3;
const int tuple::TupleManager::ALL = 0x7;
void tuple::TupleManager::initHandlers(std::ostream& output,int options,bool changeline)
{

	s_handler_head = new x86::BaseHandler(output,0,changeline);

	
	if ( options )
	{
		s_handler_head = new optimization::GraphHandler(s_handler_head,options);
			
	}
	
	
}

void tuple::TupleManager::addCode(lookup::Env* env,pTuple t)
{
	if (s_pEnv != env)
	{
		s_pEnv = env;
#ifdef DEBUG
		std::cout<<"env changed"<<std::endl;
#endif
	}

	//t->setID(s_pTuples.size());
	#ifdef DEBUG
	std::cout<< t->toString()<<std::endl;
	#endif

	if ( !err::Error::IsError() )
		s_handler_head->recieveTuple(env, t);
}

void tuple::TupleManager::addCode(lookup::Env* env,std::vector<pTuple> ts)
{

	if (s_pEnv != env)
	{
		s_pEnv = env;
#ifdef DEBUG
		std::cout<<"env changed"<<std::endl;
#endif
	}

	std::vector<pTuple>::iterator iter = ts.begin();

	while( iter != ts.end() )
	{
		addCode(env,*iter);
		iter++;
	}
}

void tuple::TupleManager::addCode( pTuple t)
{
	addCode(s_pEnv,t);
}

void tuple::TupleManager::addCode(std::vector<pTuple> ts)
{
	addCode(s_pEnv,ts);
}

void tuple::TupleManager::addFunction(lookup::Env* env)
{
	pTuple ptuple =tuple::Tuple::createTuple(tuple::Tuple::FUNC, env->getFunction(),NULL,NULL);
	//TODO alloc vars
	addCode(env,ptuple);
}

void tuple::TupleManager::putLabel(pSymbol p)
{
	pTuple ptuple =tuple::Tuple::createTuple(tuple::Tuple::LABEL,p,NULL,NULL);
	addCode(ptuple);
}