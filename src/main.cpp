#include "arg_parser.h"
#include "cache.h"
#include "data.h"
#include "parser.h"
#include <iostream>

void print_cache_statistics(const CacheStatistics &stats)
{
    std::cout << "Total loads: " << stats.loads << std::endl;
    std::cout << "Total stores: " << stats.stores << std::endl;
    std::cout << "Load hits: " << stats.load_hits << std::endl;
    std::cout << "Load misses: " << stats.load_misses << std::endl;
    std::cout << "Store hits: " << stats.store_hits << std::endl;
    std::cout << "Store misses: " << stats.store_misses << std::endl;
    std::cout << "Total cycles: " << stats.cycles << std::endl;
}
void print_cache_params(const CacheParameters &params)
{

    std::cout << "Cache parameters:" << std::endl;
    std::cout << "Sets: " << params.sets << std::endl;
    std::cout << "Blocks per set: " << params.blocks << std::endl;
    std::cout << "Bytes per block: " << params.bytes_per_block << std::endl;
    std::cout << "Write policy:" << params.writeType << std::endl;
    std::cout << "Write allocation: " << (params.allocate_on_write ? "Yes" : "No") << std::endl;
    std::cout << "Eviction policy: " << (params.evict == LRU ? "LRU" : "FIFO") << std::endl;
}
int main(int argc, char **argv)
{
    try
    {
        CacheParameters params = parseArguments(argc, argv);
        std::vector<MemReq> requests = parseRequestsFromStdin();
        //        print_cache_params(params);
        Cache cache{params};
        for (MemReq req : requests)
        {
            cache.proc_request(req);
        }
        print_cache_statistics(cache.get_statistics());
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
