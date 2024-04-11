#include "data.h"
#include <set>
#pragma once
class CacheUtil
{
    CacheParameters params;

  public:
    long long get_block_num(long long address);
    long long get_set_num(long long address);
    long long get_mem_delay();
    bool is_full(std::set<long long> &s);
    bool alloc_on_write();
    WriteType get_write_type();
    CacheUtil(CacheParameters params);
};
