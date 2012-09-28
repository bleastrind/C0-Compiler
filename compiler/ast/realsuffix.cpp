#include "realsuffix.h"
#include <cmath>
#include <assert.h>
#include <cctype>
void ast::RealSuffix::addToken(Token t)
{
	std::string str = t.getString();
	m_value = (float)atoi(str.c_str()) / pow(10,(float)str.size());
}
