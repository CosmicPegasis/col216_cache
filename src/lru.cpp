#include "lru.h"
#include <stdexcept>

long long LRUCache::get_lru()
{
    if (l.size())
    {
        return l.back();
    }
    else
    {
        throw std::runtime_error("LRUCache is empty");
    }
}
void LRUCache::insert(long long addr)
{
    if (m.find(addr) != m.end())
    {
        l.erase(m[addr]);
    }
    l.push_front(addr);
    m[addr] = l.begin();
}
long long LRUCache::get_size()
{
    return l.size();
}

void LRUCache::erase()
{
    m.erase(l.back());
    l.pop_back();
}
