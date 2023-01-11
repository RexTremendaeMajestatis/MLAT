#include "parser.h"
#include "error.h"
#include "CNF.h"
#include "interpretation.h"

#include <string>
#include <sstream>


int main() {
    auto i = Interpretation();
    i.Define("a", Interpretation::False);
    i.Define("b", Interpretation::True);
    std::stringstream ss{"a & !a"};
    Tokenizer tokenizer{&ss};
    Parser parser{&tokenizer};

    parser.DebugPrint();

    auto tree = parser.GetSimplifiedTree();
    i.Define("pasha", tree);
    std::cout << tree->Eval(&i)->DebugPrint() << std::endl;

    auto x = CNF(tree);

    std::cout << x->DebugPrint() << std::endl;

    return 0;
}