#include "data.h"
#include <set>
#pragma once
class CacheUtil
{
    CacheParameters params;

  public:
    int get_block_num(int address);
    int get_set_num(int address);
    int get_mem_delay();
    bool is_full(std::set<int> &s);
    bool alloc_on_write();
    WriteType get_write_type();
    CacheUtil(CacheParameters params);
};
