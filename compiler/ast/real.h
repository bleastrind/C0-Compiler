#ifndef __BLEASTRIND_REAL
#define __BLEASTRIND_REAL
#include "../lookup/symbol.h"
#include "integer.h"
/******************************************************
<实数>    ::=<整数> [<实数补充>] 

//if set m_fromdeclaration
<实数> ↓identifier    ::=<整数> ↑Value[<实数补充>] @addIdentifier
//else
<实数> ↑Value   ::=<整数> ↑Value[<实数补充>] ↑Value
******************************************************/
namespace ast
{
	class Real:public AbstractConst
	{
	public:
		Real():m_int(0),m_float(0),m_valuetype(lookup::Int){}

		void nodeComplete();
		
		void setIdentifier(Token t){ m_identifier = t; m_fromdeclaration = true; }

		void addNode(pInteger);
		void addNode(pRealSuffix);
		int getInt() { return m_int; }
		float getValue() { return m_float; };

		lookup::ValueType getType() { return m_valuetype; }
	private:
		int m_int;
		float m_float;
		lookup::ValueType m_valuetype;

		Token m_identifier;
	};
}//namespace
#endif
