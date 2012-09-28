#ifndef __BLEASTRIND_BASEHANDLER
#define __BLEASTRIND_BASEHANDLER
#include <iostream>
#include <map>
#include <list>
#include "../tuple/tuple.h"
#include "../tuple/tuplemanager.h"
#include "../lookup/symbolfwd.h"
#include "../util.h"
#include "registermanager.h"
namespace x86
{
	class BaseHandler:public tuple::TupleHandler,public UnChangeable
	{
	public:

		std::string getOpString(tuple::Tuple::OpType);
		
		BaseHandler(std::ostream &os = std::cout,lookup::Env* env=NULL,bool changeLine=true);

		void recieveTuple(lookup::Env*,pTuple);
	private:

		bool m_changeLine;

		RegisterManager m_registerManager;

		

		std::ostream& m_ostream;
		pTuple m_tuple;
		lookup::Env* m_env;

		std::list<std::string> m_output;
		std::list<std::string> m_data;
		std::list<std::string> m_code;
		std::list<std::string> m_function;
		
		std::list<lookup::ValueType> m_pushedparam;

		void emitJUMP();
		void emitIFJUMP();
		void emitPARAM();
		void emitCALL();
		void emitRET();
		void emitASSIGN();
		void emitADDS();
		void emitMULS();
		void emitLABEL();
		void emitFUNC();
		void emitENDP();
		void emitMAINEND();
		void emitPRINTF();
		void emitSCANF();

		std::map<tuple::Tuple::OpType,std::string> m_opstringMap;
		


		//for optimization
		bool WillAlive(pSymbol p,bool includecurrent);


		//for calc
		void intAdds(pSymbol,pSymbol,pSymbol);
		void intDiv(pSymbol,pSymbol,pSymbol);
		void floatCalcs(pSymbol,pSymbol,pSymbol);
		void floatJump(tuple::Tuple::OpType);
		
		//for convert type
		typedef void (BaseHandler::* MovFunc)(pSymbol,pSymbol);
		static MovFunc convertTable[4][4];
		void movError(pSymbol,pSymbol);
		void movCharToChar(pSymbol,pSymbol);
		void movIntToInt(pSymbol,pSymbol);
		void movFloatToFloat(pSymbol,pSymbol);
		void movIntToChar(pSymbol,pSymbol);
		void movCharToInt(pSymbol,pSymbol);
		void movIntToFloat(pSymbol,pSymbol);
		void movFloatToInt(pSymbol,pSymbol);
		void movCharToFloat(pSymbol,pSymbol);
		void movFloatToChar(pSymbol,pSymbol);
		void MOV(pSymbol target,pSymbol src);

		lookup::ValueType unifyArgsTo(pSymbol &,pSymbol &);

		//for code cope
		void saveVarInTempRegister();
		void occupyRegister( pSymbol,x86::RegisterManager::Register reg = x86::RegisterManager::FAILED);
		void saveConflict(ConflictPair,bool includecurrent = false);
		void flushCode();
		void flushAll();
		void functionComplete();

		void putLocalFloat();

		std::list<std::string> getRegRestore();
		std::list<std::string> getRegProtect();
		void replaceParamDecl();
		void replaceVarDecl();
		std::map<std::string,std::string> getParamDecl();
		std::map<std::string,std::string> getVarDecl();

		std::list<std::string> getStringDecl();
		std::list<std::string> getConstFloatDecl();
		std::list<std::string> getGlobalVarDecl();
	};
}//namespace
#endif
