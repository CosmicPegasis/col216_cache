#include "arg_parser.h"
#include <stdexcept>
#include <string>

CacheParameters parseArguments(int argc, char *argv[])
{
    if (argc != 7)
    {
        throw std::invalid_argument("Usage: <sets> <blocks> <bytes_per_block> <write_allocation: "
                                    "write-allocate or no-write-allocate> <write_policy: write-through or "
                                    "write-back> <eviction_policy: lru or fifo>");
    }

    CacheParameters params;

    try
    {
        params.sets = std::stoi(argv[1]);
        params.blocks = std::stoi(argv[2]);
        params.bytes_per_block = std::stoi(argv[3]);
        params.allocate_on_write = (std::string(argv[4]) == "write-allocate");
    }
    catch (const std::exception &e)
    {
        throw std::invalid_argument("Invalid argument(s) for sets, blocks, "
                                    "bytes_per_block, or write_allocation");
    }

    if (params.sets <= 0 || (params.sets & (params.sets - 1)) != 0)
    {
        throw std::invalid_argument("Number of sets must be a positive power-of-2");
    }

    if (params.blocks <= 0 || (params.blocks & (params.blocks - 1)) != 0)
    {
        throw std::invalid_argument("Number of blocks must be a positive power-of-2");
    }

    if (params.bytes_per_block < 4 || (params.bytes_per_block & (params.bytes_per_block - 1)) != 0)
    {
        throw std::invalid_argument("Number of bytes per block must be a positive "
                                    "power-of-2 and at least 4");
    }

    std::string writePolicy(argv[5]);
    if (writePolicy == "write-through")
    {
        params.writeType = WriteThrough;
    }
    else if (writePolicy == "write-back")
    {
        params.writeType = WriteAllocate;
    }
    else
    {
        throw std::invalid_argument("Invalid write policy. Choose either write-through or write-back");
    }

    std::string evictionPolicy(argv[6]);
    if (evictionPolicy == "lru")
    {
        params.evict = LRU;
    }
    else if (evictionPolicy == "fifo")
    {
        params.evict = FIFO;
    }
    else
    {
        throw std::invalid_argument("Invalid eviction policy. Choose either lru or fifo");
    }

    return params;
}
/* int main(int argc, char *argv[]) { */
/*   try { */
/*     CacheParameters params = parseArguments(argc, argv); */
/*     std::cout << "Cache parameters:" << std::endl; */
/*     std::cout << "Sets: " << params.sets << std::endl; */
/*     std::cout << "Blocks per set: " << params.blocks << std::endl; */
/*     std::cout << "Bytes per block: " << params.bytes_per_block << std::endl;
 */
/*     std::cout << "Write allocation: " */
/*               << (params.allocate_on_write ? "Yes" : "No") << std::endl; */
/*     std::cout << "Eviction policy: " << (params.evict == LRU ? "LRU" :
 * "FIFO") */
/*               << std::endl; */
/*   } catch (const std::exception &e) { */
/*     std::cerr << "Error: " << e.what() << std::endl; */
/*     return 1; */
/*   } */
/*   return 0; */
/* } */
