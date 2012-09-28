#ifndef __BLEASTRIND_ENV
#define __BLEASTRIND_ENV
#include <map>
#include <list>
#include "../lexer/lexer.h"
#include "../tuple/tuplefwd.h"
#include "symbolfwd.h"
#include "../optimization/alivevarinfofwd.h"
#include "../optimization/conflictgraphfwd.h"
namespace lookup
{
	class Env
	{
	public:

		static Env* newMain();	
		
		Env():m_parent(NULL),m_function(NULL),m_aliveVarInfo(NULL),m_conflictGraph(NULL){}
		

		static lookup::Variable* createTemp(ValueType);
		static lookup::Variable* createVariable(std::string,ValueType);
		static lookup::Const* createConst(std::string,int);
		static lookup::Const* createConst(std::string,float);
		static lookup::Const* createConst(std::string,char);
		static lookup::Function* createFunction(std::string , lookup::ValueType );

		static lookup::Const* createConst(int);
		static lookup::Const* createConst(float);
		static lookup::Const* createConst(char);
		static lookup::Const* createZero();
		static lookup::String* createString(std::string,std::string suffix = "");
		static lookup::Label* createLabel();

		static bool IsGloablConst(Symbol*);
		static std::map<float,Const*> getFloatConsts(){ return s_constfloatPool; }
		
		static std::map<std::string,String*> getStrings() { return s_conststringPool; }

		std::map<std::string,Variable *> getVariables() { return m_variables; }
		//if not found,return NULL
		Const* findConst(std::string) const;
		Const* findConstOnLevel(std::string) const;
		Symbol* findSymbol(std::string) const;
		Symbol* findSymbolOnLevel(std::string) const;
		Variable* findVariableOrParam(std::string) const;
		Variable* findVariableOrParamOnLevel(std::string) const;
		Variable* findVariableOnLevel(std::string) const;
		Function* findFunction(std::string) const;
		//if existed in the same level, return false
		bool addConst(Const*);
		bool addVariable(Variable*);
		bool addParam(Variable*);
		bool addFunction(Function*);


		bool isTop() const{ return m_parent == NULL; }

		Env* getParent() const { return m_parent; }
		void setParent(Env * p) { m_parent = p; }

		void setFunction(Function * f) { m_function = f; }
		lookup::Function* getFunction() { return m_function; }

		std::list<Variable*> getParams() { return m_params;}
		std::map<std::string,Const*> getLocalConsts() { return m_consts; };
		
		optimization::AliveVarInfo* getAliveVarInfo() { return m_aliveVarInfo; }
		void setAliveVarInfo(optimization::AliveVarInfo * a) { m_aliveVarInfo = a; }
		
		optimization::ConflictGraph* getConflictGraph() { return m_conflictGraph; }
		void setConflictGraph(optimization::ConflictGraph * a) { m_conflictGraph = a; }
		
		~Env();
	private:
		static std::map<int,Const*> s_constintPool;
		static std::map<float,Const*> s_constfloatPool;
		static std::map<char,Const*> s_constcharPool;

		static std::map<std::string,String*> s_conststringPool;
		static std::list<Label*> s_labelPool;


		Env* m_parent;

		std::list<Variable*> m_params;
		mutable std::map<std::string,Variable *> m_variables;
		mutable std::map<std::string,Const*> m_consts;
		mutable std::map<std::string,Function*> m_functions;

		//记录当前环境的函数
		Function* m_function;

		//记录当前环境内的活跃变量情况
		optimization::AliveVarInfo* m_aliveVarInfo;
		optimization::ConflictGraph* m_conflictGraph;
		
		lookup::Variable* findParam(std::string) const;
	};
}//namespace
#endif
