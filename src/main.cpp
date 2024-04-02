#include "arg_parser.h"
#include "data.h"
#include "parser.h"
#include <iostream>

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
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
