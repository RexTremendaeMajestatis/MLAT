#include <gtest/gtest.h>
#include "tokenizer.h"
#include "parser.h"
#include "interpretation.h"
#include "error.h"

TEST(TokenizerTests, Empty)
{
    std::stringstream ss{""};
    Tokenizer tokenizer{&ss};

    ASSERT_TRUE(tokenizer.Next() == EndToken{});
}

TEST(TokenizerTests, SymbolToken)
{
    std::stringstream ss{"a"};
    Tokenizer tokenizer{&ss};

    ASSERT_TRUE(tokenizer.Next() == SymbolToken{.name="a"});
    ASSERT_TRUE(tokenizer.Next() == EndToken{});
}

TEST(TokenizerTests, SymbolTokens)
{
    std::stringstream ss{"foo bar zog-zog"};
    Tokenizer tokenizer{&ss};

    ASSERT_TRUE(tokenizer.Next() == SymbolToken{"foo"});
    ASSERT_TRUE(tokenizer.Next() == SymbolToken{"bar"});
    ASSERT_TRUE(tokenizer.Next() == SymbolToken{"zog-zog"});
}

TEST(TokenizerTests, UnknownToken)
{
    std::stringstream ss{"foo? bar zog-zog"};
    Tokenizer tokenizer{&ss};

    ASSERT_TRUE(tokenizer.Next() == SymbolToken{"foo"});
    ASSERT_TRUE(tokenizer.Next() == UnknownToken{});
}

TEST(ParserTests, UnknownTokenEx)
{
    std::stringstream ss{"x?"};
    Tokenizer tokenizer{&ss};

    ASSERT_THROW(Parser parser{&tokenizer}, LexicalError);
}

TEST(ParserTests, IncorrectSyntaxEx1)
{
    std::stringstream ss{"(x"};
    Tokenizer tokenizer{&ss};

    ASSERT_THROW(Parser parser{&tokenizer}, SyntaxError);
}

TEST(ParserTests, IncorrectSyntaxEx2)
{
    std::stringstream ss{"x)"};
    Tokenizer tokenizer{&ss};

    ASSERT_THROW(Parser parser{&tokenizer}, SyntaxError);
}

TEST(ParserTests, IncorrectSyntaxEx3)
{
    std::stringstream ss{")"};
    Tokenizer tokenizer{&ss};

    ASSERT_THROW(Parser parser{&tokenizer}, SyntaxError);
}

TEST(ParserTests, IncorrectSyntaxEx4)
{
    std::stringstream ss{"!"};
    Tokenizer tokenizer{&ss};

    ASSERT_THROW(Parser parser{&tokenizer}, SyntaxError);
}

TEST(ParserTests, IncorrectSyntaxEx5)
{
    std::stringstream ss{"!)"};
    Tokenizer tokenizer{&ss};

    ASSERT_THROW(Parser parser{&tokenizer}, SyntaxError);
}

TEST(ParserTests, IncorrectSyntaxEx6)
{
    std::stringstream ss{"((a & b) | c"};
    Tokenizer tokenizer{&ss};

    ASSERT_THROW(Parser parser{&tokenizer}, SyntaxError);
}

TEST(ParserTests, IncorrectSyntaxEx7)
{
    std::stringstream ss{"(a & b) | c)"};
    Tokenizer tokenizer{&ss};

    ASSERT_THROW(Parser parser{&tokenizer}, SyntaxError);
}

TEST(ParserTests, IncorrectSyntaxEx8)
{
    std::stringstream ss{"(a & b))"};
    Tokenizer tokenizer{&ss};

    ASSERT_THROW(Parser parser{&tokenizer}, SyntaxError);
}

TEST(ParserTests, IncorrectSyntaxEx9)
{
    std::stringstream ss{"(a &| b)"};
    Tokenizer tokenizer{&ss};

    ASSERT_THROW(Parser parser{&tokenizer}, SyntaxError);
}

