#ifndef __BLEASTRIND_SYMBOLSFWD
#define __BLEASTRIND_SYMBOLSFWD

namespace lookup
{		

	class Symbol;

	class Variable;
	class Const;
	class Function;
	class String;
	class Label;
	//位置不可变动，会用来比较
	enum ValueType
	{
		Null,
		Char,
		Int,
		Float
	};
}//namespace
typedef lookup::Symbol* pSymbol;
#endif
