#include "registermanager.h"
#include "../tuple/tuple.h"
#include "../lookup/symbol.h"
#include "../lookup/env.h"
#include "../optimization/alivevarinfo.h"
#include "../optimization/conflictgraph.h"
#include <stack>
#include <cassert>
#include <algorithm>

//x86::RegisterManager(lookup::Env *env,std::ostream & ostream):m_env(env),m_ostream(ostream)
//{
//	//m_changedGlobalReg.push_back(x86::RegisterManager::EBX);
//}

std::string x86::RegisterManager::toString(const x86::RegisterManager::Register reg,bool lowbyte)
{
	#define CANTPUTCHARINEDIORESI 0
	switch (reg)
	{
	case x86::RegisterManager::EAX:
		return lowbyte ? "al":"eax";
	case x86::RegisterManager::EBX:
		return lowbyte ? "bl":"ebx";
	case x86::RegisterManager::ECX:
		return lowbyte ? "cl":"ecx";
	case x86::RegisterManager::EDI:
		return lowbyte ? assert(CANTPUTCHARINEDIORESI),"EDI":"edi";
	case x86::RegisterManager::EDX:
		return lowbyte ? "dl":"edx";
	case x86::RegisterManager::ESI:
		return lowbyte ? assert(CANTPUTCHARINEDIORESI),"ESI":"esi";
	default:
		assert(0);
		return "";
	}
}

x86::RegisterManager::Register x86::RegisterManager::getUseless()
{
	x86::RegisterManager::Register ans = x86::RegisterManager::FAILED;

	optimization::AliveVarInfo* info = m_env ? m_env->getAliveVarInfo() : NULL ;
	if (! info)
		return ans;
	
	std::map<RegisterManager::Register,pSymbol>::iterator iter = m_registermap.begin();
	
	while (iter != m_registermap.end())
	{
		if ( !info->WillAlive(m_tuple, iter->second) )
			return iter->first;
		if ( !info->IsAlive(m_tuple, iter->second) )
			ans = iter->first;
		iter++;
	}

	return ans;
}


x86::RegisterManager::Register x86::RegisterManager::getRegister(const pSymbol p) 
{


	std::map<RegisterManager::Register,pSymbol>::iterator iter = m_registermap.begin();
	while (iter != m_registermap.end())
	{
		if (iter->second == p)
			return iter->first;
		iter++;
	}

	std::vector<pSymbol> & ebxvars = m_globalassignment[x86::RegisterManager::EBX];
	std::vector<pSymbol> & edivars = m_globalassignment[x86::RegisterManager::EDI];
	std::vector<pSymbol> & esivars = m_globalassignment[x86::RegisterManager::ESI];
	if (std::find( ebxvars.begin(),ebxvars.end(),p) != ebxvars.end() )
		return x86::RegisterManager::EBX;
	if (std::find( edivars.begin(),edivars.end(),p) != edivars.end() )
		return x86::RegisterManager::EDI;
	if (std::find( esivars.begin(),esivars.end(),p) != esivars.end() )
		return x86::RegisterManager::ESI;

	return x86::RegisterManager::FAILED;
}


pSymbol x86::RegisterManager::useRegister(const x86::RegisterManager::Register reg,const pSymbol p)
{
	pSymbol conflict = NULL;
	std::map<RegisterManager::Register,pSymbol>::iterator iter = m_registermap.find(reg);
	if (iter!=  m_registermap.end())
	{
		conflict = (p == m_registermap[reg] ?  NULL : iter->second);

	}

	/*std::vector<pSymbol> & ebxvars = m_globalassignment[x86::RegisterManager::EBX];
	std::vector<pSymbol> & edivars = m_globalassignment[x86::RegisterManager::EDI];
	std::vector<pSymbol> & esivars = m_globalassignment[x86::RegisterManager::ESI];
	

	switch( reg )
	{
	case x86::RegisterManager::EBX:
		assert(ebxvars.size());
		return ebxvars[0];
		break;
	case x86::RegisterManager::EDI:
		assert(edivars.size());
		return edivars[0];
		break;
	case x86::RegisterManager::ESI:
		assert(esivars.size());
		return esivars[0];
		break;
	default:*/
		m_registermap[reg] = p;
	//}
	
	return conflict;
}



ConflictPair x86::RegisterManager::grabRegister(const pSymbol p,x86::RegisterManager::Register reg)
{
	if(	p &&
		(reg == x86::RegisterManager::FAILED
		|| reg == x86::RegisterManager::EBX
		|| reg == x86::RegisterManager::EDI
		|| reg == x86::RegisterManager::ESI
		)
	)
	{
		
		if ( m_registermap[x86::RegisterManager::EAX] == NULL)
			reg =  x86::RegisterManager::EAX;
		else if ( m_registermap[x86::RegisterManager::ECX] == NULL )
			reg = x86::RegisterManager::ECX;
		else if ( m_registermap[x86::RegisterManager::EDX] == NULL )
			reg = x86::RegisterManager::EDX;
		else
			reg = getUseless();


		if (reg == x86::RegisterManager::FAILED)
			reg =  x86::RegisterManager::EAX;
	}
		
	ConflictPair conflict(reg,useRegister(reg,p));
	return conflict;
}

