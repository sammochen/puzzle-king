#include "board.h"

#include <gtest/gtest.h>

TEST(BoardTests, DefaultConstructor) { Board board; }

TEST(BoardTests, StringConstructor) {

    std::string s = "rnbqkbnr"
                    "pppppppp"
                    "--------"
                    "--------"
                    "--------"
                    "--------"
                    "PPPPPPPP"
                    "RNBQKBNR";

    Board board(s);
    EXPECT_EQ(s, board.toString());
}

TEST(BoardTests, KingsConstructor) {

    std::string s = "--------"
                    "--------"
                    "---k----"
                    "--------"
                    "---K----"
                    "--------"
                    "--------"
                    "--------";

    Board board(s);
    EXPECT_EQ(s, board.toString());
}
