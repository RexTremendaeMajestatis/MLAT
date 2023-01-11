#pragma once

#include "object.h"
#include <vector>
#include <map>

using Pair = std::pair<ObjectPointer, std::vector<ObjectPointer>>;
using Model = std::map<std::string, bool>;
using S = std::vector<Disjunction>;

Pair Tseitin(Pair p);

ObjectPointer CNF(ObjectPointer tree);
