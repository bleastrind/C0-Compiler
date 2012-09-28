#include "env.h"
#include "symbol.h"
#include <assert.h>
#include "../util.h"
#include "../tuple/tuplemanager.h"
#include "../optimization/alivevarinfo.h"
#include "../optimization/conflictgraph.h"
using namespace lookup;
lookup::Env* lookup::Env::newMain()
{

	lookup::Function* mainfunc = new lookup::Function("main",lookup::Null);

	lookup::Env* main = new lookup::Env();
	main->setFunction(mainfunc);
	return main;
}

lookup::Symbol* lookup::Env::findSymbolOnLevel(std::string name) const
{	
	lookup::Symbol* p;
	if( ( p = findParam(name) ) != NULL )
		return p;
	else if ( m_consts.end() != m_consts.find( name ) )
		return m_consts[name];
	else if( m_functions.end() != m_functions.find( name ) )
		return m_functions[name];
	else if( m_variables.end() != m_variables.find( name ) )
		return m_variables[name];
	else
		return NULL;

}
lookup::Variable* lookup::Env::findParam(std::string name) const
{
	std::list<lookup::Variable*>::const_iterator iter = m_params.begin();
	while ( iter != m_params.end() )
	{
		if( (*iter)->toString() == name )
			return *iter;
		iter ++;
	}
	return NULL;
}
lookup::Symbol* lookup::Env::findSymbol(std::string name) const
{
	lookup::Symbol * psymbol = findSymbolOnLevel(name) ;
	if ( NULL != psymbol)
		return psymbol;
	else if(m_parent != NULL)
		return m_parent->findSymbolOnLevel(name);
	else
		return NULL;
}

bool lookup::Env::addConst(lookup::Const * p)
{
	if ( findSymbolOnLevel( p->getName() ) != NULL )
	{
		delete p;
		return false;
	}
	m_consts[p->getName()]= p;
	return true;
}

lookup::Const* lookup::Env::findConst(std::string name) const
{
	if ( m_consts.find(name) != m_consts.end() )
		return m_consts[name];
	else if( m_parent != NULL)
		return m_parent->findConst(name);
	return NULL;
}

lookup::Const* lookup::Env::findConstOnLevel(std::string name) const
{
	if ( m_consts.find(name) != m_consts.end() )
		return m_consts[name];
	return NULL;
}


bool lookup::Env::addParam(lookup::Variable * p)
{	
	if ( findSymbolOnLevel( p->getName() ) != NULL )
	{
		delete p;
		return false;
	}
	m_params.push_back( p);
	return true;
}

bool lookup::Env::addVariable(lookup::Variable* p)
{
	if ( findSymbolOnLevel( p->getName() ) != NULL )
	{
		delete p;
		return false;
	}
	m_variables[p->getName()]= p;
	return true;
}


bool lookup::Env::addFunction(lookup::Function * pfunc)
{
	if ( m_functions.end() != m_functions.find( pfunc->getName() ) )
	{
		delete pfunc;
		return false;
	}
	m_functions[pfunc->getName()]= pfunc;
	return true;
}
lookup::Variable* lookup::Env::findVariableOnLevel(std::string name) const
{
	if ( m_variables.find(name) != m_variables.end() )
		return m_variables[name];
	else
		return NULL;
}

lookup::Variable* lookup::Env::findVariableOrParam(std::string name) const
{
	lookup::Variable * p;

	if ( ( p = findVariableOrParamOnLevel(name) ) != NULL )
		return p;
	//父节点找
	else if( m_parent != NULL)
		return m_parent->findVariableOrParam(name);
	else
		return NULL;
}

lookup::Variable* lookup::Env::findVariableOrParamOnLevel(std::string name) const
{
	lookup::Variable * p;
	//找局部变量
	if ( m_variables.find(name) != m_variables.end() )
		return m_variables[name];
	//找参数
	else if ( ( p = findParam(name) ) != NULL )
		return p;
	else
		return NULL;
}

lookup::Function* lookup::Env::findFunction(std::string name) const
{
	if ( m_functions.find(name) != m_functions.end() )
		return m_functions[name];
	else if( m_parent != NULL)
		return m_parent->findFunction(name);
	else
		return NULL;
}


