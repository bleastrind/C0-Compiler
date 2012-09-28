#include "basehandler.h"
#include "../lookup/env.h"
#include "../lookup/symbol.h"
#include "../util.h"
#include "../optimization/alivevarinfo.h"
#include <sstream>
#include <string>
#include <cassert>
#include <iostream>
#define NOT_IMPLY 0
//#define DEBUG
using namespace x86;

x86::BaseHandler::BaseHandler( std::ostream &os,lookup::Env* env,bool changeLine):m_ostream(os),m_registerManager(env,os),m_changeLine(changeLine)
{
	m_opstringMap.insert(std::pair<tuple::Tuple::OpType,std::string>(tuple::Tuple::JMP,"JMP"));
	m_opstringMap.insert(std::pair<tuple::Tuple::OpType,std::string>(tuple::Tuple::JE,"JE"));
	m_opstringMap.insert(std::pair<tuple::Tuple::OpType,std::string>(tuple::Tuple::JNE,"JNE"));
	m_opstringMap.insert(std::pair<tuple::Tuple::OpType,std::string>(tuple::Tuple::JG,"JG"));
	m_opstringMap.insert(std::pair<tuple::Tuple::OpType,std::string>(tuple::Tuple::JGE,"JGE"));
	m_opstringMap.insert(std::pair<tuple::Tuple::OpType,std::string>(tuple::Tuple::JL,"JL"));
	m_opstringMap.insert(std::pair<tuple::Tuple::OpType,std::string>(tuple::Tuple::JLE,"JLE"));
	//m_opstringMap.insert(std::pair<tuple::Tuple::OpType,std::string>(tuple::Tuple::PARAM,"PARAM"));
	//m_opstringMap.insert(std::pair<tuple::Tuple::OpType,std::string>(tuple::Tuple::CALL,"CALL"));
	//m_opstringMap.insert(std::pair<tuple::Tuple::OpType,std::string>(tuple::Tuple::RET,"RET"));
	//m_opstringMap.insert(std::pair<tuple::Tuple::OpType,std::string>(tuple::Tuple::ASSIGN,"mov"));
	//m_opstringMap.insert(std::pair<tuple::Tuple::OpType,std::string>(tuple::Tuple::LABEL,":"));
	//m_opstringMap.insert(std::pair<tuple::Tuple::OpType,std::string>(tuple::Tuple::FUNC,"PROC "));
	//m_opstringMap.insert(std::pair<tuple::Tuple::OpType,std::string>(tuple::Tuple::ENDP,"ENDP"));
	//m_opstringMap.insert(std::pair<tuple::Tuple::OpType,std::string>(tuple::Tuple::MAINEND,"END _main"));
	//m_opstringMap.insert(std::pair<tuple::Tuple::OpType,std::string>(tuple::Tuple::SCANF,"CALL _scanf"));
	//m_opstringMap.insert(std::pair<tuple::Tuple::OpType,std::string>(tuple::Tuple::PRINTF,"CALL _printf"));

	m_output.push_back("; #########################################################################");
	m_output.push_back("		.386");
	m_output.push_back("		.387");
	m_output.push_back("		.model flat, stdcall");
	m_output.push_back("		option casemap :none   ; case sensitive");
	m_output.push_back("; #########################################################################");
    m_output.push_back("  c_msvcrt typedef PROTO C :VARARG");
    m_output.push_back("  externdef _imp__printf:PTR c_msvcrt");
    m_output.push_back("  _printf equ <_imp__printf>");
    m_output.push_back("  externdef _imp__scanf:PTR c_msvcrt");
    m_output.push_back("  _scanf equ <_imp__scanf>");
	m_output.push_back("  externdef _imp___ftol:PTR c_msvcrt");
	m_output.push_back("  @ftol equ <_imp___ftol>");
	m_output.push_back("		includelib msvcrt.lib");
	m_output.push_back("; #########################################################################");

	m_data.push_back(".data");

	m_code.push_back(".code");

}



