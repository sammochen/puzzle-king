#include "board.h"

#include <gtest/gtest.h>

TEST(BoardTests, DefaultConstructor) { Board board; }

TEST(BoardTests, StringConstructor) {
    Board board("rnbqkbnr"
                "pppppppp"
                "--------"
                "--------"
                "--------"
                "--------"
                "PPPPPPPP"
                "RNBQKBNR");
}
