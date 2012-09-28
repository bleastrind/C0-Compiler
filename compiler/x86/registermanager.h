#ifndef __BLEASTRIND_REGISTERMANAGER
#define __BLEASTRIND_REGISTERMANAGER
#include <map>
#include <list>
#include <iostream>
#include "../lookup/env.h"
#include "../util.h"
#include "../lookup/symbolfwd.h"
namespace x86
{
	class RegisterManager:public UnChangeable
	{
	public:

		enum Register
		{
			FAILED,

			EAX,
			EDX,
			ECX,

			EBX,
			ESI,
			EDI,
			
		};
		static std::string toString(const RegisterManager::Register,bool lowbyte = false);

		RegisterManager(lookup::Env* env,std::ostream &ostream):m_env(env),m_ostream(ostream)
		{
		}
		void resetEnv(lookup::Env* p);
		void setTuple(tuple::Tuple* p){ m_tuple = p; }

		bool IsImm(const pSymbol) const;


		void clearRegisterUsage() { m_registermap.clear(); }

		RegisterManager::Register getRegister(const pSymbol);
		RegisterManager::Register getUseless();

		//�ı�Ĵ���ʹ��״�������س�ͻ�ı���
		pSymbol useRegister(const RegisterManager::Register,const pSymbol);

		typedef x86::RegisterManager::Register Register;
		typedef std::pair<Register,pSymbol> ConflictPair;
		ConflictPair grabRegister(const pSymbol,RegisterManager::Register reg = RegisterManager::FAILED);

		std::list<Register> getChangedGlobalRegs() { return m_changedGlobalReg; }
		std::list<lookup::Variable*> getUsedVars() { return m_usedvar; }

		//������Ҫ�����ڴ棬�����һ�η��ʼ���UsedVars
		void accessMemory(lookup::Variable*);


		//���ݼĴ�����״������ʹ�ú�����ʽ
		std::string putVar(pSymbol,bool allowregister = true);
	private:
		lookup::Env* m_env;
		tuple::Tuple* m_tuple;
		std::ostream &m_ostream;

		std::list<Register> m_changedGlobalReg;
		std::list<lookup::Variable*> m_usedvar;
		std::map<Register,pSymbol> m_registermap;

		typedef std::vector<pSymbol> symbolvector;
		std::map<Register,symbolvector> m_globalassignment;
		void assignGlobalRegister();

		Register getNonConflictReg(std::list<pSymbol>);
	};
}//namespace
typedef std::pair<x86::RegisterManager::Register,pSymbol> ConflictPair;
#endif