void x86::BaseHandler::recieveTuple(lookup::Env * penv, pTuple ptuple)
{
#ifdef DEBUG
	std::cout<<ptuple->toString()<<std::endl;
#endif
	//设置环境
	m_registerManager.setTuple(ptuple);

	m_function.push_back("    ;" + ptuple->toString());

	if ( ptuple->getOp() != tuple::Tuple::FUNC )
		assert( m_env == penv);
	m_env = penv;
	m_tuple = ptuple;
	switch( ptuple->getOp() )
	{

	case tuple::Tuple::JMP:
		emitJUMP();
		break;
	case tuple::Tuple::JE:
	case tuple::Tuple::JNE:
	case tuple::Tuple::JG:
	case tuple::Tuple::JGE:
	case tuple::Tuple::JL:
	case tuple::Tuple::JLE:
		emitIFJUMP();
		break;
	case tuple::Tuple::PARAM:
		emitPARAM();
		break;
	case tuple::Tuple::CALL:
		emitCALL();
		break;
	case tuple::Tuple::RET:
		emitRET();
		break;
	case tuple::Tuple::ASSIGN:
		emitASSIGN();
		break;
	case tuple::Tuple::ADD:
	case tuple::Tuple::SUB:
	case tuple::Tuple::MUL:
		emitADDS();
		break;

	case tuple::Tuple::DIV:
		emitMULS();
		break;
	case tuple::Tuple::LABEL:
		emitLABEL();
		break;
	case tuple::Tuple::FUNC:
		emitFUNC();
		break;
	case tuple::Tuple::ENDP:
		emitENDP();
		break;
	case tuple::Tuple::MAINEND:
		emitMAINEND();
		break;
	case tuple::Tuple::PRINTF:
		emitPRINTF();
		break;
	case tuple::Tuple::SCANF:
		emitSCANF();
		break;

	default:
	#define NEW_TUPLE 0
		assert(NEW_TUPLE);
	}
}


lookup::ValueType x86::BaseHandler::unifyArgsTo(pSymbol &p1,pSymbol &p2)
{

	
	if (p2->getType() == p1->getType())
		return  p1->getType();

	pSymbol &larger = p2->getType() > p1->getType() ? p2 : p1;
	pSymbol &smaller = p2->getType() > p1->getType() ? p1 : p2;
	
	lookup::ValueType max = larger->getType() ;

	switch( max )
	{
	case lookup::Float:
		{
			lookup::Variable* var = m_env->createTemp(max);
			m_env->addVariable(var);
			MOV(var,smaller);
			smaller = var;
		}
		break;
	case lookup::Int:
	case lookup::Char:
		//assert(NOT_IMPLY);
		break;
	default:
		assert(0);
	}

	return max;
}

void x86::BaseHandler::saveVarInTempRegister()
{
	saveConflict( m_registerManager.grabRegister(NULL,x86::RegisterManager::EAX) );
	saveConflict( m_registerManager.grabRegister(NULL,x86::RegisterManager::EDX) );
	saveConflict( m_registerManager.grabRegister(NULL,x86::RegisterManager::ECX) );
	
}
//保存原寄存器内容并把新内容填到寄存器
void x86::BaseHandler::occupyRegister(pSymbol p,x86::RegisterManager::Register reg )
{
		std::string s1 = m_registerManager.putVar(p);

		ConflictPair conflict = m_registerManager.grabRegister(p,reg);
		saveConflict(conflict);
		m_function.push_back(
			"mov " + 
			m_registerManager.toString(conflict.first) + 
			","+
			s1
			);
}

bool x86::BaseHandler::WillAlive(pSymbol p,bool includecurrent)
{
	
	optimization::AliveVarInfo* info = m_env->getAliveVarInfo();
	if (info == NULL)
		return true;
	else if ( m_env->findVariableOnLevel(p->getName()) )
		return includecurrent ? info->IsAlive(m_tuple,p) :info->WillAlive(m_tuple,p);
	else
		return true;
	
}
void x86::BaseHandler::saveConflict(ConflictPair pair,bool includecurrent)
{
	if ( pair.second != NULL && !m_registerManager.IsImm(pair.second) )
	{
		assert(pair.first != x86::RegisterManager::FAILED);
		
		if ( WillAlive( pair.second ,includecurrent) )
			m_function.push_back(
				"mov " + 
				m_registerManager.putVar( pair.second ) + 
				"," + 
				x86::RegisterManager::toString( pair.first)
				);
			//#define WONT_CONFLICT_WHEN_NO_OPTIMISTIC 0
			//assert(WONT_CONFLICT_WHEN_NO_OPTIMISTIC);
	}
}

void x86::BaseHandler::flushCode()
{
	m_code.splice( m_code.end(), m_function);
}

void x86::BaseHandler::flushAll()
{
	std::list<std::string>::iterator iter = m_output.begin();
	while(iter != m_output.end() )
	{
		m_ostream<<*iter<<std::endl;
		iter++;
	}
}


