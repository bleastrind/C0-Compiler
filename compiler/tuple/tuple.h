#ifndef __BLEASTRIND_TUPLE
#define __BLEASTRIND_TUPLE
#include <string>
#include "../lookup/symbolfwd.h"
#include "tuplefwd.h"
#include "../util.h"
namespace tuple
{

	class Tuple
	{
	public:
		enum OpType
		{
			Null,

			JMP,	//	JUMP,	目标地址,		空	,	空				无条件跳转
			JE,		//	JE	,	变量,		变量	,	目标地址				等于跳转
			JNE,	//	JNE	,	变量,		变量	,	目标地址				不等于跳转
			JG,		//	JG	,	变量,		变量	,	目标地址				大于跳转
			JGE,	//	JGE	,	变量,		变量	,	目标地址				大于等于跳转
			JL,		//	JL	,	变量,		变量	,	目标地址				小于跳转
			JLE,	//	JLE	,	变量,		变量	,	目标地址				小于等于跳转


			PARAM,	//	PARAM,	参数	,		空	,	空				参数传递
			CALL,	//	CALL,	函数名	,		空	,	返回值				函数调用
			
			RET,	//	RET	,	函数名	,		返回值	,	空					获取函数返回值

			ASSIGN,	//	ASSIGN,	变量,	,		空	,	目标变量				赋值

			ADD,	//	ADD	,	变量	,	变量	,	目标变量				加法
			SUB,	//	SUB	,	变量	,	变量	,	目标变量				减法	
			MUL,	//	MUL,	变量	,	变量	,	目标变量				乘法
			DIV,	//	DIV,	变量	,	变量	,	目标变量				除法

			LABEL,
			FUNC,	//	FUNC,	函数名	,	空	,	空	,	空		函数调用
			ENDP,
			MAINEND,

			PRINTF,	// PRINTF, String ,var,空
			SCANF,//	SCANF,	String	,	空	,	var

		};

		static Tuple* createTuple(Tuple::OpType,lookup::Symbol*,lookup::Symbol*,lookup::Symbol*);

		typedef Tuple::OpType TupleOpType;
		Tuple( TupleOpType op,lookup::Symbol* src1, lookup::Symbol *src2, lookup::Symbol *target)
			:m_op(op),m_src1(src1),m_src2(src2),m_target(target){}
	
		//virtual ~Tuple();
		/*void setID(int id) { m_id = id;}
		int getID() { return m_id; }*/

		Tuple::OpType getOp() { return m_op; }
		void setSrc1(pSymbol p) { m_src1 = p; }
		void setSrc2(pSymbol p) { m_src2 = p; }
		lookup::Symbol* getSrc1() { return m_src1; }
		lookup::Symbol* getSrc2() { return m_src2; }
		lookup::Symbol* getTarget() { return m_target; }

		std::string toString();
	private:
		//int m_id;
		Tuple::OpType m_op;
		lookup::Symbol* m_src1;
		lookup::Symbol* m_src2;
		lookup::Symbol* m_target;
	};


}//namespace

#endif
