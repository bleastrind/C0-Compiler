#ifndef __BLEASTRIND_DBITSET
#define __BLEASTRIND_DBITSET

#include <vector>

class DBitSet
{
public:
    DBitSet operator+(const unsigned int&);
    DBitSet operator+(const DBitSet&);//并集
    DBitSet& operator+=(const unsigned int&);
    DBitSet& operator+=(const DBitSet&);
    DBitSet operator-(const unsigned int&);
    DBitSet operator-(const DBitSet&);//差集
    DBitSet& operator-=(const unsigned int&);
    DBitSet& operator-=(const DBitSet&);
    DBitSet operator&(const DBitSet&);//交集
    DBitSet& operator&=(const DBitSet&);

    bool empty() const;
    bool find(unsigned int) const;
    std::vector<unsigned int> getNumbers() const;
    friend bool operator==(const DBitSet&, const DBitSet&);
    friend bool operator!=(const DBitSet&, const DBitSet&);
    friend std::ostream& operator<<(std::ostream&, const DBitSet&);
private:
    std::vector<unsigned int> m_members;
    const static unsigned int bits_per_byte;
    const static unsigned int bits_per_int;
};

#endif