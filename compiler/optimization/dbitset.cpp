#include "dbitset.h"
#include <iostream>
#include <cassert>

#define _NO_WASTE_SPACE(arg) ((arg).m_members.size() <= 1 || (arg).m_members.back() != 0)
const unsigned int DBitSet::bits_per_byte = 8;
const unsigned int DBitSet::bits_per_int = DBitSet::bits_per_byte * sizeof(unsigned int);

DBitSet DBitSet::operator+(const unsigned int& element)
{
    DBitSet result(*this);
    result += element;
    return result;
}

DBitSet DBitSet::operator+(const DBitSet& oper)
{
    DBitSet result(*this);
    result += oper;
    return result;
}

DBitSet& DBitSet::operator+=(const unsigned int& element)
{
    unsigned int index = element / bits_per_int;
    unsigned int bit = element % bits_per_int;

    if(index >= m_members.size())
    {
        m_members.resize(index + 1);
    }
    m_members[index] |= (1 << bit);
    assert(_NO_WASTE_SPACE((*this)));
    return *this;
}

DBitSet& DBitSet::operator+=(const DBitSet& oper)
{
    unsigned int lower_bound = m_members.size() < oper.m_members.size() ? m_members.size() : oper.m_members.size();
    unsigned int upper_bound = m_members.size() > oper.m_members.size() ? m_members.size() : oper.m_members.size();

    m_members.resize(upper_bound);
    for(unsigned int i = 0; i < lower_bound; i++)
    {
        m_members[i] |= oper.m_members[i];
    }
    for(unsigned int j = lower_bound; j < oper.m_members.size(); j++)
    {
        m_members[j] = oper.m_members[j];
    }
    assert(_NO_WASTE_SPACE(*this));
    return *this;
}

DBitSet DBitSet::operator-(const unsigned int& element)
{
    DBitSet result(*this);
    result -= element;
    return result;
}

DBitSet& DBitSet::operator-=(const unsigned int& element)
{
    unsigned int index = element / bits_per_int;
    unsigned int bit = element % bits_per_int;
    if(index < m_members.size())
    {
        m_members[index] &= (~(1 << bit));
    }
    for(unsigned int i = m_members.size(); i > 0; i--)
    {
        if(m_members[i - 1] || i == 1)
        {
            m_members.resize(i);
            break;
        }
    }
    assert(_NO_WASTE_SPACE(*this));
    return *this;
}

DBitSet DBitSet::operator-(const DBitSet& oper)
{
    DBitSet result(*this);
    result -= oper;
    return result;
}

DBitSet& DBitSet::operator-=(const DBitSet& oper)
{
    unsigned int lower_bound = m_members.size() < oper.m_members.size() ? m_members.size() : oper.m_members.size();

    for(unsigned int i = 0; i < lower_bound; i++)
    {
        m_members[i] &= (~oper.m_members[i]);
    }
    for(unsigned int j = m_members.size(); j > 0; j--)
    {
        if(m_members[j - 1] || j == 1)
        {
            m_members.resize(j);
            break;
        }
    }
    assert(_NO_WASTE_SPACE(*this));
    return *this;
}

DBitSet DBitSet::operator&(const DBitSet& oper)
{
    DBitSet result(*this);
    result &= oper;
    return result;
}

DBitSet& DBitSet::operator&=(const DBitSet& oper)
{
    unsigned int lower_bound = m_members.size() < oper.m_members.size() ? m_members.size() : oper.m_members.size();

    m_members.resize(lower_bound);
    for(unsigned int i = 0; i < lower_bound; i++)
    {
        m_members[i] &= oper.m_members[i];
    }
    for(unsigned int j = m_members.size(); j > 0; j--)
    {
        if(m_members[j - 1] || j == 1)
        {
            m_members.resize(j);
            break;
        }
    }
    assert(_NO_WASTE_SPACE(*this));
    return *this;
}

bool DBitSet::empty() const
{
    assert(_NO_WASTE_SPACE(*this));
    if(m_members.size() == 0 || (m_members.size() == 1 && m_members[0] == 0))
        return true;
    else
        return false;
}

bool DBitSet::find(unsigned int element) const
{
    assert(_NO_WASTE_SPACE(*this));
    unsigned int index = element / bits_per_int;
    unsigned int bit = element % bits_per_int;
    if(index >= m_members.size())
    {
        return false;
    }
    else 
    {
        if(m_members[index] & (1 << bit))
            return true;
        else
            return false;
    }
}

std::vector<unsigned int> DBitSet::getNumbers() const
{
    unsigned int number = 0;
    std::vector<unsigned int> numbers;
    for(unsigned int i = 0; i < m_members.size(); i++)
    {
        for(unsigned int j = 0; j < bits_per_int; j++)
        {
            if(m_members[i] & (1 << j))
            {
                numbers.push_back(number + j);
            }
        }
        number += bits_per_int;
    }
    return numbers;
}

bool operator==(const DBitSet& oper1, const DBitSet& oper2)
{
    if(oper1.m_members.size() != oper2.m_members.size())return false;
    assert(_NO_WASTE_SPACE(oper1));
    assert(_NO_WASTE_SPACE(oper2));
    for(unsigned int i = 0; i < oper1.m_members.size(); i++)
    {
        if(oper1.m_members[i] != oper2.m_members[i])
            return false;
    }
    return true;
}

bool operator!=(const DBitSet& oper1, const DBitSet& oper2)
{
    return !(oper1 == oper2);
}

std::ostream& operator<<(std::ostream& out, const DBitSet& set)
{
    unsigned int number = 0;
    bool first_element = true;
    out << "{";
    for(unsigned int i = 0; i < set.m_members.size(); i++)
    {
        for(unsigned int j = 0; j < DBitSet::bits_per_int; j++)
        {
            if(set.m_members[i] & (1 << j))
            {
                if(!first_element)
                    out << ",";
                else
                    first_element = false;
                out << (number + j);
            }
        }
        number += DBitSet::bits_per_int;
    }
    out << "}" ;
	return out;
}