void x86::BaseHandler::floatCalcs(pSymbol p1,pSymbol p2,pSymbol p3)
{
	saveConflict( m_registerManager.grabRegister(NULL,m_registerManager.getRegister(p1) ),true );
	saveConflict( m_registerManager.grabRegister(NULL,m_registerManager.getRegister(p2) ),true );
	saveConflict( m_registerManager.grabRegister(NULL,m_registerManager.getRegister(p3) ),true );


	m_function.push_back( "fld "+ m_registerManager.putVar(p1) );

	std::string opstr ;
	switch(m_tuple->getOp())
	{
	case tuple::Tuple::ADD:
		opstr = "fadd ";
		break;
	case tuple::Tuple::SUB:
		opstr = "fsub ";
		break;
	case tuple::Tuple::MUL:
		opstr = "fmul ";
		break;
	case tuple::Tuple::DIV:
		opstr = "fdiv ";
		break;
	default:
		assert(0);
	}

	m_function.push_back( opstr + m_registerManager.putVar(p2) );

	m_function.push_back( "fstp "+ m_registerManager.putVar(p3) );
}

void x86::BaseHandler::intAdds(pSymbol p1,pSymbol p2,pSymbol p3)
{

	std::string opstr;
	switch(m_tuple->getOp())
	{
	case tuple::Tuple::ADD:
		opstr = "add ";
		break;
	case tuple::Tuple::SUB:
		opstr = "sub ";
		break;
	case tuple::Tuple::MUL:
		opstr = "imul ";
		break;
	default:
		assert(0);
	}

	std::string s1 = m_registerManager.putVar(p1);
	//std::string s2 = m_registerManager.putVar(p2);
	if ( m_registerManager.getRegister(p3) == x86::RegisterManager::FAILED)
	{
		optimization::AliveVarInfo* info = m_env->getAliveVarInfo();
		
		//尽量使p1直接就是p3
		ConflictPair conflict = m_registerManager.grabRegister( p3 , m_registerManager.getRegister(p1) );
		

		//若与自己冲突,可能无需mov（在p1已经无用且已是p3时）
		if (conflict.second == p1)
		{
			//仍要使用，需保存
			if(info== NULL || info->WillAlive(m_tuple, conflict.second) )
				m_function.push_back(
					"mov "+
					m_registerManager.putVar(p1)+
					","+
					x86::RegisterManager::toString(conflict.first)
					);
		}
		else
		{
			if (conflict.second == p2)
			{
				m_function.push_back(
					"mov " +
					m_registerManager.putVar(p2)+
					","+
					x86::RegisterManager::toString(conflict.first)
					);
			}
			else
				saveConflict( conflict );

			
			m_function.push_back(
				"mov " +
				m_registerManager.putVar(p3) +
				"," +
				m_registerManager.putVar(p1)
				);
		}

		////若与p2冲突且p2不再需要使用，需用其原来位置的值
		//if (conflict.second == p2
		//	&& 
		//	(info== NULL || info->WillAlive(m_tuple, conflict.second) )
		//	)
		//{
		//	s2 = s2;
		//}else
		//	s2 = m_registerManager.putVar(p2);//否则需用新值
	}
	else
	{
		m_function.push_back(
			"mov " +
			m_registerManager.putVar(p3) +
			"," +
			s1
			);
	}



	//做运算
	m_function.push_back(
		opstr +
		m_registerManager.putVar(p3) +
		"," +
		m_registerManager.putVar(p2)
		);




}


void x86::BaseHandler::intDiv(pSymbol p1,pSymbol p2,pSymbol p3)
{
	if ( m_registerManager.IsImm(p2) && m_registerManager.getRegister(p2) == x86::RegisterManager::FAILED )//立即数
	{
		lookup::Variable* var = lookup::Env::createTemp( p2->getType() );
		m_env->addVariable(var);

		MOV(var,p2);
		p2 = var;
	}			

	std::string s1 = m_registerManager.putVar(p1);


	ConflictPair eaxconflict =  m_registerManager.grabRegister(NULL,x86::RegisterManager::EAX);
	ConflictPair edxconflict = m_registerManager.grabRegister(NULL,x86::RegisterManager::EDX);

	//if p2 conflicted,need save it
	if (eaxconflict.second == p1)
	{
		saveConflict( eaxconflict);
		m_registerManager.useRegister(x86::RegisterManager::EAX,p1);
	}
	else if (eaxconflict.second == p2)
	{
		m_function.push_back("mov "+ m_registerManager.putVar(p2) +",eax");
	}
	else 
		saveConflict( eaxconflict);


	if (edxconflict.second == p2)
	{
		m_function.push_back("mov "+ m_registerManager.putVar(p2) +",edx");
	}
	else 
		saveConflict( edxconflict);

	//begin calc
	m_function.push_back("mov eax,"+s1);

	m_function.push_back("cdq");

	m_function.push_back(
		"idiv " +
		m_registerManager.putVar(p2)
		);

	m_registerManager.useRegister(x86::RegisterManager::EAX,p3);

}

