#include "arg_parser.h"
#include "cache.h"
#include "data.h"
#include "parser.h"
#include <iostream>

void print_cache_statistics(const CacheStatistics &stats)
{
    std::cout << "Cache Statistics:" << std::endl;
    std::cout << "  Loads:           " << stats.loads << std::endl;
    std::cout << "  Load Hits:        " << stats.load_hits << std::endl;
    std::cout << "  Load Misses:      " << stats.load_misses << std::endl;
    std::cout << "  Stores:          " << stats.stores << std::endl;
    std::cout << "  Store Hits:       " << stats.store_hits << std::endl;
    std::cout << "  Store Misses:     " << stats.store_misses << std::endl;
    std::cout << "  Cycles:           " << stats.cycles << std::endl;
}
int main(int argc, char **argv)
{
    try
    {
        CacheParameters params = parseArguments(argc, argv);

        std::vector<MemReq> requests = parseRequestsFromStdin();
        std::cout << "Cache parameters:" << std::endl;
        std::cout << "Sets: " << params.sets << std::endl;
        std::cout << "Blocks per set: " << params.blocks << std::endl;
        std::cout << "Bytes per block: " << params.bytes_per_block << std::endl;

        std::cout << "Write allocation: " << (params.allocate_on_write ? "Yes" : "No") << std::endl;
        std::cout << "Eviction policy: " << (params.evict == LRU ? "LRU" : "FIFO") << std::endl;
        for (const auto &req : requests)
        {
            std::cout << "Instruction: " << (req.ins == Load ? "Load" : "Save") << ", Address: " << req.address
                      << std::endl;
        }

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
