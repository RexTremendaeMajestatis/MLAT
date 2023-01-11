#include "object.h"
#include <cstdlib>
#include <sstream>

std::string Variable::DebugPrint() {
    std::stringstream ss;
    ss << "'" << name_ << "'";
    return ss.str();
}

ObjectPointer Variable::Simplify() {
    return shared_from_this();
}

std::string Constant::DebugPrint() {
    return value_ ? "TRUE" : "FALSE";
}

ObjectPointer Constant::Simplify() {
    return shared_from_this();
}

std::string Conjunction::DebugPrint() {
    std::stringstream ss;

    if (Is<Disjunction>(left_)) {
        ss << '(' << left_->DebugPrint() << ')';
    } else {
        ss << left_->DebugPrint();
    }

    ss << " & ";

    if (Is<Disjunction>(right_)) {
        ss << '(' << right_->DebugPrint() << ')';
    } else {
        ss << right_->DebugPrint();
    }

    return ss.str();
}

ObjectPointer Conjunction::Simplify() {
    left_ = left_->Simplify();
    right_ = right_->Simplify();

    if (Is<Constant>(left_)) {
        if (!As<Constant>(left_)->value_) {
            return std::make_shared<Constant>(false);
        }
    }

    if (Is<Constant>(right_)) {
        if (!As<Constant>(right_)->value_) {
            return std::make_shared<Constant>(false);
        }
    }

    if (Is<Constant>(left_)) {
        if (As<Constant>(left_)->value_) {
            return right_;
        }
    }

    if (Is<Constant>(right_)) {
        if (As<Constant>(right_)->value_) {
            return left_;
        }
    }

    return shared_from_this();
}

std::string Disjunction::DebugPrint() {
    std::stringstream ss;

    ss << left_->DebugPrint() << " | " << right_->DebugPrint();
    return ss.str();
}

ObjectPointer Disjunction::Simplify() {
    left_ = left_->Simplify();
    right_ = right_->Simplify();

    if (Is<Constant>(left_)) {
        if (As<Constant>(left_)->value_) {
            return std::make_shared<Constant>(true);
        }
    }

    if (Is<Constant>(right_)) {
        if (As<Constant>(right_)->value_) {
            return std::make_shared<Constant>(true);
        }
    }

    if (Is<Constant>(left_)) {
        if (!As<Constant>(left_)->value_) {
            return right_;
        }
    }

    if (Is<Constant>(right_)) {
        if (!As<Constant>(right_)->value_) {
            return left_;
        }
    }

    return shared_from_this();
}

std::string Negation::DebugPrint() {
    std::stringstream ss;
    ss << '!';
    if (Is<Conjunction>(expr_) || Is<Disjunction>(expr_)) {
        ss << '(' << expr_->DebugPrint() << ')';
    } else {
        ss << expr_->DebugPrint();
    }
    return ss.str();
}

ObjectPointer Negation::Simplify() {
    expr_ = expr_->Simplify();

    if (Is<Constant>(expr_)) {
        return As<Constant>(expr_)->value_ ? std::make_shared<Constant>(false) : std::make_shared<Constant>(true);
    }

    if (Is<Negation>(expr_)) {
        return As<Negation>(expr_)->expr_->Simplify();
    }


    return shared_from_this();
}
