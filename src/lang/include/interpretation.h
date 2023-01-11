#pragma once

#include "object.h"

#include <map>
#include <string>


class Interpretation {
public:
    bool Define(std::string var, ObjectPointer val);

    bool Set(std::string var, ObjectPointer val);

    ObjectPointer Get(std::string var);

    static ObjectPointer True;
    static ObjectPointer False;

private:
    std::map<std::string, ObjectPointer> variables_;
};