void x86::RegisterManager::resetEnv(lookup::Env *p)
{
	
	m_env = p;
	m_registermap.clear();
	m_usedvar.clear();
	
	m_changedGlobalReg.clear();
	m_globalassignment[x86::RegisterManager::EBX] = std::vector<pSymbol>();
	m_globalassignment[x86::RegisterManager::EDI] = std::vector<pSymbol>();
	m_globalassignment[x86::RegisterManager::ESI] = std::vector<pSymbol>();
	
	assignGlobalRegister();
	if (m_globalassignment[x86::RegisterManager::EBX].size() > 0)
		m_changedGlobalReg.push_back(x86::RegisterManager::EBX);
	if (m_globalassignment[x86::RegisterManager::EDI].size() > 0)
		m_changedGlobalReg.push_back(x86::RegisterManager::EDI);
	if (m_globalassignment[x86::RegisterManager::ESI].size() > 0)
		m_changedGlobalReg.push_back(x86::RegisterManager::ESI);

}

void x86::RegisterManager::accessMemory(lookup::Variable * pvar)
{
	std::list<lookup::Variable*>::const_iterator iter =  m_usedvar.begin();

	while ( iter != m_usedvar.end())
	{
		if (*iter == pvar)
			return;
		iter++;
	}

	m_usedvar.push_back(pvar);
}

namespace
{
	std::string getPtrType(lookup::ValueType type)
	{
		
		switch( type )
		{
		case lookup::Char:
			return "DWORD PTR ";
		case lookup::Int:
		case lookup::Float:
			return "DWORD PTR ";
		default:
			return "";
		}

			
	}

}

bool x86::RegisterManager::IsImm(const pSymbol p)const 
{
	return (m_env->findSymbol(p->getName() ) == NULL) 
		&& ( !lookup::Env::IsGloablConst(p) );
}

std::string x86::RegisterManager::putVar(pSymbol p,bool allowregister)
{	
	assert(p);
	lookup::ValueType type = p->getType();


	std::string ans = p->toString();

	if ( m_env->findSymbol( p->getName() ) )
		ans = getPtrType(type) + "_"+p->getName();


	if ( m_env->findSymbolOnLevel( p->getName() ) )
	{
		if ( m_env->findVariableOrParamOnLevel(p->getName()) )
		{	
			ans = getPtrType(type) +  "_"+p->getName()+ "@" + m_env->getFunction()->getName() +"[ebp]";
		}
		else if (m_env->findConstOnLevel(p->getName() ))
		{
			ans = getPtrType(type) + "_" + p->getName() + "@" + m_env->getFunction()->getName();
		}
		else
			assert(0);
	}	
	else if (lookup::Env::IsGloablConst(p))
	{
		ans =getPtrType(type) + "_"+p->getName();
	}


 	x86::RegisterManager::Register reg = getRegister(p);

	if ( allowregister )
		allowregister = reg != x86::RegisterManager::FAILED;
	if(allowregister)
		//char is also longbyte
		ans = toString(reg);//. type == lookup::Char);



	//使用局部变量
	lookup::Variable* var = m_env->findVariableOnLevel(p->toString());
	if ( var != NULL && !allowregister)
	{
		accessMemory(var);
	}


	return ans;
}

void x86::RegisterManager::assignGlobalRegister()
{
	optimization::AliveVarInfo* info = m_env->getAliveVarInfo();
	optimization::ConflictGraph* conflictgraph = m_env->getConflictGraph();
	
	if (!info || !conflictgraph || conflictgraph->size() == 0)
		return;

	optimization::ConflictGraph backup = *conflictgraph;

	std::stack<pSymbol> assignstack;
	while ( true )
	{
		if (conflictgraph->size() <= 1 )
			break;

		pSymbol freenode;
		while ( 
			( freenode = conflictgraph->findFreeNode(3)) 
			&& conflictgraph->size() > 1 
			)
		{
			assignstack.push(freenode);
			conflictgraph->removeSymbol(freenode);
		}

		if (conflictgraph->size() <= 1 )
			break;
		
		//引用计数的启发方法
		conflictgraph->removeSymbol (conflictgraph->getfirst(info));
	}

	assignstack.push(conflictgraph->getfirst(info));

	while (!assignstack.empty())
	{
		std::list<pSymbol> conflictsymbols = backup.getConflictSymbols(assignstack.top());

		x86::RegisterManager::Register reg = getNonConflictReg(conflictsymbols);
		m_globalassignment[reg].push_back(assignstack.top());
		assignstack.pop();
	}

}

x86::RegisterManager::Register x86::RegisterManager::getNonConflictReg(std::list<pSymbol> symbols)
{
	typedef std::vector<pSymbol> symbolvector;
	typedef std::map<x86::RegisterManager::Register,symbolvector> RegMap;
	RegMap::iterator iter = m_globalassignment.begin();
	while ( iter != m_globalassignment.end() )
	{
		std::list<pSymbol>::iterator iter2 = symbols.begin();
		bool conflict = false;
		while (iter2 != symbols.end())
		{
			//冲突
			if (std::find( iter->second.begin(),iter->second.end(),*iter2) != iter->second.end() )
			{
				conflict = true;
				break;
			}
			iter2++;
		}
		if (!conflict)
			return iter->first;

		iter++;
	}

	assert(0);
	return x86::RegisterManager::FAILED;
}
