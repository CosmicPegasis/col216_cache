#include <list>
#include <map>
class LRUCache
{
    std::map<long long, std::list<long long>::iterator> m;

  public:
    long long get_lru();
    std::list<long long> l;
    void insert(long long addr);
    long long get_size();
    void erase();
};