void x86::BaseHandler::emitADDS()
{
	tuple::Tuple::OpType op = m_tuple->getOp();
	assert(op == tuple::Tuple::ADD || op == tuple::Tuple::SUB || op == tuple::Tuple::MUL);
	pSymbol p1 = m_tuple->getSrc1();
	pSymbol p2 = m_tuple->getSrc2();
	pSymbol preal = m_tuple->getTarget();
	pSymbol p3 = preal;

	lookup::ValueType max = unifyArgsTo(p2,p1);
	
	bool needFinalTransfer = false;
	if (p3->getType() != max)
	{
		needFinalTransfer = true;
		lookup::Variable* newvar = lookup::Env::createTemp( max );
		m_env->addVariable(newvar);
		p3 = newvar;
	}

	switch( max )
	{
	case lookup::Float:
		floatCalcs(p1,p2,p3);
		break;
	case lookup::Int:
	case lookup::Char:
		intAdds(p1,p2,p3);

		break;
	default:
		assert(0);
	}

	if ( needFinalTransfer  )
	{
		assert(p3 != preal);
		MOV(preal,p3);
	}

	if ( preal->getType() == lookup::Char )
	{
		occupyRegister( preal, x86::RegisterManager::EAX );
		m_function.push_back("movsx eax,al");
	}
}
void x86::BaseHandler::emitMULS()
{
	tuple::Tuple::OpType op = m_tuple->getOp();
	assert(op == tuple::Tuple::MUL || op == tuple::Tuple::DIV);
	pSymbol p1 = m_tuple->getSrc1();
	pSymbol p2 = m_tuple->getSrc2();
	pSymbol preal = m_tuple->getTarget();
	pSymbol p3 = preal;

	lookup::ValueType max = unifyArgsTo(p2,p1);
	
	bool needFinalTransfer = false;
	if (p3->getType() != max)
	{
		needFinalTransfer = true;
		lookup::Variable* newvar = lookup::Env::createTemp( max );
		m_env->addVariable(newvar);
		p3 = newvar;
	}

	switch( max )
	{
	case lookup::Float:
		floatCalcs(p1,p2,p3);
		break;
	case lookup::Int:
	case lookup::Char:
		if ( op == tuple::Tuple::DIV )
			intDiv(p1,p2,p3);
		else
			intAdds(p1,p2,p3);
		
		break;
	default:
		assert(0);
	}
	if ( needFinalTransfer )
	{
		assert(p3 != preal);
		MOV(preal,p3);
	}
	
	if ( preal->getType() == lookup::Char )
	{
		occupyRegister( preal, x86::RegisterManager::EAX );
		m_function.push_back("movsx eax,al");
	}
}

typedef void (BaseHandler::* MovFunc)(pSymbol,pSymbol);
MovFunc x86::BaseHandler::convertTable[4][4]  ={
	{&BaseHandler::movError	,&BaseHandler::movError			,&BaseHandler::movError		,&BaseHandler::movError			},
	{&BaseHandler::movError	,&BaseHandler::movCharToChar	,&BaseHandler::movIntToChar	,&BaseHandler::movFloatToChar	},
	{&BaseHandler::movError	,&BaseHandler::movCharToInt		,&BaseHandler::movIntToInt	,&BaseHandler::movFloatToInt	},
	{&BaseHandler::movError	,&BaseHandler::movCharToFloat	,&BaseHandler::movIntToFloat,&BaseHandler::movFloatToFloat	},
};