lookup::Env::~Env()
{
	std::list<lookup::Variable *>::iterator iter = m_params.begin();
	while ( iter != m_params.end() )
	{
		delete *iter;
		iter++;
	}

	std::map<std::string,lookup::Variable *>::iterator iter2 = m_variables.begin();
	while ( iter2 != m_variables.end() )
	{
		delete iter2->second;
		iter2++;
	}
	std::map<std::string,lookup::Const *>::iterator iter3 = m_consts.begin();
	while ( iter3 != m_consts.end() )
	{
		delete iter3->second;
		iter3++;
	}
	std::map<std::string,lookup::Function *>::iterator iter4 = m_functions.begin();
	while ( iter4 != m_functions.end() )
	{
		delete iter4->second;
		iter4++;
	}

	delete m_aliveVarInfo;
	delete m_conflictGraph;
}



Function* Env::createFunction(std::string name, lookup::ValueType valuetype)
{
	return new Function(name,valuetype);
}

Variable* Env::createVariable(std::string name,lookup::ValueType t)
{
	switch ( t )
	{
	case lookup::Char:
		return new Variable( name,lookup::Char);
	case lookup::Int:
		return new Variable( name,lookup::Int);
	case lookup::Float:
		return new Variable( name,lookup::Float);
	default:
		assert(0);
		return NULL;
	}
}

Const* Env::createConst(std::string name,int value) 
{ 
	return new lookup::ConstInt( name,value ); 
}
Const* Env::createConst(std::string name,float value)
{ 
	return new lookup::ConstFloat( name,value ); 
}
Const* Env::createConst(std::string name,char value)
{ 
	return new lookup::ConstChar( name,value ); 
}

namespace
{
	int tempcount = 0;
}
Variable* Env::createTemp(lookup::ValueType type)
{

	return new lookup::Variable("@tmp"+util::ToString(tempcount++),type);
}


std::map<int,Const*> Env::s_constintPool;
std::map<float,Const*> Env::s_constfloatPool;
std::map<char,Const*> Env::s_constcharPool;
std::map<std::string,String*> Env::s_conststringPool;
std::list<Label*> Env::s_labelPool;

namespace
{
	int floatcount = 0;
}
Const* Env::createConst(float value)
{
	if ( s_constfloatPool.find(value) != s_constfloatPool.end() )
		return s_constfloatPool[value];
	Const* p = new lookup::ConstFloat("float"+util::ToString(floatcount++),value);
	s_constfloatPool[value] = p;

	return p;
}
Const* Env::createConst(int value)
{
	if ( s_constintPool.find(value) != s_constintPool.end() )
		return s_constintPool[value];
	Const* p = new lookup::ConstInt("",value);
	s_constintPool[value] = p;

	return p;
}
Const* Env::createConst(char value)
{
	if ( s_constcharPool.find(value) != s_constcharPool.end() )
		return s_constcharPool[value];
	Const* p = new lookup::ConstChar("",value);
	s_constcharPool[value] = p;

	return p;
}

Const* Env::createZero()
{
	return createConst('\0');
}

String* Env::createString(std::string context,std::string suffix)
{
	if ( s_conststringPool.find( context + suffix ) != s_conststringPool.end() )
		return s_conststringPool[context + suffix ];
	String* p = new lookup::String(context,suffix);
	s_conststringPool[context + suffix ] = p;

	return p;
}

Label* Env::createLabel()
{
	Label* ans = new Label();
	s_labelPool.push_back(ans);
	return ans;
}

bool Env::IsGloablConst(lookup::Symbol * p)
{
	std::map<float,lookup::Const*>::iterator iter =  s_constfloatPool.begin();
	while(iter != s_constfloatPool.end())
	{
		if (iter->second == p)
			return true;
		iter++;
	}
	
	std::map<std::string,String*>::iterator iter2 = s_conststringPool.begin();
	while(iter2 != s_conststringPool.end())
	{
		if (iter2->second == p)
			return true;
		iter2++;
	}
	return false;
}
