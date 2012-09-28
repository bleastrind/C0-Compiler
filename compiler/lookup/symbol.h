#ifndef __BLEASTRIND_SYMBOLS
#define __BLEASTRIND_SYMBOLS

#include <map>
#include "../lexer/lexer.h"
#include "../util.h"
#include "../tuple/tuple.h"
#include "symbolfwd.h"
#include <string>
namespace lookup
{		


	class Symbol
	{
	public:

		Symbol(std::string name,lookup::ValueType rettype)
			:m_name(name),m_rettype(rettype) {}

		std::string getName() { return m_name; }
		virtual std::string toString() { return m_string; }
		virtual ValueType getType() { return m_rettype; }
	protected:
		std::string m_string;
	private:
		std::string m_name;
		ValueType m_rettype;
	};


	class Variable:public Symbol
	{
	public:
		Variable(std::string name,lookup::ValueType rettype)
			:Symbol(name,rettype) {}
		ValueType getType() { return Symbol::getType(); }
		std::string toString() { return Symbol::getName(); }
	};
		
	
	class Const:public Symbol
	{
	public:
		Const(std::string name,lookup::ValueType rettype)
			:Symbol(name,rettype) {}
		ValueType getType() { return Symbol::getType(); }
		std::string toString() { return Symbol::toString(); }
	};


	class Function:public Symbol
	{
	public:
		Function (std::string name, lookup::ValueType type)
			:Symbol( name,type)
		{
			m_string = "_"+name;
		}		
		ValueType getType() { return Symbol::getType(); }
		std::string toString() { return Symbol::toString(); }

		void addParam(lookup::ValueType param) { m_params.push_back( param ); }
		lookup::ValueType getParamKind(unsigned int index) { return m_params[index]; }
		unsigned int getParamCount() { return m_params.size(); }
	private:
		std::vector<lookup::ValueType> m_params;
	};

	class ConstChar: public Const
	{
	public:
		ConstChar (std::string name,char value)
			: Const( name,lookup::Char), m_value(value) 
		{
			m_string = util::ToString(value);
		}
		char getValue() { return m_value; }

	private:
		char m_value;
	};

	class ConstFloat: public Const
	{
	public:
		ConstFloat (std::string name,float value)
			:Const( name, lookup::Float), m_value(value)
		{
			m_string = util::ToString(value);
		}
		float getValue() { return m_value; }

	private:
		float m_value;
	};

	class ConstInt: public Const
	{
	public:
		ConstInt (std::string name,int value)
			: Const( name,lookup::Int), m_value(value)
		{
			m_string = util::ToString(value);
		}
		int getValue() { return m_value; }

	private:
		int m_value;
	};

	class String: public Const
	{
	public:
		
		String(std::string context,std::string rawcontext="")
			: Const( "@string" + util::ToString(stringid++) ,lookup::Null )
		{
			for (unsigned int i = 0;i < context.length();i++)
			{
				if (context[i] != '%')
					m_string += context[i];
				else
					m_string += "%%";
			}
			m_string += rawcontext;

		}
		void extend(std::string more)
		{
			m_string += more;
		}
	private:
		static int stringid;
	};

	class Label: public Const
	{
	public:
		Label()
			: Const( "@Label" + util::ToString(labelid++) ,lookup::Null )
		{
			m_string = getName();
		}
	private:
		static int labelid;
	};

}//namespace
#endif