void x86::BaseHandler::MOV(pSymbol ptarget,pSymbol psrc)
{
	void (BaseHandler::*mov)(pSymbol,pSymbol) = convertTable[ptarget->getType()][psrc->getType()];
	(this->*mov)(psrc,ptarget);
}
void x86::BaseHandler::movError(pSymbol p1,pSymbol p2)
{
	assert(0);
}
void x86::BaseHandler::movCharToChar(pSymbol p1,pSymbol p2)
{
	movIntToInt(p1,p2);
}
//p2 必在内存中
void x86::BaseHandler::movIntToInt(pSymbol p1,pSymbol p2)
{
	if (p1 == p2)
		return;
	//相等必然为FAILED
	if ( m_registerManager.getRegister(p1) == m_registerManager.getRegister(p2)
		&& !m_registerManager.IsImm(p1) 
		)
	{
		assert( m_registerManager.getRegister(p2) == x86::RegisterManager::FAILED);
		occupyRegister(p1);
	}
	
	m_function.push_back(
		"mov " +
		m_registerManager.putVar(p2) +
		"," +
		m_registerManager.putVar(p1)
		);
}
void x86::BaseHandler::movFloatToFloat(pSymbol p1,pSymbol p2)
{
	if (p1 == p2)
		return;
	saveConflict( m_registerManager.grabRegister(NULL,m_registerManager.getRegister(p1)),true);
	saveConflict( m_registerManager.grabRegister(NULL,m_registerManager.getRegister(p2)),true);
	//相等必然为FAILED
	if ( m_registerManager.getRegister(p1) == m_registerManager.getRegister(p2))
	{			
		m_function.push_back("fld " + m_registerManager.putVar(p1));
		m_function.push_back("fstp " + m_registerManager.putVar(p2));

	}else
		assert(0);
	

}
void x86::BaseHandler::movIntToChar(pSymbol p1,pSymbol p2)
{
	movIntToInt(p1,p2);
	
	occupyRegister( p2, x86::RegisterManager::EAX);
	m_function.push_back("movsx eax,al");
}
void x86::BaseHandler::movCharToInt(pSymbol p1,pSymbol p2)
{
	movIntToInt(p1,p2);
}
void x86::BaseHandler::movIntToFloat(pSymbol p1,pSymbol p2)
{
	if ( m_registerManager.IsImm(p1) 
		|| m_registerManager.getRegister(p1)== x86::RegisterManager::EDI
		||m_registerManager.getRegister(p1)== x86::RegisterManager::ESI
		||m_registerManager.getRegister(p1)== x86::RegisterManager::EBX
		)
	{
		lookup::Variable* var = lookup::Env::createTemp( p1->getType() );
		m_env->addVariable(var);

		movIntToInt(p1,var);
		p1 = var;
	}

	saveConflict( m_registerManager.grabRegister(NULL,m_registerManager.getRegister(p1)),true);
	m_function.push_back("fild " + m_registerManager.putVar(p1));
	m_registerManager.useRegister( m_registerManager.getRegister(p2),NULL);
	m_function.push_back("fstp " + m_registerManager.putVar(p2));
}
void x86::BaseHandler::movFloatToInt(pSymbol p1,pSymbol p2)
{
	//saveConflict( m_registerManager.grabRegister(NULL,m_registerManager.getRegister(p1)),true);
	//m_function.push_back("fld " + m_registerManager.putVar(p1));
	//m_registerManager.useRegister( m_registerManager.getRegister(p2),NULL);
	//m_function.push_back("fistp " + m_registerManager.putVar(p2));
	//
	saveVarInTempRegister();
	assert( m_registerManager.getRegister(p1) == x86::RegisterManager::FAILED);
	m_function.push_back("fld " + m_registerManager.putVar(p1));
	m_function.push_back("call @ftol");
	
	m_registerManager.useRegister(  x86::RegisterManager::EAX,p2 );
}
void x86::BaseHandler::movCharToFloat(pSymbol p1,pSymbol p2)
{
	lookup::Variable* ptemp = m_env->createTemp(lookup::Int);
	m_env->addVariable(ptemp);

	movCharToInt(p1,ptemp);
	movIntToFloat(ptemp,p2);
}
void x86::BaseHandler::movFloatToChar(pSymbol p1,pSymbol p2)
{
	movFloatToInt(p1,p2);
}

void x86::BaseHandler::emitASSIGN()
{
	pSymbol p1 = m_tuple->getSrc1();
	pSymbol p2 = m_tuple->getTarget();


	MOV(p2,p1);


}
void x86::BaseHandler::emitCALL()
{
	saveVarInTempRegister();

	pSymbol p1 = m_tuple->getSrc1();
	m_function.push_back("CALL " +p1->toString());

	pSymbol ret = m_tuple->getTarget();
	if ( ret != NULL )
	{
		m_function.push_back(
			"mov "+
			m_registerManager.putVar(ret)+
			",EAX"
			);
	}
	m_pushedparam.clear();
}



void x86::BaseHandler::emitENDP()
{
	functionComplete();
	//m_registerManager.resetEnv(m_env->getParent());
}
void x86::BaseHandler::emitFUNC()
{
	m_registerManager.resetEnv(m_env);
	
	std::list<lookup::Variable*> params = m_env->getParams();

	std::list<lookup::Variable*>::iterator iter = params.begin();
	while(iter != params.end())
	{
		x86::RegisterManager::Register reg = m_registerManager.getRegister(*iter);
		
		if ( reg != x86::RegisterManager::FAILED )
			m_function.push_back(
			"mov "+
			x86::RegisterManager::toString(reg)+
			","+
			m_registerManager.putVar(*iter,false)
			);
		iter++;
	}
}
void x86::BaseHandler::putLocalFloat()
{
	std::map<std::string ,lookup::Const*> consts = m_env->getLocalConsts();
	std::map<std::string ,lookup::Const*>::iterator iter = consts.begin();
	while (iter != consts.end() )
	{
		switch (iter->second->getType())
		{
			
		case lookup::Float:
			m_data.push_back("_" +iter->second->getName()+ "@" +m_env->getFunction()->getName() +" DD "+ iter->second->toString());
			break;
		case lookup::Int:
		case lookup::Char:
			m_data.push_back("_" +iter->second->getName()+ "@" +m_env->getFunction()->getName() +" DD "+ iter->second->toString());
			break;
		default:
			assert(0);
		}
		iter++;
	}
}

