#include "data.h"
#pragma once
class EvictionHandler
{
  public:
    EvictionType evict;

    virtual ~EvictionHandler(){};
    virtual long long get_size(long long index) = 0;
    virtual long long get_to_evict(long long index) = 0;
    virtual void insert(long long index, long long addr) = 0;
    virtual void touch(long long index, long long addr) = 0;
    virtual void erase(long long index) = 0;
};
