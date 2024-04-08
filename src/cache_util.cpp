#include "cache_util.h"
#include <iostream>

CacheUtil::CacheUtil(CacheParameters params) : params{params} {};

int CacheUtil::get_block_num(int address)
{
    /* Memory is byte addressed
     * If each block holds 2^b bytes
     * From a byte address if we want the the block address,
     * we can divide by 2^b to get block address
     */

    return address / params.bytes_per_block;
}
int CacheUtil::get_set_num(int address)
{
    return get_block_num(address) % params.sets;
}

int CacheUtil::get_mem_delay()
{
    return MEM_DELAY * (params.bytes_per_block / NUM_TRANSFER_BYTES);
}

bool CacheUtil::is_full(std::set<int> &s)
{
    if (s.size() == params.blocks)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool CacheUtil::alloc_on_write()
{
    return params.allocate_on_write;
}
WriteType CacheUtil::get_write_type()
{
    return params.writeType;
}
