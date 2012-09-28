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
	//λ�ò��ɱ䶯���������Ƚ�
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
