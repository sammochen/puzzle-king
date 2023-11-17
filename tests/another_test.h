#include "board.h"

#include <gtest/gtest.h>

// Demonstrate some basic assertions.
TEST(BoardTests, DefaultConstructor) {
    // construction
    Board board;
}

TEST(BoardTests, StringConstructor) {
    Board board("rnbqkbnr"
                "pppppppp"
                "--------"
                "--------"
                "--------"
                "--------"
                "PPPPPPPP"
                "RNBQKBNR",
                Color::WHITE);
}

TEST(BoardTests, InCheck) {
    Board board("--R---k-"
                "-----ppp"
                "--------"
                "--------"
                "--------"
                "--------"
                "--------"
                "--------",
                Color::BLACK);

    EXPECT_EQ(board.inCheck(), true);
}

// you are in check if your king is being attacked

