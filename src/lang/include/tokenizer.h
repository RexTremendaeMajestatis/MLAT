#pragma once

#include "error.h"

#include <variant>
#include <deque>
#include <iostream>


enum class BracketToken { OPEN, CLOSE };

enum class OperationToken { AND, OR, NOT };

struct SymbolToken {
    std::string name;

    bool operator==(const SymbolToken& other) const {
        return name == other.name;
    }
};

struct UnknownToken {
    int byte;
    std::streamoff pos;
    bool operator==(const UnknownToken& other) const {
        return true;
    }
};

struct EndToken {
    bool operator==(const EndToken& other) const {
        return true;
    }
};

using Token = std::variant<BracketToken, OperationToken, SymbolToken, UnknownToken, EndToken>;

template<typename T>
bool operator==(const T& t, const Token& v) {
    auto* c = std::get_if<T>(&v);
    return c && (*c == t);
}

template<typename T>
bool operator==(const Token& v, const T& t) {
    return t == v;
}

template<typename T>
bool operator!=(const T& t, const Token& v) {
    return !(t == v);
}

template<typename T>
bool operator!=(const Token& v, const T& t) {
    return !(t == v);
}

struct TokenStringFunctor {
    std::string operator()(BracketToken bracket) const {
        switch (bracket)
        {
        case BracketToken::OPEN:
            return "BracketToken::OPEN";
            break;

        case BracketToken::CLOSE:
            return "BracketToken::CLOSE";
            break;

        default:
            throw LexicalError("unknown token");
        }
    }

    std::string operator()(OperationToken operation) const {
        switch (operation)
        {
        case OperationToken::AND:
            return "OperationToken::AND";
            break;

        case OperationToken::OR:
            return "OperationToken::OR";
            break;

        case OperationToken::NOT:
            return "OperationToken::NOT";
            break;

        default:
            throw LexicalError("unknown token");
        }
    }

    std::string operator()(SymbolToken symbol) const {
        return "SymbolToken::'" + symbol.name + "'";
    }

    std::string operator()(EndToken end) const {
        return "EndToken";
    }

    std::string operator()(UnknownToken unknown) const {
        auto res = std::string{};
        res += "UnknownToken::'";
        res += (char)unknown.byte;
        res += "'";
        return res;
    }
};

class Tokenizer {
public:
    Tokenizer(std::istream* in) : in_(in) {
        while (in_->peek() != EOF) {
            tokens_.push_back(ProcessNext());
        }

        tokens_.push_back(ProcessNext());
    }

    Token Peek();

    Token Next();

    void DebugPrint();

private:
    std::istream* in_;
    std::deque<Token> tokens_;

    bool IsSymbolBegin(char c);

    bool IsSymbolBody(char c);

    Token ProcessNext();
};
