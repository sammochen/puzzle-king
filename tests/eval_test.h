#include "eval.h"

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
    EXPECT_EQ(eval.evaluate(board), 0);
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
    EXPECT_EQ(eval.evaluate(board), 9);
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
    EXPECT_EQ(eval.evaluate(board), -9);
}
