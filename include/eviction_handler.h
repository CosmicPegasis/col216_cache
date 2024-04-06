#include "data.h"
#pragma once

class EvictionHandler
{
  public:
    EvictionType evict;

    EvictionHandler(EvictionType type) : evict{type} {};
    EvictionHandler(){};
    ~EvictionHandler(){};

    virtual int get_size(int index)
    {
        (void)index;
        return 0;
    };
    virtual int get_to_evict(int index)
    {
        (void)index;
        return 0;
    };
    virtual void insert(int index, int addr)
    {
        (void)index;
        (void)addr;
    };
    virtual void touch(int index, int addr)
    {
        (void)index;
        (void)addr;
    };
    virtual void erase(int index)
    {
        (void)index;
    };
};
