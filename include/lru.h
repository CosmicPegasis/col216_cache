#include <list>
#include <map>

class LRUCache
{
    std::map<int, std::list<int>::iterator> m;

  public:
    int get_lru();
    std::list<int> l;
    void insert(int addr);
    int get_size();
    void erase();
};