void x86::BaseHandler::floatJump( tuple::Tuple::OpType op)
{
	switch(op)
	{
	case tuple::Tuple::JE:

		m_function.push_back("test	ah, 68					; 00000044H");
		m_function.push_back("jnp " + m_registerManager.putVar(m_tuple->getTarget()) );
		break;
	case tuple::Tuple::JG:
		m_function.push_back("test	ah, 5");
		m_function.push_back("jnp " + m_registerManager.putVar(m_tuple->getTarget()) );
		break;
	case tuple::Tuple::JGE:
		m_function.push_back("test	ah, 65					; 00000041H");
		m_function.push_back("jnp " + m_registerManager.putVar(m_tuple->getTarget()) );
		break;
	case tuple::Tuple::JL:
		m_function.push_back("test	ah, 65					; 00000041H");
		m_function.push_back("je " + m_registerManager.putVar(m_tuple->getTarget()) );
		break;
	case tuple::Tuple::JLE:
		m_function.push_back("test	ah, 1");
		m_function.push_back("je " + m_registerManager.putVar(m_tuple->getTarget()) );
		break;
	case tuple::Tuple::JNE:
		m_function.push_back("test	ah, 68");
		m_function.push_back("je " + m_registerManager.putVar(m_tuple->getTarget()) );
		break;
	default:
		assert(0);
	}
}

void x86::BaseHandler::emitIFJUMP()
{

	
	

	pSymbol p1 = m_tuple->getSrc1();
	pSymbol p2 = m_tuple->getSrc2();
	pSymbol p3 = m_tuple->getTarget();

	lookup::ValueType max = unifyArgsTo(p1,p2);
	switch(max)
	{
	case lookup::Float:
		{
			saveConflict( m_registerManager.grabRegister( NULL, m_registerManager.getRegister(p1)));
			saveConflict( m_registerManager.grabRegister( NULL, m_registerManager.getRegister(p2)));
			saveConflict( m_registerManager.grabRegister( NULL, x86::RegisterManager::EAX ));


			//跳之前保存
			saveVarInTempRegister();

			m_function.push_back(
				"fld " +
				m_registerManager.putVar(p1)
				);
			m_function.push_back(
				"fld " +
				m_registerManager.putVar(p2)
				);

			m_function.push_back("fcompp");
			m_function.push_back("fnstsw	ax");
			floatJump( m_tuple->getOp() );
		}
		break;
	case lookup::Int:
	case lookup::Char:
		{
			if ( m_registerManager.getRegister(p1) == x86::RegisterManager::FAILED)
			{
				occupyRegister( p1 );
			}


			m_function.push_back(
				"cmp " +
				m_registerManager.putVar(p1) +
				"," +
				m_registerManager.putVar(p2)
				);
			
			//跳之前保存
			saveVarInTempRegister();

			m_function.push_back(
				m_opstringMap[m_tuple->getOp()] +
				" " + m_registerManager.putVar( p3 )
				);
		}
		break;
	default:
		assert(0);
	}


}
void x86::BaseHandler::emitJUMP()
{
	saveVarInTempRegister();
	m_function.push_back( "jmp " +m_tuple->getSrc1()->toString());
	
}
void x86::BaseHandler::emitLABEL()
{
	saveVarInTempRegister();
	m_function.push_back(m_tuple->getSrc1()->toString()+":");
}
void x86::BaseHandler::emitMAINEND()
{

	functionComplete();

	std::list<std::string> strings = getStringDecl();
	std::list<std::string> floats = getConstFloatDecl();
	std::list<std::string> globalvars = getGlobalVarDecl();

	m_data.splice(m_data.end(), strings);
	m_data.splice(m_data.end(), floats);
	m_data.splice(m_data.end(), globalvars);

	m_output.splice(m_output.end(),m_data);
	m_output.splice(m_output.end(),m_code);
	m_output.push_back("END _main");

	flushAll();
}

void x86::BaseHandler::emitPARAM()
{
	m_pushedparam.push_back( m_tuple->getSrc1()->getType() );

	//must push a dword!!!char relative modify
	m_function.push_back( "push " + m_registerManager.putVar( m_tuple->getSrc1()) );
}

