#include "parser.h"
#include "error.h"
#include "CNF.h"

#include <string>
#include <sstream>


int main() {
    std::stringstream ss{"!((a & b))"};
    Tokenizer tokenizer{&ss};
    Parser parser{&tokenizer};

    parser.DebugPrint();

    auto tree = parser.GetSimplifiedTree();
    std::cout << tree->DebugPrint() << std::endl;

    auto x = CNF(tree);

    std::cout << x->DebugPrint() << std::endl;

    return 0;
}