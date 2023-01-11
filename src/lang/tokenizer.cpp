#include "error.h"
#include "tokenizer.h"
#include <string>
#include <variant>
#include <deque>
#include <istream>
#include <iostream>


Token Tokenizer::Peek() {
    if (!tokens_.empty()) {
        return tokens_.front();
    }

    return EndToken{};
}

Token Tokenizer::Next() {
    Token ret = Peek();
    tokens_.pop_front();
    std::cout << std::visit(TokenStringFunctor(), ret) << std::endl;
    return ret;
}

void Tokenizer::DebugPrint() {
    for (auto el : tokens_) {
        std::cout << std::visit(TokenStringFunctor(), el) << " ";
    }

    std::cout << std::endl;
}

bool Tokenizer::IsSymbolBegin(char c) {
    return std::isalpha(c);
}

bool Tokenizer::IsSymbolBody(char c) {
    return IsSymbolBegin(c) || std::isdigit(c) || c == '-';
}

Token Tokenizer::ProcessNext() {
    while (std::isspace(in_->peek()))
    {
        in_->get();
    }

    while (in_->peek() != EOF)
    {
        if (in_->peek() == '(') {
            in_->get();
            return BracketToken::OPEN;
        }

        if (in_->peek() == ')') {
            in_->get();
            return BracketToken::CLOSE;
        }

        if (in_->peek() == '&') {
            in_->get();
            return OperationToken::AND;
        }

        if (in_->peek() == '|') {
            in_->get();
            return OperationToken::OR;
        }

        if (in_->peek() == '!') {
            in_->get();
            return OperationToken::NOT;
        }

        if (IsSymbolBegin(in_->peek())) {
            std::string symbol{};
            while (IsSymbolBody(in_->peek())) {
                symbol += in_->get();
            }

            return SymbolToken{.name = symbol};
        }

        return UnknownToken{.byte = in_->get(), .pos = in_->tellg()};
    }

    in_->get();
    return EndToken{};
}