void x86::BaseHandler::emitRET()
{
	
	
	pSymbol retvalue = m_tuple->getSrc2();
	if (retvalue != NULL)
	{

		if ( m_registerManager.getRegister( retvalue ) != RegisterManager::EAX )
		{
			lookup::Variable* retvar = lookup::Env::createTemp( m_env->getFunction()->getType() );
			m_env->addVariable(retvar);

			//return 无需保存EAX的值，不用处理冲突			
			MOV(retvar,retvalue);

			m_function.push_back("mov eax,"+m_registerManager.putVar(retvar));
		}
	}

	//可能要保存全局
	saveVarInTempRegister();
	
	m_function.push_back("jmp " + m_env->getFunction()->getName() + "END" );

	
}

namespace
{
	int getParamsSize(std::list<lookup::ValueType>& params)
	{
		int size = 0;
		std::list<lookup::ValueType>::iterator iter = params.begin();
		while ( iter != params.end() )
		{
			switch (*iter)
			{
			case lookup::Char:
			case lookup::Int:
				size += 4;
				break;
			case lookup::Float:
				size += 8;
				break;
			default:
				assert(0);
			}
			iter++;
		}

		return size;

	}

}

void x86::BaseHandler::emitSCANF()
{

	saveVarInTempRegister();


	pSymbol str = m_tuple->getSrc1();
	pSymbol var = m_tuple->getTarget();
	
	assert(str&&var);
	assert( !m_registerManager.IsImm(var));

	m_function.push_back("sub esp,4");
	m_function.push_back("push esp");
	m_function.push_back("push offset "+ m_registerManager.putVar(str));
	m_function.push_back("call _scanf");
	m_function.push_back("add esp,8" );
	m_function.push_back("pop "+m_registerManager.putVar(var));

}
void x86::BaseHandler::emitPRINTF()
{

	
	pSymbol str = m_tuple->getSrc1();

	pSymbol var = m_tuple->getSrc2();
	std::string offset="4";

	if (var  && var->getType()==lookup::Float)
	{
		//float can't have a register
		assert(m_registerManager.getRegister(var) == x86::RegisterManager::FAILED);
		m_function.push_back("sub esp, 8");
		m_function.push_back("fld "+m_registerManager.putVar(var));
		
		m_function.push_back("fstp QWORD PTR [esp]");
		offset = "12";
	}
	else if (var )
	{
		m_function.push_back("push " + m_registerManager.putVar(var) );
		offset = "8";
	}
	assert(str);
	
	m_function.push_back("push offset " + m_registerManager.putVar(str));


	//call 之前保存全局
	saveVarInTempRegister();


	m_function.push_back("call _printf");
		
	m_function.push_back("add esp,"+ offset );

	m_pushedparam.clear();
}


void x86::BaseHandler::replaceVarDecl()
{
	std::map<std::string,std::string> vars = getVarDecl();
	std::map<std::string,std::string>::iterator striter = vars.begin();
	while(striter != vars.end())
	{
		std::list<std::string>::iterator cmdsiter = m_function.begin();
		while ( cmdsiter != m_function.end() )
		{
			util::replace_once(*cmdsiter,striter->first,striter->second);
			cmdsiter++;
		}
		striter++;
	}
}
void x86::BaseHandler::replaceParamDecl()
{
	std::map<std::string,std::string> params = getParamDecl();
	std::map<std::string,std::string>::iterator striter = params.begin();
	while(striter != params.end())
	{
		std::list<std::string>::iterator cmdsiter = m_function.begin();
		while ( cmdsiter != m_function.end() )
		{
			std::string s = *cmdsiter;
			util::replace_once(*cmdsiter,striter->first,striter->second);
			cmdsiter++;
		}
		striter++;
	}
}
std::map<std::string,std::string> x86::BaseHandler::getVarDecl()
{
	std::map<std::string,std::string> ans;
	int varoffset = 4;

	std::list<lookup::Variable*> vars = m_registerManager.getUsedVars();
	std::list<lookup::Variable*>::reverse_iterator iter 
		= vars.rbegin();
	while ( iter != vars.rend())
	{
		ans["_"+(*iter)->getName()+ "@" + m_env->getFunction()->getName()+"[ebp]"] 
			= "[ebp -" + util::ToString(varoffset)+"]";
		
		varoffset += 4;
		iter++;
	}
	return ans;
}

std::map<std::string,std::string> x86::BaseHandler::getParamDecl()
{
	std::map<std::string,std::string> ans;
	std::list<lookup::Variable*> params = m_env->getParams();
	std::list<lookup::Variable*>::iterator iter = params.begin();
	int paramoffset = 8 + m_registerManager.getChangedGlobalRegs().size()*4;
	while ( iter != params.end() )
	{
		ans["_" + (*iter)->getName()+ "@" + m_env->getFunction()->getName()+"[ebp]"]
			= "[ebp +"+util::ToString(paramoffset)+"]";

		paramoffset += 4;
		iter++;
	}
	return ans;
}

