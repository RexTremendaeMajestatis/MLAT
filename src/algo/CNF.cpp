#include "CNF.h"

Pair Tseitin(Pair p) {
    if (Is<Variable>(p.first)) {
        return p;
    }

    if (Is<Negation>(p.first)) {
        auto x = Tseitin(Pair(As<Negation>(p.first)->expr_, p.second));
        return Pair(std::make_shared<Negation>(x.first), x.second);
    }

    if (Is<Conjunction>(p.first)) {
        auto newVariable = std::make_shared<Variable>();

        auto left = Tseitin(Pair(As<Conjunction>(p.first)->left_, p.second));
        auto right = Tseitin(Pair(As<Conjunction>(p.first)->right_, left.second));

        auto newDelta = std::vector<ObjectPointer>(right.second);

        newDelta.push_back(
            std::make_shared<Disjunction>(
                std::make_shared<Negation>(newVariable)
                , left.first)->Simplify());

        newDelta.push_back(
            std::make_shared<Disjunction>(
                std::make_shared<Negation>(newVariable)
                , right.first)->Simplify());

        newDelta.push_back(
            std::make_shared<Disjunction>(
                std::make_shared<Disjunction>(
                    std::make_shared<Negation>(left.first)
                    , std::make_shared<Negation>(right.first))
                , newVariable)->Simplify());

        return Pair(newVariable, newDelta);
    }

    if (Is<Disjunction>(p.first)) {
        auto newVariable = std::make_shared<Variable>();

        auto left = Tseitin(Pair(As<Disjunction>(p.first)->left_, p.second));
        auto right = Tseitin(Pair(As<Disjunction>(p.first)->right_, left.second));

        auto newDelta = std::vector<ObjectPointer>(right.second);

        newDelta.push_back(
            std::make_shared<Disjunction>(
                std::make_shared<Disjunction>(
                    std::make_shared<Negation>(newVariable)
                    , left.first)
                , right.first)->Simplify());

        newDelta.push_back(
            std::make_shared<Disjunction>(
                std::make_shared<Negation>(left.first)
                , newVariable)->Simplify());

        newDelta.push_back(
            std::make_shared<Disjunction>(
                std::make_shared<Negation>(right.first)
                , newVariable)->Simplify());

        return Pair(newVariable, newDelta);
    }

    return Pair(nullptr, std::vector<ObjectPointer>());
}

ObjectPointer CNF(ObjectPointer tree) {
    auto t = Tseitin(Pair(tree, std::vector<ObjectPointer>()));

    auto x = t.first;

    for (auto e : t.second) {
        x = std::make_shared<Conjunction>(x, e);
    }

    return x;
}
