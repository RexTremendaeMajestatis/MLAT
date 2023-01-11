#include "object.h"
#include "tokenizer.h"
#include "parser.h"
#include "interpretation.h"

/**
 *  E    ->      E | M      |       M
 *  M    ->      M & A      |       A
 *  A    ->       !A        |      (E)
 *
 *  E - Object
 *  M - Monome
 *  A - Atom
 */

void Parser::DebugPrint() {
    std::cout << tree_->DebugPrint() << std::endl;
}

ObjectPointer Parser::GetTree() {
    return tree_;
}

ObjectPointer Parser::GetSimplifiedTree() {
    return tree_->Simplify();
}

Token Parser::Next(Tokenizer* tok) {
    auto ret = tok->Next();

    if (ret == BracketToken::OPEN) {
        bracket_counter_++;
    }

    if (ret == BracketToken::CLOSE) {
        bracket_counter_--;
    }

    if (bracket_counter_ < 0) {
        throw SyntaxError("Unexpected close bracket");
    }

    if (std::holds_alternative<UnknownToken>(ret)) {
        throw LexicalError("Unknown token");
    }

    return ret;
}

Token Parser::Peek(Tokenizer* tok) {
    return tok->Peek();
}

ObjectPointer Parser::ParseExpression(Tokenizer* tok) {
    auto a = ParseMonome(tok);

    while (Peek(tok) == OperationToken::OR) {
        Next(tok);
        auto b = ParseMonome(tok);
        a = std::make_shared<Disjunction>(std::move(a), std::move(b));
    }

    return a;
}

ObjectPointer Parser::ParseMonome(Tokenizer* tok) {
    auto a = ParseAtom(tok);

    while (Peek(tok) == OperationToken::AND) {
        Next(tok);
        auto b = ParseAtom(tok);
        a = std::make_shared<Conjunction>(std::move(a), std::move(b));
    }

    return a;
}

ObjectPointer Parser::ParseAtom(Tokenizer* tok) {
    if (Peek(tok) == BracketToken::OPEN) {
        Next(tok);

        auto ret = ParseExpression(tok);

        if (Peek(tok) != BracketToken::CLOSE) {
            throw SyntaxError("Close bracket expected");
        }

        Next(tok);
        return ret;
    }

    if (Peek(tok) == OperationToken::NOT) {
        Next(tok);
        auto x = ParseAtom(tok);
        return std::make_shared<Negation>(x);
    }

    if (std::holds_alternative<SymbolToken>(Peek(tok))) {
        auto symbol = std::get<SymbolToken>(Next(tok));

        if (symbol.name == "T") {
            return Interpretation::True;
        }

        if (symbol.name == "F") {
            return Interpretation::False;
        }

        return std::make_shared<Variable>(symbol.name);
    }

    throw SyntaxError("Null expression");
}
