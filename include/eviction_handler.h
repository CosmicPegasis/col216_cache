#include "data.h"
#pragma once

class EvictionHandler
{
  public:
    EvictionType evict;

    virtual ~EvictionHandler(){};
    virtual int get_size(int index) = 0;
    virtual int get_to_evict(int index) = 0;
    virtual void insert(int index, int addr) = 0;
    virtual void touch(int index, int addr) = 0;
    virtual void erase(int index) = 0;
};
