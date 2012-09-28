#include "util.h"
#include <string>
namespace
{
	std::string toStringint(int t)
	{
		std::stringstream ss;
		ss<<t;
		return ss.str();
	}
	
	std::string toStringfloat(float t)
	{
		int * temp = (int*)(&t);
		std::stringstream ss;
		ss<<"0"<<std::hex<<*temp<<"H";
		return ss.str();
	}
}
std::string util::ToString(int a)
{
	return toStringint(a);
}
std::string util::ToString(char a)
{
	return toStringint(a);
}
std::string util::ToString(float a)
{
	return toStringfloat(a);
}


std::string& util::replace_once(std::string& str,const std::string& old_value,const std::string& new_value)   
{   
	unsigned int pos=0;
	if( (pos=str.find(old_value,pos)) != std::string::npos )   
		str.replace(pos,old_value.length(),new_value);   

	return str;   
}   