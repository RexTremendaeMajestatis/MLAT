#pragma once

#include "error.h"

#include <memory>
#include <string>
#include <sstream>


class Object : public std::enable_shared_from_this<Object> {
public:
    virtual ~Object() = default;
    virtual std::string DebugPrint() {
        throw RuntimeError("DebugPrint not implemented");
    }
    virtual std::shared_ptr<Object> Simplify() {
        throw RuntimeError("Simplify not implemented");
    }
};

using ObjectPointer = std::shared_ptr<Object>;

template <class T>
std::shared_ptr<T> As(const ObjectPointer& obj) {
    return std::dynamic_pointer_cast<T>(obj);
}

template <class T>
bool Is(const ObjectPointer& obj) {
    return std::dynamic_pointer_cast<T>(obj) != nullptr;
}

struct Variable : public Object {
    Variable() {
        static size_t i = 1;
        std::stringstream ss;
        ss << "auto_" << i++;
        name_ = ss.str();
    }

    Variable(std::string name) : name_(name) {
    }

    std::string DebugPrint() override;

    ObjectPointer Simplify() override;

    std::string name_;
};

struct Constant : public Object {
    Constant(bool value) : value_(value) {
    }

    std::string DebugPrint() override;

    ObjectPointer Simplify() override;

    bool value_;
};

struct Conjunction : public Object {
    Conjunction(ObjectPointer left, ObjectPointer right)
        : left_(std::move(left)), right_(std::move(right)) {
    }

    std::string DebugPrint() override;

    ObjectPointer Simplify() override;

    ObjectPointer left_;
    ObjectPointer right_;
};

struct Disjunction : public Object {
    Disjunction(ObjectPointer left, ObjectPointer right)
        : left_(std::move(left)), right_(std::move(right)) {
    }

    std::string DebugPrint() override;

    ObjectPointer Simplify() override;

    ObjectPointer left_;
    ObjectPointer right_;
};

struct Negation : public Object {
    Negation(ObjectPointer expr) : expr_(std::move(expr)) {
    }

    std::string DebugPrint() override;

    ObjectPointer Simplify() override;

    ObjectPointer expr_;
};
