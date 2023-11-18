#include "eval.h"
#include "game.h"

#include <gtest/gtest.h>

TEST(EvalTests, Eval) {
    Board board("rnbqkbnr"
                "pppppppp"
                "--------"
                "--------"
                "--------"
                "--------"
                "PPPPPPPP"
                "RNBQKBNR");

    Eval eval;
    Game game(board, Color::White);
    EXPECT_DOUBLE_EQ(eval.evaluate(game), 0);
}

TEST(EvalTests, EvalWhiteWinning) {

    Board board("rnb-kbnr"
                "pppppppp"
                "--------"
                "--------"
                "--------"
                "--------"
                "PPPPPPPP"
                "RNBQKBNR");

    Eval eval;
    Game game(board, Color::White);
    EXPECT_DOUBLE_EQ(eval.evaluate(game), 0.009);
}

TEST(EvalTests, EvalBlackWinning) {

    Board board("rnbqkbnr"
                "pppppppp"
                "--------"
                "--------"
                "--------"
                "--------"
                "PPPPPPPP"
                "RNB-KBNR");

    Eval eval;
    Game game(board, Color::White);
    EXPECT_DOUBLE_EQ(eval.evaluate(game), -0.009);
}
