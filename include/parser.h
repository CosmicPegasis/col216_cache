#include "data.h"
#include <string>
#include <vector>

#pragma once

int hexToLL(const std::string &hexStr);
std::vector<MemReq> parseRequestsFromStdin();