std::list<std::string> x86::BaseHandler::getRegProtect()
{
	std::list<std::string> ans;
	std::list<x86::RegisterManager::Register> regs = m_registerManager.getChangedGlobalRegs();
	std::list<x86::RegisterManager::Register>::iterator iter = regs.begin();
		

	while ( iter != regs.end() )
	{
		ans.push_back( "push "+ m_registerManager.toString(*iter) );
		iter++;
	}

	return ans;
}

std::list<std::string> x86::BaseHandler::getRegRestore()
{
	std::list<std::string> ans;
	std::list<x86::RegisterManager::Register> regs = m_registerManager.getChangedGlobalRegs();
	std::list<x86::RegisterManager::Register>::reverse_iterator iter = regs.rbegin();
		

	while ( iter != regs.rend() )
	{
		ans.push_back( "pop "+ m_registerManager.toString(*iter) );
		iter++;
	}
	return ans;
}

void x86::BaseHandler::functionComplete()
{
	saveVarInTempRegister();

	//put in to data
	putLocalFloat();

	//add end

	m_function.push_back(m_env->getFunction()->getName() + "END:");

	m_function.push_back("mov esp, ebp");
	m_function.push_back("pop ebp");

	//restore regs
	std::list<std::string> regrestore = getRegRestore();
	m_function.splice( m_function.end(), regrestore );



	m_function.push_back("ret " + util::ToString((int)m_env->getFunction()->getParamCount()*4));
	m_function.push_back(m_env->getFunction()->toString() +" ENDP");
	m_function.push_back("");

	//front reverse order
	int varssize = m_registerManager.getUsedVars().size()*4  ;
	
	m_function.push_front( "sub esp,"+ util::ToString(varssize) );

	m_function.push_front("mov ebp,esp");
	m_function.push_front("push ebp");

	//protect regs
	std::list<std::string> regprotect = getRegProtect();
	m_function.splice( m_function.begin() ,regprotect);
	
	


	m_function.push_front( m_env->getFunction()->toString() + " PROC");
	
	//replace params
	replaceParamDecl();

	//replace vars
	replaceVarDecl();
	
	

	flushCode();
}


std::list<std::string> BaseHandler::getStringDecl()
{
	std::map<std::string,lookup::String*> strings = lookup::Env::getStrings();
	std::map<std::string,lookup::String*>::iterator iter = strings.begin();

	std::list<std::string> ans;
	while ( iter != strings.end() )
	{
		ans.push_back("_"+iter->second->getName()+" DB '"+ iter->second->toString()+"'," +
			(m_changeLine ? "10,0" : "0"));
		iter++;
	}

	return ans;
}

std::list<std::string> BaseHandler::getConstFloatDecl()
{
	std::map<float,lookup::Const*> floats = lookup::Env::getFloatConsts();
	std::map<float,lookup::Const*>::iterator iter = floats.begin();

	std::list<std::string> ans;
	while ( iter != floats.end() )
	{
		ans.push_back("_" +iter->second->getName()+" DD "+ iter->second->toString());
		iter++;
	}

	return ans;
}
std::list<std::string> BaseHandler::getGlobalVarDecl()
{
	lookup::Env* topenv = m_env;
	assert(m_env);
	while ( !topenv->isTop() )
		topenv = m_env->getParent();


	std::list<std::string> ans;
	std::map<std::string,lookup::Variable *> vars = topenv->getVariables();
	std::map<std::string,lookup::Variable *>::iterator iter = vars.begin();
	while ( iter != vars.end() )
	{
		lookup::Variable* var = iter->second;
		std::string typestr;
		switch(var->getType())
		{
		case lookup::Char:
			//typestr = " DB ";
			//break;
		case lookup::Int:
		case lookup::Float:
			typestr = " DD ";
			break;
		default:
			assert(0);
		}
		ans.push_back( "_" + var->getName()+ typestr + " ?" );
		iter++;
	}

	std::map<std::string,lookup::Const*> consts = topenv->getLocalConsts();
	std::map<std::string,lookup::Const *>::iterator iter2 = consts.begin();
	while ( iter2 != consts.end() )
	{
		lookup::Const* c = iter2->second;
		std::string typestr;
		switch(c->getType())
		{
		case lookup::Char:
			//typestr = " DB ";
			//break;
		case lookup::Int:
		case lookup::Float:
			typestr = " DD ";
			break;
		default:
			assert(0);
		}
		ans.push_back( "_" + c->getName()+ typestr + " " + c->toString() );
		iter2++;
	}

	return ans;

}
