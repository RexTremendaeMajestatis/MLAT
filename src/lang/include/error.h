#pragma once

#include <stdexcept>


class SyntaxError : public std::runtime_error {
    using std::runtime_error::runtime_error;
};

class RuntimeError : public std::runtime_error {
    using std::runtime_error::runtime_error;
};

class LexicalError : public std::runtime_error {
public:
    explicit LexicalError(const std::string msg) : std::runtime_error{"Lexical error: " + msg} {
    }
};

class NameError : public std::runtime_error {
public:
    explicit NameError(const std::string& name) : std::runtime_error{"Name not found: " + name} {
    }
};
