#include "parser.h"
#include <iostream>
#include <sstream>
#include <vector>
int hexToLL(const std::string &hexStr)
{
    std::stringstream ss;
    ss << std::hex << hexStr;
    int address;
    ss >> address;
    return address;
}

std::vector<MemReq> parseRequestsFromStdin()
{
    std::vector<MemReq> requests;
    std::string line;
    while (std::getline(std::cin, line))
    {
        std::istringstream iss(line);
        char insChar;
        std::string addressStr;
        int ignore;
        if (iss >> insChar >> addressStr >> ignore)
        {
            Instruction ins = (insChar == 'l') ? Load : Save;
            int address = hexToLL(addressStr);
            requests.push_back({ins, address});
        }
    }
    return requests;
}
