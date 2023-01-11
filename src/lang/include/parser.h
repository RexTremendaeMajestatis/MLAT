#pragma once

#include "object.h"
#include "tokenizer.h"

/**
 *  E    ->      E | M      |       M
 *  M    ->      M & A      |       A
 *  A    ->       !A        |      (E)
 *
 *  E - Object
 *  M - Monome
 *  A - Atom
 */

class Parser {
public:
    Parser(Tokenizer* tok) : bracket_counter_(0), tree_(ParseExpression(tok)) {
        if (Next(tok) != EndToken{}) {
            throw SyntaxError("End of token stream was not reached");
        }
    }

    void DebugPrint();

    ObjectPointer GetTree();

    ObjectPointer GetSimplifiedTree();

private:
    int64_t bracket_counter_;

    ObjectPointer tree_;

    Token Next(Tokenizer* tok);

    Token Peek(Tokenizer* tok);

    ObjectPointer ParseExpression(Tokenizer* tok);

    ObjectPointer ParseMonome(Tokenizer* tok);

    ObjectPointer ParseAtom(Tokenizer* tok);
};