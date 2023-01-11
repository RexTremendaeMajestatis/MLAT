#include "interpretation.h"
#include "error.h"


#include <memory>

ObjectPointer Interpretation::True = std::make_shared<Constant>(true);
ObjectPointer Interpretation::False = std::make_shared<Constant>(false);

bool Interpretation::Define(std::string var, std::shared_ptr<Object> val) {
    if (variables_.find(var) == variables_.end()) {
        variables_[var] = val;
        return true;
    }

    return false;
}

bool Interpretation::Set(std::string var, std::shared_ptr<Object> val) {
    if (variables_.find(var) != variables_.end()) {
        variables_[var] = val;
        return true;
    }

    return false;
}

std::shared_ptr<Object> Interpretation::Get(std::string var) {
    if (variables_.find(var) != variables_.end()) {
        return variables_[var];
    }

    throw RuntimeError("Var is not defined");
}