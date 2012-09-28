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

			JMP,	//	JUMP,	Ŀ���ַ,		��	,	��				��������ת
			JE,		//	JE	,	����,		����	,	Ŀ���ַ				������ת
			JNE,	//	JNE	,	����,		����	,	Ŀ���ַ				��������ת
			JG,		//	JG	,	����,		����	,	Ŀ���ַ				������ת
			JGE,	//	JGE	,	����,		����	,	Ŀ���ַ				���ڵ�����ת
			JL,		//	JL	,	����,		����	,	Ŀ���ַ				С����ת
			JLE,	//	JLE	,	����,		����	,	Ŀ���ַ				С�ڵ�����ת


			PARAM,	//	PARAM,	����	,		��	,	��				��������
			CALL,	//	CALL,	������	,		��	,	����ֵ				��������
			
			RET,	//	RET	,	������	,		����ֵ	,	��					��ȡ��������ֵ

			ASSIGN,	//	ASSIGN,	����,	,		��	,	Ŀ�����				��ֵ

			ADD,	//	ADD	,	����	,	����	,	Ŀ�����				�ӷ�
			SUB,	//	SUB	,	����	,	����	,	Ŀ�����				����	
			MUL,	//	MUL,	����	,	����	,	Ŀ�����				�˷�
			DIV,	//	DIV,	����	,	����	,	Ŀ�����				����

			LABEL,
			FUNC,	//	FUNC,	������	,	��	,	��	,	��		��������
			ENDP,
			MAINEND,

			PRINTF,	// PRINTF, String ,var,��
			SCANF,//	SCANF,	String	,	��	,	var

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
