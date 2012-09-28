#include "abstractcode.h"
#include "../tuple/tuplemanager.h"
void ast::AbstractCode::genCode()
{	
	tuple::TupleManager::addCode(getEnv(),
		tuple::Tuple::createTuple(m_op,m_src1,m_src2,m_target)
	);
}