TEST(ParserTests, IncorrectSyntaxEx10)
{
    std::stringstream ss{"a &"};
    Tokenizer tokenizer{&ss};

    ASSERT_THROW(Parser parser{&tokenizer}, SyntaxError);
}

TEST(ParserTests, IncorrectSyntaxEx11)
{
    std::stringstream ss{"& a"};
    Tokenizer tokenizer{&ss};

    ASSERT_THROW(Parser parser{&tokenizer}, SyntaxError);
}

TEST(ParserTests, IncorrectSyntaxEx12)
{
    std::stringstream ss{"&"};
    Tokenizer tokenizer{&ss};

    ASSERT_THROW(Parser parser{&tokenizer}, SyntaxError);
}

TEST(ParserTests, IncorrectSyntaxEx13)
{
    std::stringstream ss{"(a & b)) | c"};
    Tokenizer tokenizer{&ss};

    ASSERT_THROW(Parser parser{&tokenizer}, SyntaxError);
}

TEST(ParserTests, CorrectSyntaxEx1)
{
    std::stringstream ss{"a"};
    Tokenizer tokenizer{&ss};

    ASSERT_NO_THROW(Parser parser{&tokenizer});
}

TEST(ParserTests, CorrectSyntaxEx2)
{
    std::stringstream ss{"a & b"};
    Tokenizer tokenizer{&ss};

    ASSERT_NO_THROW(Parser parser{&tokenizer});
}

TEST(ParserTests, CorrectSyntaxEx3)
{
    std::stringstream ss{"(a & b)"};
    Tokenizer tokenizer{&ss};

    ASSERT_NO_THROW(Parser parser{&tokenizer});
}

TEST(ParserTests, CorrectSyntaxEx4)
{
    std::stringstream ss{"((a & b))"};
    Tokenizer tokenizer{&ss};

    ASSERT_NO_THROW(Parser parser{&tokenizer});
}

TEST(ParserTests, CorrectSyntaxEx5)
{
    std::stringstream ss{"!((a & b))"};
    Tokenizer tokenizer{&ss};

    ASSERT_NO_THROW(Parser parser{&tokenizer});
}

TEST(ParserTests, CorrectSyntaxEx6)
{
    std::stringstream ss{"((!!a & !!b))"};
    Tokenizer tokenizer{&ss};

    ASSERT_NO_THROW(Parser parser{&tokenizer});
}

TEST(ParserTests, CorrectSyntaxEx7)
{
    std::stringstream ss{"!!!((!!a & !!b))"};
    Tokenizer tokenizer{&ss};

    ASSERT_NO_THROW(Parser parser{&tokenizer});
}

TEST(ParserTests, CorrectSyntaxEx8)
{
    std::stringstream ss{"!!!(!!!!   !(!  !a & !   !  b))"};
    Tokenizer tokenizer{&ss};

    ASSERT_NO_THROW(Parser parser{&tokenizer});
}

TEST(ParserTests, CorrectSyntaxEx9)
{
    std::stringstream ss{"!(p1 & (p2 | !p3))"};
    Tokenizer tokenizer{&ss};

    ASSERT_NO_THROW(Parser parser{&tokenizer});
}

TEST(EvalTests, Test1)
{
    auto i = Interpretation();
    i.Define("a", Interpretation::False);

    std::stringstream ss{"a & !a"};
    Tokenizer tokenizer{&ss};
    Parser parser{&tokenizer};
    ASSERT_FALSE(parser.GetTree()->Eval(&i) == Interpretation::True);
    ASSERT_TRUE(parser.GetTree()->Eval(&i) == Interpretation::False);
}

TEST(EvalTests, Test2)
{
    auto i = Interpretation();

    std::stringstream ss{"T & F"};
    Tokenizer tokenizer{&ss};
    Parser parser{&tokenizer};
    ASSERT_FALSE(parser.GetTree()->Eval(&i) == Interpretation::True);
    ASSERT_TRUE(parser.GetTree()->Eval(&i) == Interpretation::False);
}