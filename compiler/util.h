#ifndef __BLEASTRIND_UTIL
#define __BLEASTRIND_UTIL
#include <sstream>
#include <stack>
#include <string>
class UnChangeable
{
private:
	UnChangeable& operator=(const UnChangeable&);
};


namespace util
{
	std::string ToString(int);
	std::string ToString(char);
	std::string ToString(float);

	std::string& replace_once(std::string& str,const std::string& old_value,const std::string& new_value);
}
#endif

